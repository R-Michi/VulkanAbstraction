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
     * @brief Converts a format-feature bit to an image-usage bit.
     * @param format_feature Specifies the format-feature bit.
     * @return Returns the corresponding image-usage bit.
     */
    constexpr VkImageUsageFlagBits ff2iu_bit(VkFormatFeatureFlagBits format_feature) noexcept;

    /**
     * @brief Converts an image-usage bit to a format-feature bit.
     * @param image_usage Specifies the image-usage bit.
     * @return Returns the corresponding format-feature bit.
     */
    constexpr VkFormatFeatureFlagBits iu2ff_bit(VkImageUsageFlagBits image_usage) noexcept;

    /**
     * @brief Adds an offset to a void-pointer.
     * @param p Specifies the void-pointer.
     * @param x Specifies the offset.
     * @return Returns 'p + x'.
     */
    inline void* add_vp(void* p, uintptr_t x) noexcept;
} // namespace vka::detail::common
