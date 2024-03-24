/**
* @file     vka_format_inline_impl.inl
* @brief    Detailed implementation of format functionality.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka::detail::format
{
    constexpr size_t SIZE_LOOKUP[300] = {

        // Standard / non-extension formats, (range [0, 184]):
        0, 1, 2, 2, 2, 2, 2, 2, 2, 1,
        1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
        2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        2, 2, 2, 2, 2, 2, 2, 4, 4, 4,
        4, 4, 4, 4, 6, 6, 6, 6, 6, 6,
        6, 8, 8, 8, 8, 8, 8, 8, 4, 4,
        4, 8, 8, 8, 12, 12, 12, 16, 16, 16,
        8, 8, 8, 16, 16, 16, 24, 24, 24, 32,
        32, 32, 4, 4, 2, 4, 4, 1, 3, 4,
        8, 8, 8, 8, 8, 16, 16, 16, 16, 8,
        8, 16, 16, 16, 16, 16, 16, 8, 8, 8,
        8, 16, 16, 8, 8, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // extension formats, (range [1000156000, 1000156033]):
        4, 4, 1, 1, 1, 1, 1, 2, 4, 8,
        8, 8, 6, 6, 6, 6, 6, 2, 4, 8,
        8, 8, 6, 6, 6, 6, 6, 8, 8, 6,
        6, 6, 6, 6, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // extension formats, (range [1000330000, 1000330003]):
        3, 6, 6, 6, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // extension formats, (range [1000340000, 1000340001]):
        2, 2, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // extension formats, (range [1000066000, 1000066013]):
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // extension formats, (range [1000054000, 1000054007]):
        8, 8, 8, 8, 8, 8, 8, 8, NSIZE, NSIZE,

        // extension formats, (range [1000464000, 1000464000]):
        4
    };
}

constexpr size_t vka::detail::format::get_format_size_offset(VkFormat format)
{
    const uint32_t iformat = static_cast<uint32_t>(format);

    // all available vulkan formats
    if (iformat <= 184)                                     return 0;
    if (iformat >= 1000156000   && iformat <= 1000156033)   return 1000156000 - 190;
    if (iformat >= 1000330000   && iformat <= 1000330003)   return 1000330000 - 230;
    if (iformat >= 1000340000   && iformat <= 1000340001)   return 1000340000 - 240;
    if (iformat >= 1000066000   && iformat <= 1000066013)   return 1000066000 - 250;
    if (iformat >= 1000054000   && iformat <= 1000054007)   return 1000054000 - 270;
    if (iformat >= 1000464000   && iformat <= 1000464000)   return 1000464000 - 280;

    // this statement should not be reached, if a valid vulkan format is given as input
    return NPOS;
}
