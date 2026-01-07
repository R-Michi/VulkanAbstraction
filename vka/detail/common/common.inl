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

constexpr void* vka::detail::common::add_vp(void* p, uintptr_t x) noexcept
{
    return static_cast<char*>(p) + x;
}

constexpr const void* vka::detail::common::add_cvp(const void* p, uintptr_t x) noexcept
{
    return static_cast<const char*>(p) + x;
}

constexpr bool vka::detail::common::cmpeq_extent(VkExtent2D a, VkExtent2D b) noexcept
{
    return a.width == b.width && a.height == b.height;
}

constexpr bool vka::detail::common::cmpeq_extent(VkExtent3D a, VkExtent3D b) noexcept
{
    return a.width == b.width && a.height == b.height && a.depth == b.depth;
}

inline uint32_t vka::detail::common::ilog2(uint32_t x) noexcept
{
#ifdef VKA_X86
    unsigned long y;    // BSF instruction finds the last '1' bit, and its position is the log2(x) of an integer value.
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

inline uint32_t vka::detail::common::max_ilog2(VkExtent3D extent) noexcept
{
    // find maximum of the 3 struct-components
    uint32_t m = extent.width > extent.height ? extent.width : extent.height;
    m = extent.depth > m ? extent.depth : m;
    return ilog2(m); // a > b -> log(a) > log(b)
}
