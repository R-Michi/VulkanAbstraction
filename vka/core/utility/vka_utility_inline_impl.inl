/**
* @file     vka_utility_impl.h
* @brief    Implemenation of inline utility functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

template<bool Safe>
inline vka::utility::CommandBufferGuard<Safe>::CommandBufferGuard(VkDevice device, VkCommandPool pool, VkCommandBuffer cbo) noexcept
    : device(device), pool(pool), cbo(cbo)
{}

template<bool Safe>
inline vka::utility::CommandBufferGuard<Safe>::~CommandBufferGuard(void)
{
    if constexpr (Safe)
    {
        vkFreeCommandBuffers(this->device, this->pool, 1, &cbo);
    }
    else
    {
        if (this->cbo != VK_NULL_HANDLE)
            vkFreeCommandBuffers(this->device, this->pool, 1, &cbo);
    }
}

constexpr void vka::utility::get_color_formats(VkFormat* formats) noexcept
{
    for (uint32_t i = 1; i <= 123; i++)
        formats[i] = static_cast<VkFormat>(i);
}

constexpr void vka::utility::get_depth_formats(VkFormat* formats) noexcept
{
    formats[0] = VK_FORMAT_D16_UNORM;
    formats[1] = VK_FORMAT_X8_D24_UNORM_PACK32;
    formats[2] = VK_FORMAT_D32_SFLOAT;
    formats[3] = VK_FORMAT_D16_UNORM_S8_UINT;
    formats[4] = VK_FORMAT_D24_UNORM_S8_UINT;
    formats[5] = VK_FORMAT_D32_SFLOAT_S8_UINT;
}

constexpr void vka::utility::get_stencil_formats(VkFormat* formats) noexcept
{
    formats[0] = VK_FORMAT_S8_UINT;
    formats[1] = VK_FORMAT_D16_UNORM_S8_UINT;
    formats[2] = VK_FORMAT_D24_UNORM_S8_UINT;
    formats[3] = VK_FORMAT_D32_SFLOAT_S8_UINT;
}

constexpr void vka::utility::get_depth_stencil_formats(VkFormat* formats) noexcept
{
    formats[0] = VK_FORMAT_D16_UNORM_S8_UINT;
    formats[1] = VK_FORMAT_D24_UNORM_S8_UINT;
    formats[2] = VK_FORMAT_D32_SFLOAT_S8_UINT;
}

constexpr VkImageUsageFlags vka::utility::cvt_ff2iu(VkFormatFeatureFlags format_feature) noexcept
{
    constexpr size_t n_bits = 8 * sizeof(VkFlags);

    VkImageUsageFlags flags = 0;
    for (size_t i = 0; i < n_bits; i++)
        flags |= detail::utility::ff2iu_bit(static_cast<VkFormatFeatureFlagBits>(format_feature & (1 << i)));
    return flags;
}

constexpr VkFormatFeatureFlags vka::utility::cvt_iu2ff(VkImageUsageFlags image_usage) noexcept
{
    constexpr size_t n_bits = 8 * sizeof(VkFlags);

    VkFormatFeatureFlags flags = 0;
    for (size_t i = 0; i < n_bits; i++)
        flags |= detail::utility::iu2ff_bit(static_cast<VkImageUsageFlagBits>(image_usage & (1 << i)));
    return flags;
}
