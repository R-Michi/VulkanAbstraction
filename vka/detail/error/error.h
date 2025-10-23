/**
 * @brief Includes implementation details for error handling.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

// These functions exist to optimize better for valid code paths and optimize less for error paths.
namespace vka::detail::error
{
    /// @brief Throws a std::invalid_argument exception with a message.
    [[noreturn]] void throw_invalid_argument(const char* msg);

    /// @brief Throws an std::runtime_error exception with a message.
    [[noreturn]] void throw_runtime_error(const char* msg);

    /// @brief Throws a std::out_of_range exception with a message.
    [[noreturn]] void throw_out_of_range(const char* msg);

    /// @brief Throws a std::bad_alloc exception with a message.
    [[noreturn]] void throw_bad_alloc();

    /// @brief Checks if a pointer to dynamic memory is a nullptr and throws a std::bad_alloc exception.
    inline void check_memory(const void* mem);

    /// @brief Checks if a range is valid. If not, an std::out_of_range exception is thrown.
    inline void check_range(uint32_t range_offset, uint32_t rsize, uint32_t size, const char* msg);

    /// @brief Checks if an index exceeds a maximum size.
    template<uint32_t N>
    inline void check_idx(uint32_t idx, const char* msg);
}
