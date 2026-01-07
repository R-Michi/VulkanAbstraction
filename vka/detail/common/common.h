/**
 * @brief Includes implementation details for the common functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::detail::common
{
    /**
     * Converts a format-feature bit to an image-usage bit.
     * @param format_feature Specifies the format-feature bit.
     * @return Returns the corresponding image-usage bit.
     */
    constexpr VkImageUsageFlagBits ff2iu_bit(VkFormatFeatureFlagBits format_feature) noexcept;

    /**
     * Converts an image-usage bit to a format-feature bit.
     * @param image_usage Specifies the image-usage bit.
     * @return Returns the corresponding format-feature bit.
     */
    constexpr VkFormatFeatureFlagBits iu2ff_bit(VkImageUsageFlagBits image_usage) noexcept;

    /// Adds an offset to a void-pointer.
    constexpr void* add_vp(void* p, uintptr_t x) noexcept;

    /// Adds an offset to a const void-pointer.
    constexpr const void* add_cvp(const void* p, uintptr_t x) noexcept;

    /// Compares two VkExtent2D types for equality.
    constexpr bool cmpeq_extent(VkExtent2D a, VkExtent2D b) noexcept;

    /// Compares two VkExtent3D types for equality.
    constexpr bool cmpeq_extent(VkExtent3D a, VkExtent3D b) noexcept;

    /// Computes the integer base 2 logarithm.
    inline uint32_t ilog2(uint32_t x) noexcept;

    /// Computes max{ilog2(width), ilog2(height), ilog2(depth)}
    inline uint32_t max_ilog2(VkExtent3D extent) noexcept;
} // namespace vka::detail::common
