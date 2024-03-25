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
    /**
     * @brief Checks, if a format feature is supported for a given image tiling and format.
     * @param properties Specifies the format properties that contain all supported format features of a format.
     * @param tiling Specifies the required image tiling.
     * @param format_feature Specifies the required format features.
     * @return Returns 'true', if the format features are supported and 'false' otherwise.
     */
    bool supports_feature(const VkFormatProperties& properties, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;

    /**
     * @brief Checks, if a format feature is supported for a given image tiling and format.
     * @param device Specifies a valid physical device.
     * @param format Specifies the required format
     * @param tiling Specifies the required image tiling.
     * @param format_feature Specifies the required format features.
     * @return Returns 'true', if the format features are supported and 'false' otherwise.
     * @details In addition to the supports_feature() function, this function itself queries the format properties.
     */
    bool supports_feature2(VkPhysicalDevice device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;

    /**
     * @brief Returns all standard color formats.
     * @param formats Specifies an array in which to store all standard color formats. This array must have a length of
     * at least 123 elements.
     * @details The standard color formats include all color formats that are available without enabling any extension.
     * More formally, returns all formats in the interval of [1, 123].
     */
    constexpr void get_color(VkFormat* formats) noexcept;

    /**
     * @brief Returns all standard depth formats.
     * @param formats Specifies an array in which to store all standard depth formats. This array must have a length of
     * at least 6 elements.
     * @details The standard depth formats include all depth formats that are available without enabling any extension.
     * More formally, returns all formats in the interval of [124, 130] \ 127.
     */
    constexpr void get_depth(VkFormat* formats) noexcept;

    /**
     * @brief Returns all standard stencil formats.
     * @param formats Specifies an array in which to store all standard stencil formats. This array must have a length
     * of at least 4 elements.
     * @details The standard stencil formats include all stencil formats that are available without enabling any
     * extension. More formally, returns all formats in the interval of [127, 130].
     */
    constexpr void get_stencil(VkFormat* formats) noexcept;

    /**
     * @brief Returns all depth-stencil formats.
     * @param formats Specifies an array in which to store all standard depth-stencil formats. This array must have a
     * length of at least 3 elements.
     * @details Depth-stencil defines all formats that are a combination of a depth and a stencil portion. The standard
     * depth-stencil formats include all depth-stencil formats that are available without enabling any extension. More
     * formally, returns all formats in the interval of [128, 130].
     */
    constexpr void get_depth_stencil(VkFormat* formats) noexcept;

    /**
     * @brief Returns all supported formats for a given image tiling and format feature.
     * @param physical_device Specifies a valid physical device.
     * @param tiling Specifies the required image tiling.
     * @param format_feature Specifies the required format features.
     * @param formats Specifies a vector in which to store all supported formats. The previous contents of the vector
     * are cleared.
     */
    void get_supported(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);
}

namespace vka
{
    /**
     * @param format Specifies a vulkan format.
     * @return Returns the byte-size of a vulkan format.
     */
    constexpr size_t format_sizeof(VkFormat format) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_format_impl.inl"
#endif
#include "vka_format_inline_impl.inl"
