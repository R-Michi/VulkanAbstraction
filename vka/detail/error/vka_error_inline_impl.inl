/**
* @file     vka_error.h
* @brief    Implementation of inline error functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline bool vka::detail::error::is_error(VkResult res) noexcept
{
    return static_cast<int32_t>(res) < 0;
}

inline void vka::detail::error::check_result(VkResult res, const char* msg) noexcept
{
    if (is_error(res)) [[unlikely]]
        throw_runtime_error(msg);
}
