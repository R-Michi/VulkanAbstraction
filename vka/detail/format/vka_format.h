/**
* @file     vka_utility.h
* @brief    Detailed declaration of format functionality.
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
    constexpr size_t get_format_size_offset(VkFormat format);
}

#include "vka_format_inline_impl.inl"
