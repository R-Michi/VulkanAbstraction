/**
 * @brief Implementation details for error handling.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "error.h"

inline void vka::detail::error::check_memory(const void* mem)
{
    if (mem == nullptr) [[unlikely]]
        throw_bad_alloc();
}

inline void vka::detail::error::check_range(uint32_t range_offset, uint32_t rsize, uint32_t size, const char* msg)
{
    if ((range_offset + rsize) > size) [[unlikely]]
        throw_out_of_range(msg);
}

template<uint32_t N>
inline void vka::detail::error::check_idx(uint32_t idx, const char* msg)
{
    if (idx >= N) [[unlikely]]
        throw_out_of_range(msg);
}
