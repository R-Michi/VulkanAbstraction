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

float d, c;
__attribute__((noinline)) float foo(VkFormat format)
{
    return d * c;
}

float a, b, x, y;
void foo2(VkFormat format)
{
    a = a * b;
    float z = foo(format);
    foo(format);
    a = a + b + z;
    z = foo(format);
    x = a * y * z;
}
