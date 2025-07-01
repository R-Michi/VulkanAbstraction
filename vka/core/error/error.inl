/**
 * @brief Implements functions for error handling and result checking.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

inline bool vka::is_error(VkResult res) noexcept
{
    return static_cast<int32_t>(res) < 0;
}

inline void vka::check_result(VkResult res, const char* msg)
{
    if (is_error(res)) [[unlikely]]
        detail::error::throw_runtime_error(msg);
}
