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
     * @brief Checks if a format feature is supported for a given image tiling and format.
     * @param properties Specifies the format properties that contain all supported format features of a format.
     * @param tiling Specifies the required image tiling.
     * @param format_feature Specifies the required format features.
     * @return Returns 'true' if the format features are supported and 'false' otherwise.
     */
    inline bool supports_feature(const VkFormatProperties& properties, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;

    /**
     * @brief Checks if a format feature is supported for a given image tiling and format.
     * @param device Specifies a valid physical device.
     * @param format Specifies the required format
     * @param tiling Specifies the required image tiling.
     * @param format_feature Specifies the required format features.
     * @return Returns 'true' if the format features are supported and 'false' otherwise.
     * @details In addition to supports_feature(), this function queries the format properties.
     */
    bool supports_feature2(VkPhysicalDevice device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;

    /**
     * @return Returns all standard color formats.
     * @details The standard color formats include all color formats that are available without enabling any extension.
     * More formally, returns all formats in the interval of [1, 123].
     */
    consteval ColorFormatArray get_color() noexcept;

    /**
     * @return Returns all standard depth formats.
     * @details The standard depth formats include all depth formats that are available without enabling any extension.
     * More formally, returns all formats in the interval of [124, 130] excluding 127.
     */
    consteval DepthFormatArray get_depth() noexcept;

    /**
     * @return Returns all standard stencil formats.
     * @details The standard stencil formats include all stencil formats that are available without enabling any
     * extension. More formally, returns all formats in the interval of [127, 130].
     */
    consteval StencilFormatArray get_stencil() noexcept;

    /**
     * @return Returns all depth-stencil formats.
     * @details Depth-stencil defines all formats that are a combination of a depth and a stencil portion. The standard
     * depth-stencil formats include all depth-stencil formats that are available without enabling any extension. More
     * formally, returns all formats in the interval of [128, 130].
     */
    consteval DepthFormatArray get_depth_stencil() noexcept;

    /**
     * @param physical_device Specifies a valid physical device.
     * @param tiling Specifies the required image tiling.
     * @param format_feature Specifies the required format features.
     * @return Returns all supported formats for a given image tiling and format feature.
     */
    std::vector<VkFormat> get_supported(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature);
}

namespace vka
{
    /// @return Returns the byte-size of a vulkan format.
    constexpr size_t format_sizeof(VkFormat format) noexcept;

    /// @return Returns the byte size of a vulkan 1.0 format.
    constexpr size_t format_sizeof10(VkFormat format) noexcept;

    /// @return Reruns the component count of a vulkan format.
    constexpr size_t format_countof(VkFormat format) noexcept;

    /// @return Returns the component count of a vulkan 1.0 format.
    constexpr size_t format_countof10(VkFormat format) noexcept;
}
