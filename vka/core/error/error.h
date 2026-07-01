/**
 * @brief Helper functions for error handling and result checking.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
    /**
     * @param res Vulkan result.
     * @return Returns <c>true</c> if the result represents an error.
     */
    constexpr bool is_error(VkResult res) noexcept;

    /**
     * Checks if a vulkan result represents an error.
     * @param res Vulkan result.
     * @param msg Specifies the thrown message if the result is an error.
     * @throw std::runtime_error Is thrown if the result is an error.
     */
    constexpr void check_result(VkResult res, const char* msg);
}
