/**
 * @brief Includes implementation details for the format functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::detail::format
{
    /// Calculates the format loop-up table offset.
    constexpr uint32_t format_lut_offset(VkFormat format);

    /// @return Returns the byte-size of a vulkan format.
    constexpr size_t format_sizeof(VkFormat format) noexcept;

    /// @return Returns the byte size of a vulkan 1.0 format.
    constexpr size_t format_sizeof10(VkFormat format) noexcept;

    /// @return Reruns the component count of a vulkan format.
    constexpr size_t format_countof(VkFormat format) noexcept;

    /// @return Returns the component count of a vulkan 1.0 format.
    constexpr size_t format_countof10(VkFormat format) noexcept;
}
