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

vka::AttachmentImage::AttachmentImage(VkDevice device) noexcept
    : m_device(device), m_memory(VK_NULL_HANDLE), m_image(VK_NULL_HANDLE), m_view(VK_NULL_HANDLE), m_extent({0, 0})
{}

vka::AttachmentImage::AttachmentImage(AttachmentImage&& src) noexcept
    : m_device(src.m_device), m_memory(src.m_memory), m_image(src.m_image), m_view(src.m_view), m_extent(src.m_extent)
{
    src.m_memory = VK_NULL_HANDLE;
    src.m_image = VK_NULL_HANDLE;
    src.m_view = VK_NULL_HANDLE;
    src.m_extent = { 0, 0 };
}

vka::AttachmentImage& vka::AttachmentImage::operator= (AttachmentImage&& src) noexcept
{
    // destroys the attachment image, if it has been created, otherwise this function does nothing
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_memory = src.m_memory;
    this->m_image = src.m_image;
    this->m_view = src.m_view;
    this->m_extent = src.m_extent;
    src.m_memory = VK_NULL_HANDLE;
    src.m_image = VK_NULL_HANDLE;
    src.m_view = VK_NULL_HANDLE;
    src.m_extent = { 0, 0 };
    return *this;
}

vka::AttachmentImage::~AttachmentImage(void)
{
    this->destroy_handles();
}

void vka::AttachmentImage::init(VkDevice device) noexcept
{
    if (!this->is_valid())
        this->m_device = device;
}

void vka::AttachmentImage::create(VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info)
{
    if (!this->is_valid())
    {
        this->validate(pdevice, create_info);
        this->m_extent = create_info.imageExtent;

        // create image
        const VkImageCreateInfo image_ci = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = create_info.imageFormat,
            .extent = { create_info.imageExtent.width, create_info.imageExtent.height, 1 },
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = create_info.imageSamples,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = create_info.imageUsage,
            .sharingMode = create_info.imageSharingMode,
            .queueFamilyIndexCount = create_info.imageQueueFamilyIndexCount,
            .pQueueFamilyIndices = create_info.imageQueueFamilyIndices,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };
        detail::error::check_result(vkCreateImage(this->m_device, &image_ci, nullptr, &this->m_image), IMAGE_CREATE_FAILED);

        // query memory requirements
        VkMemoryRequirements requirements;
        vkGetImageMemoryRequirements(this->m_device, this->m_image, &requirements);

        // allocate memory for the image
        const VkMemoryAllocateInfo memory_ai = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = requirements.size,
            .memoryTypeIndex = memory::find_type_index(properties, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
        };
        detail::error::check_result(vkAllocateMemory(this->m_device, &memory_ai, nullptr, &this->m_memory), ALLOC_MEMORY_FAILED);
        detail::error::check_result(vkBindImageMemory(this->m_device, this->m_image, this->m_memory, 0), BIND_MEMORY_FAILED);

        // create image view from image
        const VkImageViewCreateInfo view_ci = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = this->m_image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = create_info.viewFormat,
            .components = create_info.viewComponentMapping,
            .subresourceRange = {
                .aspectMask = create_info.viewAspectMask,
                .baseMipLevel = 0,
                .levelCount = 1,        // we don't use mip-mapping on attachment images
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
        detail::error::check_result(vkCreateImageView(this->m_device, &view_ci, nullptr, &this->m_view), VIEW_CREATE_FAILED);
    }
}

void vka::AttachmentImage::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_memory = VK_NULL_HANDLE;
    this->m_image = VK_NULL_HANDLE;
    this->m_view = VK_NULL_HANDLE;
    this->m_extent = { 0, 0 };
}
