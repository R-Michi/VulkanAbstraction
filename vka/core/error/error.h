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
     * @param res Specifies the vulkan result.
     * @return Returns 'true' if the result represents an error.
     */
    inline bool is_error(VkResult res) noexcept;

    /**
     * @brief Checks if a vulkan result represents an error.
     * @param res Specifies the vulkan result.
     * @param msg Specifies the message thrown if the result represents an error.
     * @throw std::runtime_error Is thrown if the result represents an error.
     */
    inline void check_result(VkResult res, const char* msg);
}
