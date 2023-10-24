/**
* @file     vka_texture_impl.h
* @brief    Texture implemenation file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

vka::Texture::Texture(VkDevice device) noexcept :
    device(device),
    image(VK_NULL_HANDLE),
    memory(VK_NULL_HANDLE),
    m_sampler(VK_NULL_HANDLE),
    m_extent({0, 0, 0}), 
    m_level_count(0),
    m_layer_count(0),
    format(VK_FORMAT_MAX_ENUM),
    state(STATE_INVALID)
{}

vka::Texture::Texture(Texture&& src) noexcept :
    device(src.device), 
    image(src.image),
    memory(src.memory), 
    m_sampler(src.m_sampler), 
    views(std::move(src.views)), 
    m_extent(src.m_extent), 
    m_level_count(src.m_level_count),
    m_layer_count(src.m_layer_count),
    format(src.format),
    state(src.state)
{
    src.image = VK_NULL_HANDLE;
    src.memory = VK_NULL_HANDLE;
    src.m_sampler = VK_NULL_HANDLE;
    src.m_extent = { 0, 0, 0 };
    src.m_level_count = 0;
    src.m_layer_count = 0;
    src.format = VK_FORMAT_MAX_ENUM;
    src.state = STATE_INVALID;
}

vka::Texture& vka::Texture::operator= (Texture&& src) noexcept
{
    // destroyes the texture, if it has been created, otherwise this function does nothing
    this->destroy_handles();
    this->device = src.device;
    this->image = src.image;
    this->memory = src.memory;
    this->m_sampler = src.m_sampler;
    this->views = std::move(src.views);
    this->m_extent = src.m_extent;
    this->m_level_count = src.m_level_count;
    this->m_layer_count = src.m_layer_count;
    this->format = src.format;
    this->state = src.state;
    src.image = VK_NULL_HANDLE;
    src.memory = VK_NULL_HANDLE;
    src.m_sampler = VK_NULL_HANDLE;
    src.m_extent = { 0, 0, 0 };
    src.m_level_count = 0;
    src.m_layer_count = 0;
    src.format = VK_FORMAT_MAX_ENUM;
    src.state = STATE_INVALID;
    return *this;
}

vka::Texture::~Texture(void)
{
    this->destroy_handles();
}

void vka::Texture::init(VkDevice device) noexcept
{
    if (!this->is_valid())
        this->device = device;
}

void vka::Texture::create(const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info)
{
    if (!this->is_valid())
    {
        this->validate();

        // calculate number of mip map levels
        this->m_extent = create_info.imageExtent;
        this->m_level_count = create_info.generateMipMap ? level_count(create_info.imageExtent) : 1;
        this->m_layer_count = create_info.imageArrayLayers;
        this->format = create_info.imageFormat;

        // create image
        const VkImageCreateInfo image_ci = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = create_info.imageFlags,
            .imageType = create_info.imageType,
            .format = create_info.imageFormat,
            .extent = create_info.imageExtent,
            .mipLevels = this->m_level_count,
            .arrayLayers = create_info.imageArrayLayers,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            .sharingMode = create_info.imageQueueFamilyIndexCount > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = create_info.imageQueueFamilyIndexCount,
            .pQueueFamilyIndices = create_info.imageQueueFamilyIndices,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };
        detail::error::check_result(vkCreateImage(this->device, &image_ci, nullptr, &this->image), IMAGE_CREATE_FAILED);

        // query memory requirements
        VkMemoryRequirements requirements;
        vkGetImageMemoryRequirements(this->device, this->image, &requirements);

        // allocate memory
        const VkMemoryAllocateInfo memory_ai = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = requirements.size,
            .memoryTypeIndex = memory::find_type_index(properties, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        };
        detail::error::check_result(vkAllocateMemory(this->device, &memory_ai, nullptr, &this->memory), ALLOC_MEMORY_FAILED);
        detail::error::check_result(vkBindImageMemory(this->device, this->image, this->memory, 0), BIND_MEMORY_FAILED);

        // create sampler
        const VkSamplerCreateInfo sampler_ci = {
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
        detail::error::check_result(vkCreateSampler(this->device, &sampler_ci, nullptr, &this->m_sampler), SAMPLER_CREATE_FAILED);
        this->state = STATE_CREATED;
    }
}

void vka::Texture::create_view(const TextureViewCreateInfo& create_info)
{
    if (this->is_valid())
    {
        const VkImageViewCreateInfo ci = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = create_info.flags,
        .image = this->image,
        .viewType = create_info.viewType,
        .format = create_info.format,
        .components = create_info.components,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = this->m_level_count,
            .baseArrayLayer = create_info.baseArrayLayer,
            .layerCount = create_info.layerCount
        }
        };
        VkImageView view;
        detail::error::check_result(vkCreateImageView(this->device, &ci, nullptr, &view), VIEW_CREATE_FAILED);
        this->views.push_back(view);
    }
}

void vka::Texture::destroy(void) noexcept
{
    this->destroy_handles();
    this->image = VK_NULL_HANDLE;
    this->memory = VK_NULL_HANDLE;
    this->m_sampler = VK_NULL_HANDLE;
    this->views.clear();
    this->m_extent = { 0, 0, 0 };
    this->m_level_count = 0;
    this->m_layer_count = 0;
    this->format = VK_FORMAT_MAX_ENUM;
    this->state = STATE_INVALID;
}

void vka::Texture::create_mip_levels(VkCommandBuffer cbo) noexcept
{
    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = this->image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0xFFFFFFFF,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = this->m_layer_count
        }
    };

    VkImageBlit blit = {
        .srcSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0xFFFFFFFF,
            .baseArrayLayer = 0,
            .layerCount = this->m_layer_count,
        },
        .srcOffsets = {{}, {}},
        .dstSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0xFFFFFFFF,
            .baseArrayLayer = 0,
            .layerCount = this->m_layer_count
        },
        .dstOffsets = {{}, {}}
    };

    VkOffset3D extent = {
        (int32_t)this->m_extent.width,
        (int32_t)this->m_extent.height,
        (int32_t)this->m_extent.depth
    };

    for (uint32_t i = 1; i < this->m_level_count; i++)
    {
        barrier.subresourceRange.baseMipLevel = i - 1;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = extent;
            
        const int32_t tmpx = extent.x >>= 1;
        const int32_t tmpy = extent.y >>= 1;
        const int32_t tmpz = extent.z >>= 1;
        extent.x = tmpx < 1 ? 1 : tmpx;
        extent.y = tmpy < 1 ? 1 : tmpy;
        extent.z = tmpz < 1 ? 1 : tmpz;

        blit.dstSubresource.mipLevel = i;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = extent;

        vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
        vkCmdBlitImage(cbo, this->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, this->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_NEAREST);
    }
}

void vka::Texture::load(VkCommandBuffer cbo, const vka::Buffer& data, uint32_t layer, uint32_t layer_count, uint32_t level) noexcept
{
    if (this->state != STATE_INVALID && data.is_valid())
    {
        // If we are not in loading state, change the layout to the requiered layout
        // for the loading state and set the state to "loading".
        if (this->state == STATE_CREATED)
        {
            this->change_layout_C2L(cbo);
            this->state = STATE_LOADING;
        }
        else if (this->state == STATE_FINISHED)
        {
            this->change_layout_F2L(cbo);
            this->state = STATE_LOADING;
        }
        
        // copy buffer into the image
        const VkExtent3D extent = this->size(level);
        const VkBufferImageCopy region = {
            .bufferOffset = 0,
            .bufferRowLength = extent.width,
            .bufferImageHeight = extent.height,
            .imageSubresource = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel = level,
                .baseArrayLayer = layer,
                .layerCount = layer_count
            },
            .imageOffset = {0, 0, 0},
            .imageExtent = extent
        };
        vkCmdCopyBufferToImage(cbo, data.handle(), this->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    }
}

void vka::Texture::load_staging(const void* const* data, vka::Buffer& buffer, const VkPhysicalDeviceMemoryProperties& properties, uint32_t qfamidx, uint32_t layer_count, uint32_t level) const
{
    if (this->state != STATE_INVALID)
    {
        const VkExtent3D extent = this->size(level);
        const VkDeviceSize layer_size = (VkDeviceSize)extent.width * extent.height * extent.depth * utility::format_sizeof(this->format);
        const BufferCreateInfo create_info = {
            .bufferFlags = 0,
            .bufferSize = layer_size * layer_count,
            .bufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .bufferSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .bufferQueueFamilyIndexCount = 1,
            .bufferQueueFamilyIndices = &qfamidx,
            .memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT
        };

        // create staging buffer
        vka::Buffer staging(this->device);
        staging.create(properties, create_info);

        // load data into staging buffer
        void* map = staging.map(0, create_info.bufferSize);

        for (uint32_t i = 0; i < layer_count; i++)
            memcpy(addvp(map, i * layer_size), data[i], layer_size);

        staging.unmap();
        buffer = std::move(staging);
    }
}

void vka::Texture::finish(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept
{
    if (this->state == STATE_LOADING)
    {
        // only create mip levels, if the texture is in loading state
        // generating mip levels without loaded data will not have any effect
        if (this->m_level_count > 1)   // if mip levels should be created
        {
            this->create_mip_levels(cbo);
            this->change_layout_M2F(cbo, stages);
        }
        else    // if mip levels should not be created
        {
            this->change_layout_L2F(cbo, stages);
        }
        this->state = STATE_FINISHED;
    }
    else if (this->state == STATE_CREATED)
    {
        this->change_layout_C2F(cbo, stages);
        this->state = STATE_FINISHED;
    }
}

void vka::Texture::finish_manual(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept
{
    if (this->state == STATE_LOADING)
    {
        this->change_layout_L2F(cbo, stages);
        this->state = STATE_FINISHED;
    }
    else if (this->state == STATE_CREATED)
    {
        this->change_layout_C2F(cbo, stages);
        this->state = STATE_FINISHED;
    }
}
