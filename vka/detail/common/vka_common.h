/**
* @file     vka_utility.h
* @brief    Detailed declaration of common functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka::detail::common
{
    constexpr VkImageUsageFlagBits ff2iu_bit(VkFormatFeatureFlagBits format_feature) noexcept;
    constexpr VkFormatFeatureFlagBits iu2ff_bit(VkImageUsageFlagBits image_usage) noexcept;
    void init_format_sizeof(std::unordered_map<VkFormat, size_t>& f2s);
} // namespace vka::detail::common

#ifdef VKA_IMPLEMENTATION
    #include "vka_common_impl.inl"
#endif
#include "vka_common_inline_impl.inl"
