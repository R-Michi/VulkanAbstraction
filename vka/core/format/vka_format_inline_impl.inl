/**
* @file     vka_format_inline_impl.inl
* @brief    Implementation of format functionality which is inline.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

constexpr void vka::format::get_color(VkFormat* formats) noexcept
{
    for (uint32_t i = 1; i <= 123; i++)
        formats[i] = static_cast<VkFormat>(i);
}

constexpr void vka::format::get_depth(VkFormat* formats) noexcept
{
    formats[0] = VK_FORMAT_D16_UNORM;
    formats[1] = VK_FORMAT_X8_D24_UNORM_PACK32;
    formats[2] = VK_FORMAT_D32_SFLOAT;
    formats[3] = VK_FORMAT_D16_UNORM_S8_UINT;
    formats[4] = VK_FORMAT_D24_UNORM_S8_UINT;
    formats[5] = VK_FORMAT_D32_SFLOAT_S8_UINT;
}

constexpr void vka::format::get_stencil(VkFormat* formats) noexcept
{
    formats[0] = VK_FORMAT_S8_UINT;
    formats[1] = VK_FORMAT_D16_UNORM_S8_UINT;
    formats[2] = VK_FORMAT_D24_UNORM_S8_UINT;
    formats[3] = VK_FORMAT_D32_SFLOAT_S8_UINT;
}

constexpr void vka::format::get_depth_stencil(VkFormat* formats) noexcept
{
    formats[0] = VK_FORMAT_D16_UNORM_S8_UINT;
    formats[1] = VK_FORMAT_D24_UNORM_S8_UINT;
    formats[2] = VK_FORMAT_D32_SFLOAT_S8_UINT;
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

