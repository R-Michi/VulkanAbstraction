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
    private:
        uint32_t m_size;
        void* m_data;

        /**
         * @brief Frees the allocated memory.
         * @details If no memory has been allocated, this function does nothing.
         */
        void free_memory() const noexcept;

    public:

        /// @brief Default initialization, no memory gets allocated.
        inline PushConstantBuffer() noexcept;

        /**
         * @brief Allocates memory at construction.
         * @param size Specifies the number of bytes to allocate.
         */
        explicit inline PushConstantBuffer(uint32_t size);

        // As PushConstants cannot be copied, it does not make sense to copy this class.
        PushConstantBuffer(const PushConstantBuffer&) = delete;
        PushConstantBuffer& operator= (const PushConstantBuffer&) = delete;

        /**
         * @brief Moves another object of PushConstantBuffer into 'this'.
         * @details After move 'this' holds the ownership of the memory from the moved object.
         * The source object becomes invalidated. If 'this' currently has memory allocated, this
         * memory gets freed.
         * @param src Specifies the object to move.
         */
        inline PushConstantBuffer(PushConstantBuffer&& src) noexcept;
        inline PushConstantBuffer& operator= (PushConstantBuffer&& src) noexcept;

        /// @brief Frees the allocated memory.
        inline ~PushConstantBuffer();

        /**
         * @brief Allocates 'size' bytes of memory.
         * @details If memory has already been allocated, this function does nothing.
         * @param size Specifies the number of bytes to allocate.
         */
        void allocate(uint32_t size);

        /**
         * @brief Deallocates the memory of the buffer.
         * @details If no memory has been allocated, this function does nothing. Additionally, the size
         * of the buffer is reset to 0.
         */
        inline void free() noexcept;

        /// @return Returns the address of the allocated memory.
        inline void* data() noexcept;
        inline const void* data() const noexcept;

        /// @return Returns the current allocation size.
        inline uint32_t size() const noexcept;

        /// @return Returns whether the buffer is empty or not.
        inline bool empty() const noexcept;
    };
}

#include "push_constant.inl"
