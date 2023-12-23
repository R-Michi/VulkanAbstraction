/**
* @file     vka_push_constant.h
* @brief    Detailed push constant definition file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka::detail
{
    class PushConstantBuffer
    {
    private:
        uint32_t m_size;
        void* m_data;

        /**
         * @brief Frees the allocated memory.
         * @details If no memory has been allocated, this function does nothing.
         */
        void free_memory(void) noexcept;

    public:

        /// @brief Default initialization, no memory gets allocated.
        inline PushConstantBuffer(void) noexcept;

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
        virtual inline ~PushConstantBuffer(void);

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
        inline void free(void) noexcept;

        /// @return Returns the address of the allocated memory.
        inline void* data(void) noexcept;

        /// @return Returns the current allocation size.
        inline uint32_t size(void) const noexcept;

        /// @return Returns whether the buffer is empty or not.
        inline bool empty(void) const noexcept;
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_push_constant_impl.inl"
#endif
#include "vka_push_constant_inline_impl.inl"
