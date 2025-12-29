/**
 * @brief Includes the push-constant buffer.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::detail
{
    class PushConstantBuffer final
    {
    public:

        /// Default initialization, no memory gets allocated.
        constexpr PushConstantBuffer() noexcept;

        /**
         * Allocates memory at construction.
         * @param size Specifies the number of bytes to allocate.
         */
        explicit inline PushConstantBuffer(uint32_t size);

        /// Moves an object. If the source object is reused, free() must be called on it.
        constexpr PushConstantBuffer(PushConstantBuffer&& src) noexcept;

        /// @brief Frees the allocated memory.
        constexpr ~PushConstantBuffer();

        /// Moves an object. If the source object is reused, free() must be called on it.
        constexpr PushConstantBuffer& operator= (PushConstantBuffer&& src) noexcept;

        /**
         * Allocates memory. If memory has already been allocated, this function does nothing.
         * @param size Specifies the number of bytes to allocate.
         */
        inline void allocate(uint32_t size);

        /// Deallocates the memory.
        constexpr void free() noexcept;

        /// @return Returns the address of the allocated memory.
        constexpr void* data() noexcept;
        constexpr const void* data() const noexcept;

        /// @return Returns the current allocation size.
        constexpr uint32_t size() const noexcept;

        /// @return Returns whether the buffer is empty.
        constexpr bool empty() const noexcept;

        // deleted
        PushConstantBuffer(const PushConstantBuffer&) = delete;
        PushConstantBuffer& operator= (const PushConstantBuffer&) = delete;

    private:
        uint32_t m_size;
        void* m_data;

        /// Frees the allocated memory without resetting the state.
        constexpr void free_memory() const noexcept;
    };
}
