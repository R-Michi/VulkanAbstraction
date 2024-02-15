/**
* @file     vka_attachment_impl.inl
* @brief    Attachment image implementation file of inline functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline vka::AttachmentImage::AttachmentImage(void) noexcept :
    m_device(VK_NULL_HANDLE),
    m_memory(VK_NULL_HANDLE),
    m_image(VK_NULL_HANDLE),
    m_view(VK_NULL_HANDLE),
    m_extent({0, 0})
{}

inline vka::AttachmentImage::AttachmentImage(VkDevice device, VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info) :
    m_device(device),
    m_memory(VK_NULL_HANDLE),
    m_image(VK_NULL_HANDLE),
    m_view(VK_NULL_HANDLE),
    m_extent(create_info.imageExtent)
{
    this->internal_create(pdevice, properties, create_info);
}

inline vka::AttachmentImage::AttachmentImage(AttachmentImage&& src) noexcept :
    m_device(src.m_device),
    m_memory(src.m_memory),
    m_image(src.m_image),
    m_view(src.m_view),
    m_extent(src.m_extent)
{
    src.m_memory = VK_NULL_HANDLE;
    src.m_image = VK_NULL_HANDLE;
    src.m_view = VK_NULL_HANDLE;
    src.m_extent = { 0, 0 };
}

inline vka::AttachmentImage& vka::AttachmentImage::operator= (AttachmentImage&& src) noexcept
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

inline vka::AttachmentImage::~AttachmentImage(void)
{
    this->destroy_handles();
}

inline void vka::AttachmentImage::create(VkDevice device, VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info)
{
    if (!this->is_valid())
    {
        this->m_device = device;
        this->m_extent = create_info.imageExtent;
        this->internal_create(pdevice, properties, create_info);
    }
}

inline void vka::AttachmentImage::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_memory = VK_NULL_HANDLE;
    this->m_image = VK_NULL_HANDLE;
    this->m_view = VK_NULL_HANDLE;
    this->m_extent = { 0, 0 };
}

VkExtent2D vka::AttachmentImage::size(void) const noexcept
{
    return this->m_extent;
}

inline VkImage vka::AttachmentImage::handle(void) const noexcept
{
    return this->m_image;
}

inline VkImageView vka::AttachmentImage::view(void) const noexcept
{
    return this->m_view;
}

inline bool vka::AttachmentImage::is_valid(void) const noexcept
{
    // the view is the last handle created
    return (this->m_view != VK_NULL_HANDLE);
}
