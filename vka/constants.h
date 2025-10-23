/**
 * @brief Includes constants used by vka.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
    constexpr uint32_t NPOS = 0xFFFFFFFF;
    constexpr uint32_t NCOUNT = ~static_cast<size_t>(0);
    constexpr size_t NSIZE = ~static_cast<size_t>(0);
    constexpr uint32_t INVALID_MTL_ID = 0xFFFFFFFF;
    constexpr uint64_t NO_TIMEOUT = 0xFFFFFFFFFFFFFFFF;
}
