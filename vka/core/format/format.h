/**
 * @brief Helper function for formats.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::format
{
    /**
     * Checks if format features are supported for a given image tiling.
     * @param properties Format properties that contains all supported features.
     * @param tiling Required image tiling.
     * @param format_feature Required format features.
     * @return Returns whether the format features are supported.
     */
    constexpr bool supports_feature(const VkFormatProperties& properties, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;

    /**
     * Checks if format features are supported for a given image tiling. The difference to <c>supports_feature</c> is
     * that this function also queries the format properties internally.
     * @param device Physical device.
     * @param format Required format.
     * @param tiling Required image tiling.
     * @param format_feature Required format features.
     * @return Returns whether the format features are supported.
     */
    bool supports_feature2(VkPhysicalDevice device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;

    /**
     * The standard color formats include all color formats that are available without enabling any extension.
     * More formally, returns all formats in the interval of [1, 123].
     * @return Returns all standard color formats.
     */
    consteval ColorFormatArray get_color() noexcept;

    /**
     * The standard depth formats include all depth formats that are available without enabling any extension.
     * More formally, returns all formats in the interval of [124, 130] excluding 127.
     * @return Returns all standard depth formats.
     */
    consteval DepthFormatArray get_depth() noexcept;

    /**
     * The standard stencil formats include all stencil formats that are available without enabling any
     * extension. More formally, returns all formats in the interval of [127, 130].
     * @return Returns all standard stencil formats.
     */
    consteval StencilFormatArray get_stencil() noexcept;

    /**
     * Depth-stencil defines all formats that are a combination of a depth and a stencil portion. The standard
     * depth-stencil formats include all depth-stencil formats that are available without enabling any extension. More
     * formally, returns all formats in the interval of [128, 130].
     * @return Returns all depth-stencil formats.
     */
    consteval DepthFormatArray get_depth_stencil() noexcept;

    /**
     * Queries all supported formats for a given image tiling and format feature.
     * @param physical_device Physical device.
     * @param tiling Required image tiling.
     * @param format_feature Required format features.
     * @return Returns all supported formats for a given image tiling and format feature.
     */
    std::vector<VkFormat> get_supported(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature);
}

namespace vka
{
    /// @return Returns size of a vulkan format in bytes.
    constexpr size_t format_sizeof(VkFormat format) noexcept;

    /// @return Returns size of a vulkan 1.0 format in bytes.
    constexpr size_t format_sizeof10(VkFormat format) noexcept;

    /// @return Returns the number of components of a vulkan format.
    constexpr size_t format_countof(VkFormat format) noexcept;

    /// @return Returns the number of components of a vulkan 1.0 format.
    constexpr size_t format_countof10(VkFormat format) noexcept;
}
