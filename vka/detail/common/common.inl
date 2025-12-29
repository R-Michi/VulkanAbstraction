/**
 * @brief Implementation details for the common functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#pragma once

#include "common.h"

constexpr VkImageUsageFlagBits vka::detail::common::ff2iu_bit(VkFormatFeatureFlagBits format_feature) noexcept
{
    switch (format_feature)
    {
    case VK_FORMAT_FEATURE_TRANSFER_SRC_BIT:
        return VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    case VK_FORMAT_FEATURE_TRANSFER_DST_BIT:
        return VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    case VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT:
        return VK_IMAGE_USAGE_SAMPLED_BIT;
    case VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT:
        return VK_IMAGE_USAGE_STORAGE_BIT;
    case VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT:
        return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    case VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT:
        return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    case VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR:
        return VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV;
    case VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT:
        return VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT;
    case VK_FORMAT_FEATURE_FLAG_BITS_MAX_ENUM:
        return VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM;
    default: break; // do nothing
    };
    return static_cast<VkImageUsageFlagBits>(0);
}

constexpr VkFormatFeatureFlagBits vka::detail::common::iu2ff_bit(VkImageUsageFlagBits image_usage) noexcept
{
    switch (image_usage)
    {
    case VK_IMAGE_USAGE_TRANSFER_SRC_BIT:
        return VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
    case VK_IMAGE_USAGE_TRANSFER_DST_BIT:
        return VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
    case VK_IMAGE_USAGE_SAMPLED_BIT:
        return VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
    case VK_IMAGE_USAGE_STORAGE_BIT:
        return VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
    case VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT:
        return VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
    case VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT:
        return VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    case VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV:
        return VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
    case VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT:
        return VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT;
    case VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM:
        return VK_FORMAT_FEATURE_FLAG_BITS_MAX_ENUM;
    default: break; // do nothing
    };
    return static_cast<VkFormatFeatureFlagBits>(0);
}

inline void* vka::detail::common::add_vp(void* p, uintptr_t x) noexcept
{
    return static_cast<char*>(p) + x;
}

inline const void* vka::detail::common::add_cvp(const void* p, uintptr_t x) noexcept
{
    return static_cast<const char*>(p) + x;
}
