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

inline void vka::AttachmentImage::destroy_handles(void) noexcept
{
    if (this->m_view != VK_NULL_HANDLE)
        vkDestroyImageView(this->device, this->m_view, nullptr);
    if (this->memory != VK_NULL_HANDLE)
        vkFreeMemory(this->device, this->memory, nullptr);
    if (this->m_image != VK_NULL_HANDLE)
        vkDestroyImage(this->device, this->m_image, nullptr);
}

inline VkExtent2D vka::AttachmentImage::size(void) const noexcept
{
    return this->extent;
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

inline void vka::AttachmentImage::validate(VkPhysicalDevice pdevice, const AttachmentImageCreateInfo& create_info)
{
    if (this->device == VK_NULL_HANDLE) [[unlikely]]
        detail::error::throw_invalid_argument("[vka::AttachmentImage::create]: Device is a VK_NULL_HANDLE.");
    if (!utility::supports_format_feature2(pdevice, create_info.imageFormat, VK_IMAGE_TILING_OPTIMAL, utility::cvt_iu2ff(create_info.imageUsage))) [[unlikely]]
        detail::error::throw_invalid_argument("[vka::AttachmentImage::create]: Image format is not supported.");
}
