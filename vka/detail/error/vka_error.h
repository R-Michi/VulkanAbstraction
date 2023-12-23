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
    [[noreturn]] void throw_invalid_argument(const char* msg);

    // Throws a std::runtime_error exception with a message.
    [[noreturn]] void throw_runtime_error(const char* msg);

    // Throws a std::out_of_range exception with a message.
    [[noreturn]] void throw_out_of_range(const char* msg);

    // Throws a std::bad_alloc exception with a message.
    [[noreturn]] void throw_bad_alloc(void);

    // Checks, if the vulkan result is an error.
    inline bool is_error(VkResult res) noexcept;

    // Checks, if the vulkan result is an error and throws a std::runtime_error exception.
    inline void check_result(VkResult res, const char* msg);

    // Checks, if a pointer to dynamic memory is a nullptr and throws a std::bad_alloc exception.
    inline void check_memory(const void* mem);

    // Checks, if a range is valid. If not a std::out_of_range exception is thrown.
    inline void check_range(uint32_t roffset, uint32_t rsize, uint32_t size, const char* msg);
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_error_impl.inl"
#endif
#include "vka_error_inline_impl.inl"
