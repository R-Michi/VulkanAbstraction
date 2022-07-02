/**
* @file     vka_attachment_impl.inl
* @brief    Attachment image implementation file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

vka::AttachmentImage::AttachmentImage(void) noexcept : AttachmentImage(VK_NULL_HANDLE, VK_NULL_HANDLE) {}

vka::AttachmentImage::AttachmentImage(VkPhysicalDevice physical_device, VkDevice device) noexcept
{
    this->init_ici(this->_ici);
    this->init_ici(this->_backup_ici);
    this->init_vci(this->_vci);
    this->init_vci(this->_backup_vci);

    this->_physical_device = physical_device;
    this->_device = device;

    this->_image = VK_NULL_HANDLE;
    this->_memory = VK_NULL_HANDLE;
    this->_view = VK_NULL_HANDLE;

    this->_mem_size = 0;
    this->_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

vka::AttachmentImage::~AttachmentImage(void)
{
    this->clear();
}

vka::AttachmentImage::AttachmentImage(AttachmentImage&& src) : AttachmentImage()
{
    *this = std::move(src);
}

vka::AttachmentImage& vka::AttachmentImage::operator= (AttachmentImage&& src)
{
    this->clear();

    this->_ici = src._ici;
    this->_vci = src._vci;
    this->_backup_ici = src._backup_ici;
    this->_backup_vci = src._backup_vci;

    this->_physical_device = src._physical_device;
    this->_device = src._device;

    this->_mem_size = src._mem_size;
    this->_properties = src._properties;

    this->_image = src._image;
    this->_memory = src._memory;
    this->_view = src._view;

    src._image = VK_NULL_HANDLE;
    src._memory = VK_NULL_HANDLE;
    src._view = VK_NULL_HANDLE;
    src.clear();
    return *this;
}

void vka::AttachmentImage::init_ici(VkImageCreateInfo& ci)
{
    ci = {};
    ci.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ci.pNext = nullptr;
    ci.flags = 0;
    ci.imageType = VK_IMAGE_TYPE_2D;
    ci.mipLevels = 1;
    ci.arrayLayers = 1;
    ci.tiling = VK_IMAGE_TILING_OPTIMAL;
    ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
}

void vka::AttachmentImage::init_vci(VkImageViewCreateInfo& ci)
{
    ci = {};
    ci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ci.pNext = nullptr;
    ci.flags = 0;
    ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
    ci.subresourceRange.levelCount = 1;
    ci.subresourceRange.baseMipLevel = 0;
    ci.subresourceRange.layerCount = 1;
    ci.subresourceRange.baseArrayLayer = 0;
}

void vka::AttachmentImage::set_image_format(VkFormat format) noexcept
{
    this->_ici.format = format;
}

void vka::AttachmentImage::set_image_extent(VkExtent2D extent) noexcept
{
    this->set_image_extent(extent.width, extent.height);
}

void vka::AttachmentImage::set_image_extent(uint32_t w, uint32_t h) noexcept
{
    this->_ici.extent.width = w;
    this->_ici.extent.height = h;
    this->_ici.extent.depth = 1;
}

void vka::AttachmentImage::set_image_samples(VkSampleCountFlagBits samples) noexcept
{
    this->_ici.samples = samples;
}

void vka::AttachmentImage::set_image_usage(VkImageUsageFlags usage) noexcept
{
    this->_ici.usage = usage;
}

void vka::AttachmentImage::set_image_queue_family_index(uint32_t index) noexcept
{
    this->_ici.queueFamilyIndexCount = 1;
    this->_ici.pQueueFamilyIndices = &index;
}

void vka::AttachmentImage::set_view_format(VkFormat format) noexcept
{
    this->_vci.format = format;
}

void vka::AttachmentImage::set_view_components(VkComponentMapping component_mapping) noexcept
{
    this->_vci.components = component_mapping;
}

void vka::AttachmentImage::set_view_aspect_mask(VkImageAspectFlags aspect_mask) noexcept
{
    this->_vci.subresourceRange.aspectMask = aspect_mask;
}

// set internal requiered handles
void vka::AttachmentImage::set_physical_device(VkPhysicalDevice physical_device) noexcept
{
    this->_physical_device = physical_device;
}

void vka::AttachmentImage::set_device(VkDevice device) noexcept
{
    this->_device = device;
}


VkResult vka::AttachmentImage::create(void)
{
    if(this->is_created())
        throw std::runtime_error("[vka::AttachmentImage::create]: Attachment image has already been created, use .clear() to recreate the attachment image.");
    if(this->_physical_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::AttachmentImage::create]: Physical device of image is a VK_NULL_HANDLE!");
    if(this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::AttachmentImage::create]: Device of image is a VK_NULL_HANDLE!");
    if(!vka::utility::is_format_feature_supported(this->_physical_device, this->_ici.format, this->_ici.tiling, vka::utility::image_usage_to_format_feature(this->_ici.usage)))
        throw std::invalid_argument("[vka::AttachmentImage::create]: Failed to create attachment image, format is not supported.");

    this->_backup_ici = this->_ici;
    this->_backup_vci = this->_vci;

    // create image
    VkResult result = vkCreateImage(this->_device, &this->_backup_ici, nullptr, &this->_image);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // query memory properties for memory allocation
    VkMemoryRequirements mem_req = {};
    vkGetImageMemoryRequirements(this->_device, this->_image, &mem_req);
    this->_mem_size = mem_req.size;

    // allocate memory for buffer
    VkMemoryAllocateInfo mem_ai = {};
    mem_ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_ai.pNext = nullptr;
    mem_ai.allocationSize = this->_mem_size;
    mem_ai.memoryTypeIndex = utility::find_memory_type_index(this->_physical_device, mem_req.memoryTypeBits, this->_properties);

    result = vkAllocateMemory(this->_device, &mem_ai, nullptr, &this->_memory);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // bind memory to image
    result = vkBindImageMemory(this->_device, this->_image, this->_memory, 0);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    // create image view
    this->_backup_vci.image = this->_image;
    result = vkCreateImageView(this->_device, &this->_backup_vci, nullptr, &this->_view);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    return VK_SUCCESS;
}

void vka::AttachmentImage::clear(void)
{
    if ((this->_view != VK_NULL_HANDLE || this->_memory != VK_NULL_HANDLE || this->_image != VK_NULL_HANDLE) && this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Image::clear]: Device of image is a VK_NULL_HANDLE!");

    if (this->_view != VK_NULL_HANDLE)      vkDestroyImageView(this->_device, this->_view, nullptr);
    if (this->_memory != VK_NULL_HANDLE)    vkFreeMemory(this->_device, this->_memory, nullptr);
    if (this->_image != VK_NULL_HANDLE)     vkDestroyImage(this->_device, this->_image, nullptr);

    this->_view = VK_NULL_HANDLE;
    this->_memory = VK_NULL_HANDLE;
    this->_image = VK_NULL_HANDLE;

    this->_mem_size = 0;
    this->init_ici(this->_backup_ici);
    this->init_vci(this->_backup_vci);
}

const VkImageCreateInfo& vka::AttachmentImage::get_image_create_info(void) const noexcept
{
    return this->_backup_ici;
}

const VkImageViewCreateInfo& vka::AttachmentImage::get_view_create_info(void) const noexcept
{
    return this->_backup_vci;
}

VkExtent2D vka::AttachmentImage::extent(void) const noexcept
{
    return { this->_backup_ici.extent.width, this->_backup_ici.extent.height };
}

uint32_t vka::AttachmentImage::count(void) const noexcept
{
    return this->_backup_ici.extent.width * this->_backup_ici.extent.height;
}

size_t vka::AttachmentImage::mem_size(void) const noexcept
{
    return this->_mem_size;
}

VkImage vka::AttachmentImage::image(void) const noexcept
{
    return this->_image;
}

VkImageView vka::AttachmentImage::view(void) const noexcept
{
    return this->_view;
}

bool vka::AttachmentImage::is_created(void) const noexcept
{
    return (this->_mem_size > 0);
}
