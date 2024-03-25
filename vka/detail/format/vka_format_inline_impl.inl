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
    constexpr size_t SIZE_LOOKUP[292] = {
        // VK_VERSION_1_0 (non-extension) formats, (range [0, 184]):
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

        // VK_VERSION_1_1 (non-extension) formats, (range [1000156000, 1000156033]):
        4, 4, 3, 3, 3, 3, 3, 2, 4, 8,
        8, 8, 6, 6, 6, 6, 6, 2, 4, 8,
        8, 8, 6, 6, 6, 6, 6, 8, 8, 6,
        6, 6, 6, 6, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_VERSION_1_3 (non-extension) formats, (range [1000330000, 1000330003]):
        3, 6, 6, 6, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_VERSION_1_3 (non-extension) formats, (range [1000340000, 1000340001]):
        2, 2, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_VERSION_1_3 (non-extension) formats, (range [1000066000, 1000066013]):
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_IMG_format_pvrtc (extension) formats, (range [1000054000, 1000054007]):
        8, 8, 8, 8, 8, 8, 8, 8, NSIZE, NSIZE,

        // VK_NV_optical_flow (extension) formats, (range [1000464000, 1000464000]):
        2, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_KHR_maintenance5 (extension) formats, (range [1000470000, 1000470001]):
        2, 1
    };

    constexpr size_t COUNT_LOOKUP[292] = {
        // VK_VERSION_1_0 (non-extension) formats, (range [0, 184]):
        0, 2, 4, 4, 3, 3, 4, 4, 4, 1,
        1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
        2, 2, 2, 3, 3, 3, 3, 3, 3, 3,
        3, 3, 3, 3, 3, 3, 3, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
        2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
        3, 4, 4, 4, 4, 4, 4, 4, 1, 1,
        1, 2, 2, 2, 3, 3, 3, 4, 4, 4,
        1, 1, 1, 2, 2, 2, 3, 3, 3, 4,
        4, 4, 3, 3, 1, 2, 1, 1, 2, 2,
        2, 3, 3, 4, 4, 4, 4, 4, 4, 1,
        1, 2, 2, 3, 3, 4, 4, 3, 3, 4,
        4, 4, 4, 1, 1, 2, 2, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_VERSION_1_1 (non-extension) formats, (range [1000156000, 1000156033]):
        4, 4, 3, 3, 3, 3, 3, 1, 2, 4,
        4, 4, 3, 3, 3, 3, 3, 1, 2, 4,
        4, 4, 3, 3, 3, 3, 3, 4, 4, 3,
        3, 3, 3, 3, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_VERSION_1_3 (non-extension) formats, (range [1000330000, 1000330003]):
        3, 3, 3, 3, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_VERSION_1_3 (non-extension) formats, (range [1000340000, 1000340001]):
        4, 4, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_VERSION_1_3 (non-extension) formats, (range [1000066000, 1000066013]):
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_IMG_format_pvrtc (extension) formats, (range [1000054000, 1000054007]):
        4, 4, 4, 4, 4, 4, 4, 4, NSIZE, NSIZE,

        // VK_NV_optical_flow (extension) formats, (range [1000464000, 1000464000]):
        // NOTE: It says R16G16, however in memory is only one component stored. The second one is calculated from the
        // first one.
        1, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE, NSIZE,

        // VK_KHR_maintenance5 (extension) formats, (range [1000470000, 1000470001]):
        4, 1
    };
}

constexpr size_t vka::detail::format::format_lut_offset(VkFormat format)
{
    const uint32_t i_format = static_cast<uint32_t>(format);

    // all available vulkan formats
    if (i_format >= 1000156000 && i_format <= 1000156033)   return 1000156000 - 190;
    if (i_format >= 1000330000 && i_format <= 1000330003)   return 1000330000 - 230;
    if (i_format >= 1000340000 && i_format <= 1000340001)   return 1000340000 - 240;
    if (i_format >= 1000066000 && i_format <= 1000066013)   return 1000066000 - 250;
    if (i_format >= 1000054000 && i_format <= 1000054007)   return 1000054000 - 270;
    if (i_format >= 1000464000 && i_format <= 1000464000)   return 1000464000 - 280;
    if (i_format >= 1000470000 && i_format <= 1000470001)   return 1000464000 - 290;

    // if (i_format <= 184)
    return 0;
}
