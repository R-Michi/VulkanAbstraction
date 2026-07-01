/**
 * @brief Inline implementation for the attachment image class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "attachment.h"

constexpr vka::AttachmentImage::AttachmentImage() noexcept :
    m_extent{ 0, 0 }
{}

constexpr vka::AttachmentImage::operator bool() const noexcept
{
    return (bool)this->m_image;
}

constexpr VkExtent2D vka::AttachmentImage::size() const noexcept
{
    return this->m_extent;
}

constexpr VkDevice vka::AttachmentImage::parent() const noexcept
{
    return this->m_image.parent();
}

constexpr VkImage vka::AttachmentImage::handle() const noexcept
{
    return this->m_image.get().image;
}

constexpr VkImageView vka::AttachmentImage::view() const noexcept
{
    return this->m_image.get().view;
}

constexpr void vka::AttachmentImage::destroy() noexcept
{
    this->m_image = VK_NULL_HANDLE;
    this->m_extent = { 0, 0 };
}
