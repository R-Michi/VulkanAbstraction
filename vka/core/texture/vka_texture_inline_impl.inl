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
    if (this->m_device == VK_NULL_HANDLE) [[unlikely]]
        detail::error::throw_invalid_argument("[vka::Texture::create]: Device is a VK_NULL_HANDLE.");
}

inline void vka::Texture::destroy_handles(void) noexcept
{
    for (VkImageView view : m_views)  // VK_NULL_HANDLE views are not added
        vkDestroyImageView(this->m_device, view, nullptr);
    if (this->m_sampler != VK_NULL_HANDLE)
        vkDestroySampler(this->m_device, this->m_sampler, nullptr);
    if (this->m_memory != VK_NULL_HANDLE)
        vkFreeMemory(this->m_device, this->m_memory, nullptr);
    if (this->m_image != VK_NULL_HANDLE)
        vkDestroyImage(this->m_device, this->m_image, nullptr);
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
        .image = this->m_image,
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
        .image = this->m_image,
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
    // as we don't know the specific shader pipeline stages. But all commands includes any shader
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
            .image = this->m_image,
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
            .image = this->m_image,
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
        .image = this->m_image,
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
        .image = this->m_image,
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

inline void vka::Texture::load_staging(const void* data, vka::Buffer& buffer, const VkPhysicalDeviceMemoryProperties& properties, uint32_t qfamidx, uint32_t level) const
{
    this->load_staging(&data, buffer, properties, qfamidx, 1, level);
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
    return this->m_image;
}

inline VkSampler vka::Texture::sampler(void) const noexcept
{
    return this->m_sampler;
}

inline VkImageView vka::Texture::view(size_t i) const
{
    return this->m_views.at(i);
}

inline VkImageView vka::Texture::viewu(size_t i) const noexcept
{
    return this->m_views[i];
}

inline bool vka::Texture::is_valid(void) const noexcept
{
    return this->m_state != STATE_INVALID;
}

#ifdef VKA_STB_IMAGE_LOAD_ENABLE
template<vka::ImageDataType Type>
inline void* vka::Texture::load_image_internal(const char* path, VkExtent3D& extent, uint32_t* components, const uint32_t force_components) noexcept
{
    void* data = nullptr;
    int w, h;
    if constexpr        (Type == VKA_IMAGE_DATA_TYPE_INT8)  data = stbi_load(path, &w, &h, reinterpret_cast<int*>(components), (int)force_components);
    else if constexpr   (Type == VKA_IMAGE_DATA_TYPE_INT16) data = stbi_load_16(path, &w, &h, reinterpret_cast<int*>(components), (int)force_components);
    else if constexpr   (Type == VKA_IMAGE_DATA_TYPE_FLOAT) data = stbi_loadf(path, &w, &h, reinterpret_cast<int*>(components), (int)force_components);
    else                static_assert(false, "[vka::Texture::load_image]: Cannot load image with specified data type.");

    // image loading failed
    if (data == nullptr) [[unlikely]]
        detail::error::throw_runtime_error(IMAGE_LOAD_FAILED);

    extent = { (uint32_t)w, (uint32_t)h, 1 };
    return data;
}

template<vka::ImageDataType Type>
inline void* vka::Texture::load_image(const char* path, VkExtent3D& extent, uint32_t& components) noexcept
{
    return load_image_internal<Type>(path, extent, &components, 0);
}

template<vka::ImageDataType Type, uint32_t force_components>
inline void* vka::Texture::load_image(const char* path, VkExtent3D& extent) noexcept
{
    static_assert(
        force_components <= 4, 
        "[vka::Texture::load_image<Type, force_components>]: Template argument \"force_components\" "
        "must not be greater than 4"
    );
    static_assert(
        force_components != 0,
        "[vka::Texture::load_image<Type, force_components>]: For loading automatically the number of color compoents, use the function "
        "without the \"force_components\" template argument."
    );
    return load_image_internal<Type>(path, extent, nullptr, force_components);
}

inline void vka::Texture::free_image(void* data) noexcept
{
    std::free(data);
}

#endif
