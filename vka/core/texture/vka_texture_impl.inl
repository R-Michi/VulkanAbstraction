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

vka::Texture::Texture(void) noexcept : Texture(VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE) {}   

vka::Texture::Texture(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool, VkQueue queue) noexcept
{
    this->_pyhsical_device = physical_device;
    this->_device = device;
    this->_pool = cmd_pool;
    this->_queue = queue;

    this->init_ici(this->_ici);
    this->init_ici(this->_backup_ici);
    this->init_sci(this->_sci);
    this->init_sci(this->_backup_sci);

    this->_image = VK_NULL_HANDLE;
    this->_memory = VK_NULL_HANDLE;
    this->_sampler = VK_NULL_HANDLE;

    this->_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    this->_mem_size = 0;
}

vka::Texture::Texture(Texture&& src) : Texture()
{
    *this = std::move(src);
}

vka::Texture& vka::Texture::operator= (Texture&& src)
{
    this->clear();

    this->_pyhsical_device = src._pyhsical_device;
    this->_device = src._device;
    this->_pool = src._pool;
    this->_queue = src._queue;

    this->set_image_create_info(src._ici);
    this->set_sampler_create_info(src._sci);
    this->_backup_ici = src._backup_ici;
    this->_backup_sci = src._backup_sci;
    this->_vcis = src._vcis;

    this->_properties = src._properties;
    this->_mem_size = src._mem_size;

    this->_image = src._image;
    this->_memory = src._memory;
    this->_sampler = src._sampler;
    this->_views = src._views;
    for(size_t i = 0; i < this->_staging_buffers.size(); i++)
        this->_staging_buffers.at(i) = std::move(src._staging_buffers.at(i));

    src._image = VK_NULL_HANDLE;
    src._memory = VK_NULL_HANDLE;
    src._sampler = VK_NULL_HANDLE;
    src._views.clear();
    src.clear();
    return *this;
}

vka::Texture::~Texture(void)
{
    this->clear();
}

void vka::Texture::init_ici(VkImageCreateInfo& ci)
{
    ci.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ci.pNext = nullptr;
    ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ci.samples = VK_SAMPLE_COUNT_1_BIT;
    ci.tiling = VK_IMAGE_TILING_OPTIMAL;
    ci.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
}
void vka::Texture::init_sci(VkSamplerCreateInfo& ci)
{
    ci.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    ci.pNext = nullptr;
    ci.flags = 0;
}

void vka::Texture::set_image_flags(VkImageCreateFlags flags) noexcept
{
    this->_ici.flags = flags;
}
void vka::Texture::set_image_type(VkImageType type) noexcept
{
    this->_ici.imageType = type;
}
void vka::Texture::set_image_format(VkFormat format) noexcept
{
    this->_ici.format = format;
}
void vka::Texture::set_image_extent(VkExtent3D extent) noexcept
{
    this->_ici.extent = extent;
}
void vka::Texture::set_image_array_layers(uint32_t layers) noexcept
{   
    this->_ici.arrayLayers = layers;
    this->_staging_buffers.resize(layers);
    this->_staging_buffers.shrink_to_fit();
}
void vka::Texture::set_image_queue_families(uint32_t queue_family_index) noexcept
{
    // only one queue family can be used as the sharing mode is EXCLUSIVE
    this->_ici.queueFamilyIndexCount = 1;
    this->_ici.pQueueFamilyIndices = &queue_family_index;
}
void vka::Texture::set_image_create_info(const VkImageCreateInfo& create_info) noexcept
{
    this->_ici.flags = create_info.flags;
    this->_ici.imageType = create_info.imageType;
    this->_ici.format = create_info.format;
    this->_ici.extent = create_info.extent;
    this->_ici.queueFamilyIndexCount = create_info.queueFamilyIndexCount;
    this->_ici.pQueueFamilyIndices = create_info.pQueueFamilyIndices;
    this->set_image_array_layers(create_info.arrayLayers);
}

void vka::Texture::set_sampler_mag_filter(VkFilter mag_filter) noexcept
{
    this->_sci.magFilter = mag_filter;
}
void vka::Texture::set_sampler_min_filter(VkFilter min_filter) noexcept
{
    this->_sci.minFilter = min_filter;
}
void vka::Texture::set_sampler_mipmap_mode(VkSamplerMipmapMode mode) noexcept
{
    this->_sci.mipmapMode = mode;
}
void vka::Texture::set_sampler_address_mode(VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w) noexcept
{
    this->_sci.addressModeU = u;
    this->_sci.addressModeV = v;
    this->_sci.addressModeW = w;
}
void vka::Texture::set_sampler_mip_lod_bias(float bias) noexcept
{
    this->_sci.mipLodBias = bias;
}
void vka::Texture::set_sampler_anisotropy_enable(bool enable) noexcept
{
    this->_sci.anisotropyEnable = enable ? VK_TRUE : VK_FALSE;
}
void vka::Texture::set_sampler_max_anisotropy(float max) noexcept
{
    this->_sci.maxAnisotropy = max;
}
void vka::Texture::set_sampler_compare_enable(bool enable) noexcept
{
    this->_sci.compareEnable = enable ? VK_TRUE : VK_FALSE;
}
void vka::Texture::set_sampler_compare_op(VkCompareOp op) noexcept
{
    this->_sci.compareOp = op;
}
void vka::Texture::set_sampler_lod(float min_lod, float max_lod) noexcept
{
    this->_sci.minLod = min_lod;
    this->_sci.maxLod = max_lod;
}
void vka::Texture::set_sampler_border_color(VkBorderColor border_color) noexcept
{
    this->_sci.borderColor = border_color;
}
void vka::Texture::set_sampler_unnormalized_coordinates(bool unnormalized) noexcept
{
    this->_sci.unnormalizedCoordinates = unnormalized ? VK_TRUE : VK_FALSE;
}
void vka::Texture::set_sampler_create_info(const VkSamplerCreateInfo& create_info) noexcept
{
    this->_sci.magFilter = create_info.magFilter;
    this->_sci.minFilter = create_info.minFilter;
    this->_sci.mipmapMode = create_info.mipmapMode;
    this->_sci.addressModeU = create_info.addressModeU;
    this->_sci.addressModeV = create_info.addressModeV;
    this->_sci.addressModeW = create_info.addressModeW;
    this->_sci.mipLodBias = create_info.mipLodBias;
    this->_sci.anisotropyEnable = create_info.anisotropyEnable;
    this->_sci.maxAnisotropy = create_info.maxAnisotropy;
    this->_sci.compareEnable = create_info.compareEnable;
    this->_sci.compareOp = create_info.compareOp;
    this->_sci.minLod = create_info.minLod;
    this->_sci.maxLod = create_info.maxLod;
    this->_sci.borderColor = create_info.borderColor;
    this->_sci.unnormalizedCoordinates = create_info.unnormalizedCoordinates;
}

void vka::Texture::set_pyhsical_device(VkPhysicalDevice physical_device) noexcept
{
    this->_pyhsical_device = physical_device;
}
void vka::Texture::set_device(VkDevice device) noexcept
{
    this->_device = device;
}
void vka::Texture::set_command_pool(VkCommandPool command_pool) noexcept
{
    this->_pool = command_pool;
}
void vka::Texture::set_queue(VkQueue queue) noexcept
{
    this->_queue = queue;
}

const VkImageCreateInfo& vka::Texture::get_image_create_info(void) const noexcept
{
    return this->_backup_ici;
}
const VkSamplerCreateInfo& vka::Texture::get_sampler_create_info(void) const noexcept
{
    return this->_backup_sci;
}
const std::vector<VkImageViewCreateInfo>& vka::Texture::get_view_create_infos(void) const noexcept
{
    return this->_vcis;
}
VkImage vka::Texture::image(void) const noexcept
{
    return this->_image;
}
VkSampler vka::Texture::sampler(void) const noexcept
{
    return this->_sampler;
}
const std::vector<VkImageView>& vka::Texture::views(void) const noexcept
{
    return this->_views;
}
VkExtent3D vka::Texture::extent(void) const noexcept
{
    return this->_backup_ici.extent;
}
uint32_t vka::Texture::count(void) const noexcept
{
    return this->_backup_ici.extent.width * this->_backup_ici.extent.height * this->_backup_ici.extent.depth;
}
size_t vka::Texture::mem_size(void) const noexcept
{
    return this->_mem_size;
}
uint32_t vka::Texture::mip_levels(void) const noexcept
{
    return this->_backup_ici.mipLevels;
}
uint32_t vka::Texture::array_layers(void) const noexcept
{
    return this->_backup_ici.arrayLayers;
}
bool vka::Texture::is_created(void) const noexcept
{
    return (this->_mem_size > 0);
}

void vka::Texture::add_view(const VkImageViewCreateInfo& ci)
{
    if(this->is_created())
        throw std::runtime_error("[vka::Texture::add_view]: Unable to add image view after texture was created.");

    this->_vcis.push_back(ci);
}

VkResult vka::Texture::load(uint32_t array_layer, const void* data)
{
    if(this->is_created())
        throw std::runtime_error("[vka::Texture::load]: Unable to load data after the texture was created.");
    if(array_layer >= this->_ici.arrayLayers)
        throw std::out_of_range("[vka::Texture::load]: Parameter array_layer must be smaller than the number of array layers specified in the image.");

    const size_t buff_size = this->_ici.extent.width * this->_ici.extent.height * this->_ici.extent.depth * vka::utility::format_sizeof(this->_ici.format);
    Buffer& tmp = this->_staging_buffers.at(array_layer);

    // if buffer has already been created, clear and recreate it
    if(tmp.is_created()) tmp.clear();

    tmp.set_physical_device(this->_pyhsical_device);
    tmp.set_device(this->_device);
    tmp.set_create_flags(0);
    tmp.set_create_usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    tmp.set_create_size(buff_size);
    tmp.set_create_sharing_mode(VK_SHARING_MODE_EXCLUSIVE);
    tmp.set_create_queue_families(this->_ici.pQueueFamilyIndices, 1);
    tmp.set_memory_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    VkResult result = tmp.create();
    if(result != VK_SUCCESS) return result;

    void* map = tmp.map(tmp.size(), 0);
    if(data == nullptr) memset(map, 0, tmp.size());
    else                memcpy(map, data, tmp.size());
    tmp.unmap();

    return VK_SUCCESS;
}

VkResult vka::Texture::create(bool mipgen, VkFilter mip_filter)
{
    if(this->is_created())
        throw std::runtime_error("[vka::Texture::create]: Texture has already been created, use .clear() to recreate the texture.");
    if(this->_pyhsical_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Texture::create]: Physical device of texture is a VK_NULL_HANDLE!");
    if(this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Texture::create]: Device of texture is a VK_NULL_HANDLE!");
    if(this->_pool == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Texture::create]: Command pool of texture is a VK_NULL_HANDLE!");
    if(this->_queue == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Texture::create]: Queue of texture is a VK_NULL_HANDLE!");
    if(!this->validate_buffers())
        throw std::runtime_error("[vka::Texture::create]: The size of loaded array layers are invalid. This could be caused by: "
                                 "(1) changing the image's extent after loading an array layer, "
                                 "(2) changing the image's format after loading an array layer or by "
                                 "(3) not initializing one or multiple array layers. "
                                 "In order to create a texture ALL array layers MUST have the same size and MUST be initialized.");
    if(!this->validate_views())
        throw std::runtime_error("[vka::Texture::create]: (Base-array-layer) + (layer-count) of an image view is greater than the "
                                 "number of array layers specified in the image create info.");
    if(mip_filter == VK_FILTER_LINEAR && !vka::utility::supports_format_feature2(this->_pyhsical_device, this->_ici.format, this->_ici.tiling, VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
        throw std::invalid_argument("[vka::Texture::create]: Linear filtering for mip maps (VK_FILTER_LINEAR) is not supproted. "
                                    "VkFormatProperties does not support the format feature VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT.");
    if(mip_filter == VK_FILTER_CUBIC_IMG && !vka::utility::supports_format_feature2(this->_pyhsical_device, this->_ici.format, this->_ici.tiling, VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG))
        throw std::invalid_argument("[vka::Texture::create]: Cubic filtering for mip maps (VK_FILTER_CUBINC_IMG or VK_FILTER_CUBIC_EXT) is not supported. "
                                    "VkFormatProperties does not support the format feature VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG or "
                                    "VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_EXT.");
    this->_backup_ici = this->_ici;
    this->_backup_sci = this->_sci;

    // some values
    const VkExtent3D& extent = this->_backup_ici.extent;
    const uint32_t layer_count = this->_backup_ici.arrayLayers;
    uint32_t level_count = (mipgen) ? this->to_mip_levels(extent) : 1;
    this->_backup_ici.mipLevels = level_count;

    // create image
    VkResult result = vkCreateImage(this->_device, &this->_backup_ici, nullptr, &this->_image);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // allocate memory for image
    VkMemoryRequirements mem_req = {};
    vkGetImageMemoryRequirements(this->_device, this->_image, &mem_req);
    this->_mem_size = mem_req.size;

    VkMemoryAllocateInfo mem_ai = {};
    mem_ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_ai.pNext = nullptr;
    mem_ai.allocationSize = this->_mem_size;
    mem_ai.memoryTypeIndex = utility::find_memory_type_index(this->_pyhsical_device, mem_req.memoryTypeBits, this->_properties);

    result = vkAllocateMemory(this->_device, &mem_ai, nullptr, &this->_memory);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // bind memory to image
    result = vkBindImageMemory(this->_device, this->_image, this->_memory, 0);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // staging buffer -> image array layers copy infos
    VkBufferImageCopy copies[layer_count];
    for(uint32_t i = 0; i < layer_count; i++)
    {
        copies[i] = {};
        copies[i].bufferOffset = 0;
        copies[i].bufferRowLength = 0;
        copies[i].bufferImageHeight = 0;
        copies[i].imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copies[i].imageSubresource.mipLevel = 0; // the source image data corresponds to mip level 0, all other levels are generated afterwards
        copies[i].imageSubresource.baseArrayLayer = i;
        copies[i].imageSubresource.layerCount = 1;  // copy layer by layer
        copies[i].imageOffset = {0, 0, 0};
        copies[i].imageExtent = extent;
    }

    // image division infos
    uint32_t div_count = level_count - 1;
    VkImageBlit divisions[div_count];
    VkOffset3D div_extent;
    div_extent.x = static_cast<int32_t>(extent.width);
    div_extent.y = static_cast<int32_t>(extent.height);
    div_extent.z = static_cast<int32_t>(extent.depth);

    for(uint32_t i = 0; i < div_count; i++)
    {
        divisions[i] = {};
        divisions[i].srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        divisions[i].srcSubresource.mipLevel = i;
        divisions[i].srcSubresource.baseArrayLayer = 0;
        divisions[i].srcSubresource.layerCount = layer_count;
        divisions[i].srcOffsets[0] = { 0, 0, 0 };
        divisions[i].srcOffsets[1] = div_extent;

        const int32_t tmp_x = div_extent.x >>= 1;
        const int32_t tmp_y = div_extent.y >>= 1;
        const int32_t tmp_z = div_extent.z >>= 1;

        div_extent.x = (tmp_x < 1) ? 1 : tmp_x;
        div_extent.y = (tmp_y < 1) ? 1 : tmp_y;
        div_extent.z = (tmp_z < 1) ? 1 : tmp_z;

        divisions[i].dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        divisions[i].dstSubresource.mipLevel = i + 1;
        divisions[i].dstSubresource.baseArrayLayer = 0;
        divisions[i].dstSubresource.layerCount = layer_count;
        divisions[i].dstOffsets[0] = { 0, 0, 0 };
        divisions[i].dstOffsets[1] = div_extent;
    }

    // pipeline barrier to transform the image layout to copy data into it
    VkImageMemoryBarrier copy_barrier = {};
    copy_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    copy_barrier.pNext = nullptr;
    copy_barrier.srcAccessMask = VK_ACCESS_NONE_KHR;
    copy_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    copy_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    copy_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    copy_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    copy_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    copy_barrier.image = this->_image;
    copy_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy_barrier.subresourceRange.baseMipLevel = 0;
    copy_barrier.subresourceRange.levelCount = 1;   // we only care about the first layout, as this is our target one
    copy_barrier.subresourceRange.baseArrayLayer = 0;
    copy_barrier.subresourceRange.layerCount = layer_count; // we want to copy into all array layers

    // pipeline barriers for blit operation
    VkImageMemoryBarrier div_src_barrier[div_count], div_dst_barrier[div_count];
    for(uint32_t i = 0; i < div_count; i++)
    {
        div_src_barrier[i] = {};
        div_src_barrier[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        div_src_barrier[i].pNext = nullptr;
        div_src_barrier[i].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        div_src_barrier[i].dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        div_src_barrier[i].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        div_src_barrier[i].newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        div_src_barrier[i].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        div_src_barrier[i].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        div_src_barrier[i].image = this->_image;
        div_src_barrier[i].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        div_src_barrier[i].subresourceRange.baseMipLevel = i;
        div_src_barrier[i].subresourceRange.levelCount = 1;
        div_src_barrier[i].subresourceRange.baseArrayLayer = 0;
        div_src_barrier[i].subresourceRange.layerCount = layer_count; // we want to divide all array layers

        div_dst_barrier[i] = {};
        div_dst_barrier[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        div_dst_barrier[i].pNext = nullptr;
        div_dst_barrier[i].srcAccessMask = VK_ACCESS_NONE_KHR;
        div_dst_barrier[i].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        div_dst_barrier[i].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        div_dst_barrier[i].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        div_dst_barrier[i].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        div_dst_barrier[i].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        div_dst_barrier[i].image = this->_image;
        div_dst_barrier[i].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        div_dst_barrier[i].subresourceRange.baseMipLevel = i + 1;
        div_dst_barrier[i].subresourceRange.levelCount = 1;
        div_dst_barrier[i].subresourceRange.baseArrayLayer = 0;
        div_dst_barrier[i].subresourceRange.layerCount = layer_count; // we want to divide all array layers
    }

    // final memory barriers to ensure that all blit transfers have finished, before any shader can start sampling
    VkImageMemoryBarrier final_barrier[2];
    for(uint32_t i = 0; i < 2; i++)
    {
        final_barrier[i] = {};
        final_barrier[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        final_barrier[i].pNext = nullptr;
        final_barrier[i].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        final_barrier[i].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        final_barrier[i].image = this->_image;
        final_barrier[i].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        final_barrier[i].subresourceRange.baseArrayLayer = 0;
        final_barrier[i].subresourceRange.layerCount = layer_count;
    }

    final_barrier[0].srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    final_barrier[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    final_barrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    final_barrier[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    final_barrier[0].subresourceRange.baseMipLevel = 0;
    final_barrier[0].subresourceRange.levelCount = level_count - 1;

    final_barrier[1].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    final_barrier[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    final_barrier[1].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    final_barrier[1].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    final_barrier[1].subresourceRange.baseMipLevel = level_count - 1;
    final_barrier[1].subresourceRange.levelCount = 1;

    // allocate command buffer
    VkCommandBufferAllocateInfo cbo_ai = {};
    cbo_ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cbo_ai.pNext = nullptr;
    cbo_ai.commandPool = this->_pool;
    cbo_ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cbo_ai.commandBufferCount = 1;

    detail::utility::CommandBuffer cbo(this->_device, cbo_ai);
    result = cbo.result();
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // start command buffer recording
    VkCommandBufferBeginInfo bi = {};
    bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bi.pNext = nullptr;
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    bi.pInheritanceInfo = nullptr;

    result = vkBeginCommandBuffer(cbo, &bi);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // execute image copies
    vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &copy_barrier);
    for(uint32_t i = 0; i < layer_count; i++)
        vkCmdCopyBufferToImage(cbo, this->_staging_buffers.at(i).handle(), this->_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, copies + i);
    
    // execute mip map creation
    for(uint32_t i = 0; i < div_count; i++)
    {
        vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, div_src_barrier + i );
        vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, div_dst_barrier + i);
        vkCmdBlitImage(cbo, this->_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, this->_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, divisions + i, mip_filter);
    }

    // final pipeline barriers to ensure synchronitzation between mip map creation and shader access
    if(level_count > 1)
        vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV, 0, 0, nullptr, 0, nullptr, 2, final_barrier);
    else
        vkCmdPipelineBarrier(cbo, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_NV, 0, 0, nullptr, 0, nullptr, 1, final_barrier + 1);

    result = vkEndCommandBuffer(cbo);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // submit command buffer to queue
    VkSubmitInfo si = {};
    si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    si.pNext = nullptr;
    si.waitSemaphoreCount = 0;
    si.pWaitSemaphores = nullptr;
    si.pWaitDstStageMask = nullptr;
    si.commandBufferCount = 1;
    si.pCommandBuffers = &cbo;
    si.signalSemaphoreCount = 0;
    si.pSignalSemaphores = nullptr;

    result = vkQueueSubmit(this->_queue, 1, &si, VK_NULL_HANDLE);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // wait for commands to be processed before the view and sampler can be created
    result = vkQueueWaitIdle(this->_queue);
    if(result != VK_SUCCESS) { this->clear(); return result; }


    for(VkImageViewCreateInfo& vci : this->_vcis)
    {
        VkImageView v;
        vci.image = this->_image;
        vci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        vci.subresourceRange.baseMipLevel = 0;
        vci.subresourceRange.levelCount = level_count;
        result = vkCreateImageView(this->_device, &vci, nullptr, &v);
        if(result != VK_SUCCESS) { this->clear(); return result; }

        this->_views.push_back(v);
    }

    result = vkCreateSampler(this->_device, &this->_backup_sci, nullptr, &this->_sampler);
    if(result != VK_SUCCESS) { this->clear(); }

    return result;
}

void vka::Texture::clear(void)
{
    if((this->_memory != VK_NULL_HANDLE || this->_image != VK_NULL_HANDLE || this->_sampler != VK_NULL_HANDLE || this->_views.size() > 0) && this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Texture::clear]: Device of image is a VK_NULL_HANDLE!");
    
    for(const VkImageView& v : this->_views)
        vkDestroyImageView(this->_device, v, nullptr);
    for(Buffer& b : this->_staging_buffers)
        b.clear();

    if(this->_sampler != VK_NULL_HANDLE) vkDestroySampler(this->_device, this->_sampler, nullptr);
    if(this->_memory != VK_NULL_HANDLE) vkFreeMemory(this->_device, this->_memory, nullptr);
    if(this->_image != VK_NULL_HANDLE) vkDestroyImage(this->_device, this->_image, nullptr);

    this->_sampler = VK_NULL_HANDLE;
    this->_memory = VK_NULL_HANDLE;
    this->_image = VK_NULL_HANDLE;
    this->_views.clear();

    this->init_ici(this->_backup_ici);
    this->init_sci(this->_backup_sci);
    this->_vcis.clear();
    this->_mem_size = 0;
}

bool vka::Texture::validate_buffers(void) const noexcept
{
    const size_t req_size = this->_ici.extent.width * this->_ici.extent.height * this->_ici.extent.depth * vka::utility::format_sizeof(this->_ici.format);
    
    for(const vka::Buffer& b : this->_staging_buffers)
    {
        if(b.size() != req_size)
            return false;
    }
    
    return true;
}

bool vka::Texture::validate_views(void) const noexcept
{
    const uint32_t max_layer_index = this->_ici.arrayLayers;

    for(const VkImageViewCreateInfo& vci : this->_vcis)
    {
        if((vci.subresourceRange.baseArrayLayer + vci.subresourceRange.layerCount) > max_layer_index)
            return false;
    }

    return true;
}

uint32_t vka::Texture::to_mip_levels(const VkExtent3D& extent)
{
    std::initializer_list<uint32_t> list = { extent.width, extent.height, extent.depth };
    return static_cast<uint32_t>(std::floor(std::log2(std::max(list)))) + 1;
}
