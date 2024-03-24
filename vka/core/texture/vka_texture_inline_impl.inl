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

inline vka::Texture::Texture(void) noexcept :
    m_device(VK_NULL_HANDLE),
    m_image(VK_NULL_HANDLE),
    m_memory(VK_NULL_HANDLE),
    m_sampler(VK_NULL_HANDLE),
    m_extent({0, 0, 0}),
    m_level_count(0),
    m_layer_count(0),
    m_format(VK_FORMAT_MAX_ENUM),
    m_state(STATE_INVALID)
{}

inline vka::Texture::Texture(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info) :
   m_device(device),
   m_image(VK_NULL_HANDLE),
   m_memory(VK_NULL_HANDLE),
   m_sampler(VK_NULL_HANDLE),
   m_extent(create_info.imageExtent),
   m_level_count(create_info.generateMipMap ? level_count(create_info.imageExtent) : 1),
   m_layer_count(create_info.imageArrayLayers),
   m_format(create_info.imageFormat),
   m_state(STATE_INVALID)
{
    this->internal_create(properties, create_info);
}

inline vka::Texture::Texture(Texture&& src) noexcept :
    m_device(src.m_device),
    m_image(src.m_image),
    m_memory(src.m_memory),
    m_sampler(src.m_sampler),
    m_views(std::move(src.m_views)),
    m_extent(src.m_extent),
    m_level_count(src.m_level_count),
    m_layer_count(src.m_layer_count),
    m_format(src.m_format),
    m_state(src.m_state)
{
    src.m_image = VK_NULL_HANDLE;
    src.m_memory = VK_NULL_HANDLE;
    src.m_sampler = VK_NULL_HANDLE;
    src.m_extent = { 0, 0, 0 };
    src.m_level_count = 0;
    src.m_layer_count = 0;
    src.m_format = VK_FORMAT_MAX_ENUM;
    src.m_state = STATE_INVALID;
}

inline vka::Texture& vka::Texture::operator= (Texture&& src) noexcept
{
    // destroys the texture, if it has been created, otherwise this function does nothing
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_image = src.m_image;
    this->m_memory = src.m_memory;
    this->m_sampler = src.m_sampler;
    this->m_views = std::move(src.m_views);
    this->m_extent = src.m_extent;
    this->m_level_count = src.m_level_count;
    this->m_layer_count = src.m_layer_count;
    this->m_format = src.m_format;
    this->m_state = src.m_state;
    src.m_image = VK_NULL_HANDLE;
    src.m_memory = VK_NULL_HANDLE;
    src.m_sampler = VK_NULL_HANDLE;
    src.m_extent = { 0, 0, 0 };
    src.m_level_count = 0;
    src.m_layer_count = 0;
    src.m_format = VK_FORMAT_MAX_ENUM;
    src.m_state = STATE_INVALID;
    return *this;
}

inline vka::Texture::~Texture(void)
{
    this->destroy_handles();
}


inline void vka::Texture::create(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info)
{
    if (!this->is_valid())
    {
        this->m_device = device;
        this->m_extent = create_info.imageExtent;
        this->m_level_count = create_info.generateMipMap ? level_count(create_info.imageExtent) : 1;
        this->m_layer_count = create_info.imageArrayLayers;
        this->m_format = create_info.imageFormat;

        this->internal_create(properties, create_info);
    }
}

inline void vka::Texture::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_image = VK_NULL_HANDLE;
    this->m_memory = VK_NULL_HANDLE;
    this->m_sampler = VK_NULL_HANDLE;
    this->m_views.clear();
    this->m_extent = { 0, 0, 0 };
    this->m_level_count = 0;
    this->m_layer_count = 0;
    this->m_format = VK_FORMAT_MAX_ENUM;
    this->m_state = STATE_INVALID;
}

inline uint32_t vka::Texture::log2i(uint32_t x) noexcept
{
#ifdef VKA_X86
    unsigned long y;    // BSF instruction finds last '1' bit and its position is the log2(x) of an integer value.
    return _BitScanReverse(&y, x) == 0 ? 0xFFFFFFFF : y;
#else
    uint32_t bit = 0xFFFFFFFF;
    while (x > 0)
    {
        x >>= 1;    // logic right shift, shifts in zeros
        ++bit;
    }
    return bit;
#endif
}

inline uint32_t vka::Texture::max_log2i(VkExtent3D extent) noexcept
{
    // find maximum of the 3 components
    uint32_t m = extent.width > extent.height ? extent.width : extent.height;
    m = extent.depth > m ? extent.depth : m;
    return log2i(m); // a > b -> log(a) > log(b)
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
inline void* vka::Texture::load_image_internal(const char* path, VkExtent3D& extent, uint32_t* components, const uint32_t force_components)
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
