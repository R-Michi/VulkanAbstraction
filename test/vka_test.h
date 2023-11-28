/**
* @file     vka_test.h
* @brief    Main include file of the test bench.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include <vulkan/vulkan.h>
#include "../vka/vka.h"

namespace vka::test
{
    inline void unique_handle_asm(void) noexcept;
}
