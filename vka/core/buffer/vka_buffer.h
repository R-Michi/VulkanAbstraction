/**
* @file     vka_buffer.h
* @brief    Buffer definition file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka
{
    class Buffer
    {
    private:
        VkDevice device;
        VkBuffer buffer;
        VkDeviceMemory memory;
        VkDeviceSize memory_size;
        bool mapped;

        /*
        * Checks if everything is correct at creation. Throws exceptions if anything was worng
        * initialized, or was not initialized.
        */
        inline void validate(void);
        inline void destroy_handles(void) noexcept;

    public:
        /*
        * Initialization constructor which initializes 'this' with a device. The device does not
        * have to be valid at initialization, it must be valid at creation. This constructor has
        * the same functionality as the .init() function. Furthermore, this constructor is also
        * used as the default constructor, where the device is initialized to a VK_NULL_HANDLE.
        * All other handles are initialized to a VK_NULL_HANDLE and every other member variable
        * contains its default initialization.
        */
        explicit Buffer(VkDevice device = VK_NULL_HANDLE) noexcept;

        /*
        * In order to copy a buffer the function .copy() must be used. For more information see the
        * description of these functions.
        */
        Buffer(const Buffer&) = delete;
        Buffer& operator= (const Buffer&) = delete;

        /*
        * Moves another object of Buffer into 'this'. 'This' now holds the ownership of all the
        * handles of the source object. The source object will become invalidated and contains its
        * default initialization except for the device. The device will be copied and is preserved
        * in the moved object. If 'this' was created and is a valid object, 'this' is destroyed and
        * replaced by the handles of the moved object.
        */
        Buffer(Buffer&& src) noexcept;
        Buffer& operator= (Buffer&& src) noexcept;

        // The destructor destroyes all the vulkan handles.
        virtual ~Buffer(void);

        /*
        * Initializes 'this' with a device. The device does not have to be valid at initialization.
        * However, it must be valid at creation. The initialization cannot be changed, if the
        * Buffer is a valid object.
        */
        void init(VkDevice device) noexcept;

        /*
        * This function creates the Buffer and the internal handles are now valid, if no error
        * occured. If an error occured, the vulkan result is returned. If no error occured,
        * VK_SUCCESS is returned. The Buffer is created with a BufferCreateInfo structure which is
        * used for the creation of the buffer and memory handle. The create info is specified by
        * 'create_info'. Additionally, the memory properties of the physical device are requiered
        * and specified by 'properties'. An std::invalid_argument exception is thrown, if 'this'
        * has not been initialized.
        */
        VkResult create(const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

        /*
        * Destroyes the Buffer object. After destroying, 'this' holds its default initialization
        * except for the device. The device will be preserved after destroying and 'this' does not
        * need to be reinitialized. This is also done by the destructor.
        */
        void destroy(void) noexcept;

        /*
        * This functions maps the buffer's memory and returns a pointer to the mapped buffer.
        * The region to map is specified by an offset and a size. The offset is specified by
        * 'offset' and the size is specified by 'size'. If the mapping operation was not successful
        * or if the buffer is not valid, nullptr is returned.
        */
        inline void* map(VkDeviceSize offset, VkDeviceSize size) noexcept;

        // Unmaps all mapped memory of the buffer.
        inline void unmap(void) noexcept;

        /*
        * This function records the requiered commands for the copy operation.
        *
        * The source and the destination buffers must be valid objects. If they are invalid, this
        * function does nothing. The destination buffer must be a valid object (must have been
        * created) because it can have a different initialization and different properties than the
        * source buffer. Addtionally, the source buffer must have the usage flag
        * VK_BUFFER_USAGE_TRANSFER_SRC_BIT set and the destination buffer must have the usage flag
        * VK_BUFFER_USAGE_TRANSFER_DST_BIT set.
        *
        * The whole source buffer is copied into the destination buffer and the destination
        * buffer's size must be greater than or equal to the source buffer's size.
        * More formally:
        *   'dst.size() >= src.size()'
        */
        inline void copy(VkCommandBuffer cbo, const Buffer& src);

        /*
        * Does the same as the copy() function but a copy region can be specified by
        * 'region'. The buffer is copied according to that region. However, there is one exception,
        * if VkBufferCopy::size equals 0:
        * 
        * If VkBufferCopy::size equals 0, everything from 'VkBufferCopy::srcOffset' to 'src.size()'
        * is copied from the source buffer into the destination buffer. The size of the destination
        * buffer must then be at least:
        *   'VkBufferCopy::dstOffset + (src.size() - VkBufferCopy::srcOffset)' bytes.
        * More formally:
        *   'dst.size() >= VkBufferCopy::dstOffset + (src.size() - VkBufferCopy::srcOffset)'
        *
        * If VkBufferCopy::size is greater than 0, then 'VkBufferCopy::size' bytes are copied and
        * the size of the destination buffer must be at least:
        *   'VkBufferCopy::dstOffset + VkBufferCopy::size' bytes.
        * More formally:
        *   'dst.size() >= VkBufferCopy::dstOffset + VkBufferCopy::size'
        *
        * See the vulkan documentation for VkBufferCopy and vkCmdCopyBuffer as it also applies to
        * this copy operation as well, except for the one special case.
        */
        inline void copy_region(VkCommandBuffer cbo, const Buffer& src, const VkBufferCopy& region);

        // Returns the size in bytes of the buffer.
        inline VkDeviceSize size(void) const noexcept;

        // Returns the vulkan buffer handle.
        inline VkBuffer handle(void) const noexcept;

        // Returns true, if the Buffer is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;

        /*
        * This function checks, if a buffer copy is valid. This check is not included when calling
        * Buffer::copy or Buffer::enqueue_copy as it is already done by the vulkan API in the
        * background. Usually, when a copy is invalid a VK_ERROR_DEVICE_LOST error is returned
        * after the command has been submitted. However, if you wish to check yourself, if a copy
        * is valid or not, you can call this function. It returns TRUE, if a copy is INVALID and
        * FALSE, if a copy is VALID. The source buffer is specified by 'src' and the destination
        * buffer is specified by 'dst'. A region can optionally be specified by 'region' like the
        * copy functions.
        * NOTE: If a buffer has the corrent usage flags set, is not tested here because those flags
        * are not stored after the buffer has been created.
        */
        static inline bool is_copy_invalid(const Buffer& src, const Buffer& dst, const VkBufferCopy* region = nullptr) noexcept;

        /*
        * This function has the same functionality but with multiple buffers. The results are
        * stored in the boolean array specified by 'results'. The boolean array must have an
        * element for each pair of buffers (source and destination) to validate. The same rule
        * applies to the optional regions specified by 'regions'.
        */
        static inline void is_copy_invalid(uint32_t count, const Buffer* src, const Buffer* dst, bool* results, const VkBufferCopy* regions = nullptr) noexcept;
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_buffer_impl.inl"
#endif
#include "vka_buffer_inline_impl.inl"
