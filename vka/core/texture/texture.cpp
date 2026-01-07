/**
 * @brief Implementation for the texture class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include <vka/vka.h>

// Range for image array layers [1, 65536] is mapped to [0, 65535] to fit in an uint16_t. No texture with a layer count
// of 0 can be created. Therefore, a layer count of 0 is invalid anyway.
vka::Texture::Texture(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info) :
    m_texture(device, create_texture(device, properties, create_info)),
    m_extent(create_info.imageExtent),
    m_layer_count(create_info.imageArrayLayers - 1),
    m_level_count(mip_level_count(create_info))
{
    this->change_layout_C2L(create_info.commandBuffer);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void vka::Texture::load(VkCommandBuffer cbo, const Buffer& data, uint32_t layer, uint32_t count, uint32_t level) noexcept
{
    const VkImageSubresourceLayers layers = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel = level,
        .baseArrayLayer = layer,
        .layerCount = count
    };
    const VkBufferImageCopy region = {
        .bufferOffset = 0,
        .bufferRowLength = 0,
        .bufferImageHeight = 0,
        .imageSubresource = layers,
        .imageOffset = ZERO_OFFSET,
        .imageExtent = common::mip_extent(this->m_extent, level)
    };
    vkCmdCopyBufferToImage(cbo, data.handle(), this->m_texture.get().image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void vka::Texture::finish(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept
{
    // If mipmap levels should be created, then m_level_count is greater than 1!
    if (this->m_level_count > 1)
    {
        this->create_mipmap(cbo);
        this->change_layout_M2F(cbo, stages);
    }
    else
    {
        this->change_layout_L2F(cbo, stages);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
void vka::Texture::finish_manual(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept
{
    this->change_layout_L2F(cbo, stages);
}


vka::Texture::TextureHandle vka::Texture::create_texture(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info)
{
    // create image
    const uint32_t level_count = mip_level_count(create_info);
    const VkImageCreateInfo image_create_info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = create_info.imageFlags,
        .imageType = create_info.imageType,
        .format = create_info.imageFormat,
        .extent = create_info.imageExtent,
        .mipLevels = level_count,
        .arrayLayers = create_info.imageArrayLayers,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        .sharingMode = create_info.imageQueueFamilyIndexCount > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = create_info.imageQueueFamilyIndexCount,
        .pQueueFamilyIndices = create_info.imageQueueFamilyIndices,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };
    VkImage image;
    check_result(vkCreateImage(device, &image_create_info, nullptr, &image), IMAGE_CREATE_FAILED);
    unique_handle image_guard(device, image);

    // query memory requirements
    VkMemoryRequirements requirements;
    vkGetImageMemoryRequirements(device, image, &requirements);

    // allocate memory
    const VkMemoryAllocateInfo allocate_info = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = requirements.size,
        .memoryTypeIndex = memory::find_type_index(properties, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    };
    VkDeviceMemory memory;
    check_result(vkAllocateMemory(device, &allocate_info, nullptr, &memory), ALLOC_MEMORY_FAILED);
    unique_handle memory_guard(device, memory);
    check_result(vkBindImageMemory(device, image, memory, 0), BIND_MEMORY_FAILED);

    // create sampler
    const VkSamplerCreateInfo sampler_create_info = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .magFilter = create_info.samplerMagFilter,
        .minFilter = create_info.samplerMinFilter,
        .mipmapMode = create_info.samplerMipmapMode,
        .addressModeU = create_info.samplerAddressModeU,
        .addressModeV = create_info.samplerAddressModeV,
        .addressModeW = create_info.samplerAddressModeW,
        .mipLodBias = create_info.samplerLodBias,
        .anisotropyEnable = create_info.samplerAnisotropyEnable,
        .maxAnisotropy = create_info.samplerMaxAnisotropy,
        .compareEnable = create_info.samplerCompareEnable,
        .compareOp = create_info.samplerCompareOp,
        .minLod = create_info.samplerMinLod,
        .maxLod = create_info.samplerMaxLod,
        .borderColor = create_info.samplerBorderColor,
        .unnormalizedCoordinates = create_info.samplerUnnormalizedCoordinates
    };
    VkSampler sampler;
    check_result(vkCreateSampler(device, &sampler_create_info, nullptr, &sampler), SAMPLER_CREATE_FAILED);
    unique_handle sampler_guard(device, sampler);

    // create image views
    unique_handle<VkImageView[]> views(device, new VkImageView[create_info.viewCount]{ VK_NULL_HANDLE }, create_info.viewCount);
    for (uint32_t i = 0; i < create_info.viewCount; i++)
    {
        const TextureViewCreateInfo texture_view = create_info.views[i];
        const VkImageSubresourceRange range = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = level_count,
            .baseArrayLayer = texture_view.baseArrayLayer,
            .layerCount = texture_view.layerCount
        };
        const VkImageViewCreateInfo view_create_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = texture_view.flags,
            .image = image,
            .viewType = texture_view.viewType,
            .format = texture_view.format,
            .components = texture_view.components,
            .subresourceRange = range
        };
        check_result(vkCreateImageView(device, &view_create_info, nullptr, views.get() + i), VIEW_CREATE_FAILED);
    }

    const uint32_t view_count = views.size();
    return {
        .image = image_guard.release(),
        .memory = memory_guard.release(),
        .sampler = sampler_guard.release(),
        .views = views.release(),
        .view_count = view_count
    };
}

inline void vka::Texture::change_layout_C2L(VkCommandBuffer cbo) const noexcept
{
    const VkImageSubresourceRange range = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = this->m_level_count,
        .baseArrayLayer = 0,
        .layerCount = this->layer_count()
    };
    const VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_NONE,
        .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = this->m_texture.get().image,
        .subresourceRange = range
    };
    vkCmdPipelineBarrier(
        cbo,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier
    );
}

inline void vka::Texture::change_layout_L2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) const noexcept
{
    const VkImageSubresourceRange range = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = this->m_level_count,
        .baseArrayLayer = 0,
        .layerCount = this->layer_count()
    };
    const VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = this->m_texture.get().image,
        .subresourceRange = range
    };
    vkCmdPipelineBarrier(
        cbo,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        stages,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier
    );
}

inline void vka::Texture::change_layout_M2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) const noexcept
{
    const VkImageSubresourceRange ranges[2] = {
        {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = (uint32_t)this->m_level_count - 1,
            .baseArrayLayer = 0,
            .layerCount = this->layer_count()
        },
        {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = (uint32_t)this->m_level_count - 1,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = this->layer_count()
        }
    };
    const VkImageMemoryBarrier barriers[2] = {
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
            .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = this->m_texture.get().image,
            .subresourceRange = ranges[0]
        },
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
            .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = this->m_texture.get().image,
            .subresourceRange = ranges[1]
        }
    };
    vkCmdPipelineBarrier(
        cbo,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        stages,
        0,
        0,
        nullptr,
        0,
        nullptr,
        2,
        barriers
    );
}

void vka::Texture::create_mipmap(VkCommandBuffer cbo) const noexcept
{
    const VkImageSubresourceRange init_range = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0, // is set in loop
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = this->layer_count()
    };
    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = this->m_texture.get().image,
        .subresourceRange = init_range
    };
    VkOffset3D src_extent = {
        (int32_t)this->m_extent.width,
        (int32_t)this->m_extent.height,
        (int32_t)this->m_extent.depth
    };

    for (uint32_t i = 1; i < this->m_level_count; i++)
    {
        const uint32_t src_level = i - 1;
        const VkImageSubresourceLayers src_layers = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = src_level,
            .baseArrayLayer = 0,
            .layerCount = this->layer_count()
        };

        const uint32_t dst_level = i;
        const VkOffset3D dst_extent = common::next_mip_extent(src_extent);
        const VkImageSubresourceLayers dst_layers = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = dst_level,
            .baseArrayLayer = 0,
            .layerCount = this->layer_count()
        };

        const VkImageBlit blit = {
            .srcSubresource = src_layers,
            .srcOffsets = { ZERO_OFFSET, src_extent },
            .dstSubresource = dst_layers,
            .dstOffsets = { ZERO_OFFSET, dst_extent }
        };
        barrier.subresourceRange.baseMipLevel = src_level;

        vkCmdPipelineBarrier(
            cbo,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &barrier
        );
        vkCmdBlitImage(
            cbo,
            this->m_texture.get().image,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            this->m_texture.get().image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &blit,
            VK_FILTER_NEAREST
        );
        src_extent = dst_extent;
    }
}

vka::Buffer vka::Texture::stage(VkDevice device, const void* data, VkDeviceSize size, TextureLoadInfo info)
{
    const BufferCreateInfo crate_info = {
        .pBufferNext = nullptr,
        .bufferFlags = 0,
        .bufferSize = size,
        .bufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .bufferSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .bufferQueueFamilyIndexCount = 1,
        .bufferQueueFamilyIndices = &info.queueFamilyIndex,
        .pMemoryNext = nullptr,
        .memoryPropertyFlags = info.memoryPropertyFlags
    };
    Buffer buffer(device, *info.memoryProperties, crate_info);
    memcpy(buffer.map(), data, size);
    buffer.unmap();
    return buffer;
}
