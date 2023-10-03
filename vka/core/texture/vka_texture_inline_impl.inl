/**
* @file     vka_texture_inline_impl.inl
* @brief    Buffer implementation file of inline functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline void vka::Texture::validate(void)
{
    if (this->device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Texture::create]: Device is a VK_NULL_HANDLE.");
}

inline void vka::Texture::destroy_handles(void) noexcept
{
    for (VkImageView view : views)  // VK_NULL_HANDLE view's are not added
        vkDestroyImageView(this->device, view, nullptr);
    if (this->m_sampler != VK_NULL_HANDLE)
        vkDestroySampler(this->device, this->m_sampler, nullptr);
    if (this->memory != VK_NULL_HANDLE)
        vkFreeMemory(this->device, this->memory, nullptr);
    if (this->image != VK_NULL_HANDLE)
        vkDestroyImage(this->device, this->image, nullptr);
}

inline uint32_t vka::Texture::log2i(uint32_t x) noexcept
{
    unsigned long y;    // BSF instruction finds last '1' bit and its position is the log2(x) of an integer value.
    return _BitScanReverse(&y, x) == 0 ? 0xFFFFFFFF : y;
}

inline uint32_t vka::Texture::max_log2i(VkExtent3D extent) noexcept
{
    // find maximum of the 3 components
    uint32_t m = extent.width > extent.height ? extent.width : extent.height;
    m = extent.depth > m ? extent.depth : m;
    return log2i(m); // a > b -> log(a) > log(b)
}

inline void* vka::Texture::addvp(void* vp, uint64_t offset) noexcept
{
    const uint64_t vp_u64 = reinterpret_cast<uint64_t>(vp);
    return reinterpret_cast<void*>(vp_u64 + offset);
}

inline void vka::Texture::change_layout_C2L(VkCommandBuffer cbo) noexcept
{
    const VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_NONE,
        .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = this->image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = this->m_level_count,
            .baseArrayLayer = 0,
            .layerCount = this->m_layer_count
        }
    };
    vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

inline void vka::Texture::change_layout_F2L(VkCommandBuffer cbo) noexcept
{
    const VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
        .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = this->image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = this->m_level_count,
            .baseArrayLayer = 0,
            .layerCount = this->m_layer_count
        }
    };
    // Also synchronize between shader read operations and write operations of the loading process.
    // The transfer operations must wait for every shader read operation from all previous commands
    // as we dont know the specific shader pipeline stages. But all commands includes any shader
    // pipeline stage that reads from the texture.
    vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

inline void vka::Texture::change_layout_M2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept
{
    const VkImageMemoryBarrier barriers[2] = {
        // this barrier changes the layout from the first n-1 mip levels to shader read
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
            .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
            .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = this->image,
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = this->m_level_count - 1,
                .baseArrayLayer = 0,
                .layerCount = this->m_layer_count
            }
        },
        // this barrier changes the layout from the last mip level to shader read
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
            .dstAccessMask = VK_ACCESS_SHADER_READ_BIT, 
            .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = this->image,
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = this->m_level_count - 1,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = this->m_layer_count
            }
        }
    };
    // also synchronize between read/write operations of the mip-map level creation process and shader read operations
    vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_TRANSFER_BIT, stages, 0, 0, nullptr, 0, nullptr, 2, barriers);
}

inline void vka::Texture::change_layout_C2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept
{
    const VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_NONE,
        .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = this->image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = this->m_level_count,
            .baseArrayLayer = 0,
            .layerCount = this->m_layer_count
        }
    };
    vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, stages, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

inline void vka::Texture::change_layout_L2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept
{
    const VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext = nullptr,
        .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
        .dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
        .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = this->image,
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = this->m_level_count,
            .baseArrayLayer = 0,
            .layerCount = this->m_layer_count
        }
    };
    // also synchronize between write operations of the loading process and shader read operations
    vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_TRANSFER_BIT, stages, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

inline VkResult vka::Texture::load_staging(const void* data, vka::Buffer& buffer, const VkPhysicalDeviceMemoryProperties& properties, uint32_t qfamidx, uint32_t level) const noexcept
{
    return this->load_staging(&data, buffer, properties, qfamidx, 1, level);
}

inline VkExtent3D vka::Texture::size(uint32_t level) const noexcept
{
    if (level == 0) return this->m_extent;
    const uint32_t tmpx = this->m_extent.width >> level;
    const uint32_t tmpy = this->m_extent.height >> level;
    const uint32_t tmpz = this->m_extent.depth >> level;
    return {
        tmpx < 1 ? 1 : tmpx,
        tmpy < 1 ? 1 : tmpy,
        tmpz < 1 ? 1 : tmpz
    };
}

inline VkExtent3D vka::Texture::size(void) const noexcept
{
    return this->m_extent;
}

inline uint32_t vka::Texture::level_count(void) const noexcept
{
    return this->m_level_count;
}

inline uint32_t vka::Texture::level_count(VkExtent3D extent) noexcept
{
    return max_log2i(extent) + 1;
}

inline uint32_t vka::Texture::layer_count(void) const noexcept
{
    return this->m_layer_count;
}

inline VkImage vka::Texture::handle(void) const noexcept
{
    return this->image;
}

inline VkSampler vka::Texture::sampler(void) const noexcept
{
    return this->m_sampler;
}

inline VkImageView vka::Texture::view(size_t i) const
{
    return this->views.at(i);
}

inline VkImageView vka::Texture::viewu(size_t i) const noexcept
{
    return this->views[i];
}

inline bool vka::Texture::is_valid(void) const noexcept
{
    return this->state != STATE_INVALID;
}

#ifdef VKA_STB_IMAGE_LOAD_ENABLE

template<vka::ImageDataType Type>
inline void* vka::Texture::load_image(const char* path, VkExtent3D& extent, uint32_t force_components, uint32_t* components) noexcept
{
    int w, h, c;
    void* data = nullptr;
    if constexpr (Type == VKA_IMAGE_DATA_TYPE_INT8)
        data = stbi_load(path, &w, &h, &c, (int)force_components);
    else if constexpr (Type == VKA_IMAGE_DATA_TYPE_INE16)
        data = stbi_load_16(path, &w, &h, &c, (int)force_components);
    else if constexpr (Type == VKA_IMAGE_DATA_TYPE_FLOAT)
        data = stbi_loadf(path, &w, &h, &c, (int)force_components);
    else
        static_assert("[vka::utility::load_image]: false image type.");
    extent = { (uint32_t)w, (uint32_t)h, 1};
    if (components != nullptr)
        *components = (uint32_t)c;
    return data;
}

inline void vka::Texture::free_image(void* data) noexcept
{
    std::free(data);
}

#endif
