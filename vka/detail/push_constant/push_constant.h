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
        /// Initializes an empty push constant buffer.
        constexpr PushConstantBuffer() noexcept;

        /**
         * Allocates memory at construction.
         * @param size Number of bytes to allocate.
         */
        explicit constexpr PushConstantBuffer(uint32_t size);

        /// Moves a buffer. The source buffer becomes invalidated and using to results in undefined behaviour.
        constexpr PushConstantBuffer(PushConstantBuffer&& src) noexcept;

        /// Frees the allocated memory.
        constexpr ~PushConstantBuffer();

        /**
         * Moves a buffer. The source buffer becomes invalidated and using to results in undefined behaviour. An already
         * allocated buffer is destroyed.
         */
        constexpr PushConstantBuffer& operator= (PushConstantBuffer&& src) noexcept;

        /// Returns a pointer to the push constant memory.
        constexpr void* data() noexcept;
        constexpr const void* data() const noexcept;

        /// Returns the size of the push constant buffer.
        constexpr uint32_t size() const noexcept;

        /// Returns whether the push constant buffer is empty.
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
