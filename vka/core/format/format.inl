/**
 * @brief Inline implementation for format functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "format.h"

inline bool vka::format::supports_feature(const VkFormatProperties& properties, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept
{
    // check if the format properties of the given format support the given format features
    if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & format_feature) == format_feature) return true;
    if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & format_feature) == format_feature) return true;
    return false;
}

consteval vka::ColorFormatArray vka::format::get_color() noexcept
{
    ColorFormatArray formats;
    for (uint32_t i = 0; i <= 123; ++i)
        formats[i] = static_cast<VkFormat>(i);
    return formats;
}

consteval vka::DepthFormatArray vka::format::get_depth() noexcept
{
    return {
        VK_FORMAT_D16_UNORM,
        VK_FORMAT_X8_D24_UNORM_PACK32,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D32_SFLOAT_S8_UINT
    };
}

consteval vka::StencilFormatArray vka::format::get_stencil() noexcept
{
    return {
        VK_FORMAT_S8_UINT,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D32_SFLOAT_S8_UINT
    };
}

consteval vka::DepthFormatArray vka::format::get_depth_stencil() noexcept
{
    return {
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_D32_SFLOAT_S8_UINT
    };
}

constexpr size_t vka::format_sizeof(VkFormat format) noexcept
{
    return detail::format::SIZE_LOOKUP[static_cast<size_t>(format) - detail::format::format_lut_offset(format)];
}

constexpr size_t vka::format_sizeof2(VkFormat format) noexcept
{
    const uint32_t i_format = static_cast<uint32_t>(format);
    return i_format <= 184 ? detail::format::SIZE_LOOKUP[i_format] : NSIZE;
}

constexpr size_t vka::format_countof(VkFormat format) noexcept
{
    return detail::format::COUNT_LOOKUP[static_cast<size_t>(format) - detail::format::format_lut_offset(format)];
}

constexpr size_t vka::format_countof2(VkFormat format) noexcept
{
    const uint32_t i_format = static_cast<uint32_t>(format);
    return i_format <= 184 ? detail::format::COUNT_LOOKUP[i_format] : NSIZE;
}

