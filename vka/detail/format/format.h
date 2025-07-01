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
    /**
     * @brief Calculates the loop-up table offset.
     * @param format Specifies the format to calculate the offset of.
     * @return Returns the offset in the look-up table.
     */
    constexpr size_t format_lut_offset(VkFormat format);
}

#include "format.inl"
