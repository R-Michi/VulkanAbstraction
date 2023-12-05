/**
* @file     vka_error.h
* @brief    Declaration of error handling functionality.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

// These functions exist to optimize better for valid code paths and optimize less for error paths.
namespace vka::detail::error
{
    // Throws a std::invalid_argument exception with a message.
    void throw_invalid_argument(const char* msg);

    // Throws a std::runtime_error exception with a message.
    void throw_runtime_error(const char* msg);

    // Throws a std::out_of_range exception with a message.
    void throw_out_of_range(const char* msg);

    // Throws a std::bad_alloc exception with a message.
    void throw_bad_alloc(void);

    // Checks, if the vulkan result is an error.
    inline bool is_error(VkResult res) noexcept;

    // Checks, if the vulkan result is an error and throws a std::runtime_error exception.
    inline void check_result(VkResult res, const char* msg) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_error_impl.inl"
#endif
#include "vka_error_inline_impl.inl"
