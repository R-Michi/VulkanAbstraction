/**
* @file     vka_test.h
* @brief    Testing assembly code of format_sizeof function.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "vka_test.h"

size_t foo(VkFormat format)
{
    return vka::format_sizeof(VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK);
}
