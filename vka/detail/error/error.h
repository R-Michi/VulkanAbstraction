/**
 * @brief Includes implementation details for error handling.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

// These functions exist to optimize better for valid code paths and optimize less for error paths.
namespace vka::detail::error
{
    /// Throws a <c>std::invalid_argument</c> exception with a message.
    [[noreturn]]
    void throw_invalid_argument(const char* msg);

    /// Throws an <c>std::runtime_error</c> exception with a message.
    [[noreturn]]
    void throw_runtime_error(const char* msg);

    /// Throws a <c>std::out_of_range</c> exception with a message.
    [[noreturn]]
    void throw_out_of_range(const char* msg);

    /// Throws a <c>std::bad_alloc</c> exception with a message.
    [[noreturn]]
    void throw_bad_alloc();

    /// Checks if a pointer to dynamic memory is a <c>nullptr</c> and throws a <c>std::bad_alloc</c> exception.
    constexpr void check_memory(const void* mem);

    /// Checks if a range is valid. If not, an <c>std::out_of_range</c> exception is thrown.
    constexpr void check_range(uint32_t range_offset, uint32_t rsize, uint32_t size, const char* msg);

    /// Checks if an index exceeds a maximum size.
    template<uint32_t N>
    constexpr void check_idx(uint32_t idx, const char* msg);
}
