/**
* @file     vka_format.h
* @brief    Functionality regarding formats and format properties.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../detail/format/vka_format.h"

namespace vka::format
{
    /*
    * Checks if a format feature is supported for a given image tiling. The format properties
    * that contains all supported format features is specified by 'properties'. The requiered
    * format feature flags are specified by 'format_feature'. Returns true, if the required
    * format features are supported and false otherwise.
    */
    bool supports_feature(const VkFormatProperties& properties, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;

    /*
    * This function does the same as supports_format_feature(). Additionally, it queries the
    * format properties internally and requires a physical device and a format from which the
    * format properties are queried. The physical device is specified by 'device' and the
    * format is specified by 'format'.
    */
    bool supports_feature2(VkPhysicalDevice device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;

    /*
    * Returns all standard color formats, that are not extension formats nor compressed
    * formats. More formally returns all formats in the interval of
    * [static_cast<VkFormat>(1), static_cast<VkFormat>(123)]. The formats are stored in the
    * array specified by 'formats'. This array must have a length of at least 123 elements.
    */
    constexpr void get_color(VkFormat* formats) noexcept;

    /*
    * Returns all standard depth formats, that are not extension formats nor compressed
    * formats. More formally returns all formats in the interval of
    * [static_cast<VkFormat>(124), static_cast<VkFormat>(130)] \ static_cast<VkFormat>(127).
    * The formats are stored in the array specified by 'formats'. This array must have a length
    * of at least 6 elements.
    */
    constexpr void get_depth(VkFormat* formats) noexcept;

    /*
    * Returns all standard stencil formats, that are not extension formats nor compressed
    * formats. More formally returns all formats in the interval of
    * [static_cast<VkFormat>(127), static_cast<VkFormat>(130)]. The formats are stored in the
    * array specified by 'formats'. This array must have a length of at least 4 elements.
    */
    constexpr void get_stencil(VkFormat* formats) noexcept;

    /*
    * Returns all standard depth-stencil formats, that are not extension formats nor compressed
    * formats. More formally returns all formats in the interval of
    * [static_cast<VkFormat>(128), static_cast<VkFormat>(130)]. The formats are stored in the
    * array specified by 'formats'. This array must have a length of at least 3 elements.
    */
    constexpr void get_depth_stencil(VkFormat* formats) noexcept;

    /*
    * Returns all supported formats for a given image tiling and format feature. A physical
    * device is requiered and is given by 'device'. The image tiling is specified by 'tiling'
    * and the format feature is specified by 'format_feature'. The returned formats are stored
    * in the 'formats' vector.
    * NOTE: A std::vector is used because the number of formats is not known in advance.
    * To speed up the querying, memory can be reserved by the vector to minimize reallocation's.
    */
    void get_supported(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);
}

namespace vka
{
    /*
    * Converts a vulkan format to a size in bytes. The format to convert is specified by 'format'.
     * The size in bytes is returned.
    */
    constexpr size_t format_sizeof(VkFormat format) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_format_impl.inl"
#endif
#include "vka_format_inline_impl.inl"
