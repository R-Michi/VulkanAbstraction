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
        static constexpr const char BUFFER_CREATE_FAILED[] = "[vka::Buffer::create]: Failed to create buffer handle.";
        static constexpr const char ALLOC_MEMORY_FAILED[] = "[vka::Buffer::create]: Failed to allocate memory.";
        static constexpr const char BIND_MEMORY_FAILED[] = "[vka::Buffer::create]: Failed to bind memory to buffer.";
        static constexpr const char MAP_MEMORY_FAILED[] = "[vka::Buffer::map]: Failed to map memory of buffer";

        VkDevice m_device;
        VkBuffer m_buffer;
        VkDeviceMemory m_memory;
        VkDeviceSize m_size;
        bool m_mapped;

        /// @brief Destroys all created vulkan handles.
        void destroy_handles(void) noexcept;

        /**
         * @brief Creates the buffer.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the create info for the buffer.
         * @throw std::runtime_error Is thrown, if creating the buffer, allocating the memory or binding the memory
         * failed.
         */
        void internal_create(const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

    public:
        /// @details Every vulkan handle is initialized to VK_NULL_HANDLE.
        inline Buffer(void) noexcept;

        /**
         * @brief Create the buffer.
         * @details This constructor has the same functionality as the function create().
         * @param device Specifies a valid device.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the create info for the buffer.
         * @throw std::runtime_error Is thrown, if creating the buffer, allocating the memory or binding the memory
         * failed.
         */
        explicit inline Buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

        /**
         * @details In order to copy a buffer the function .copy() must be used. For more information see the
         * description of these functions.
         */
        Buffer(const Buffer&) = delete;
        Buffer& operator= (const Buffer&) = delete;

        /**
         * @brief Transfers ownership of a buffer to 'this'.
         * @details The source buffer becomes invalidated. If 'this' has been created before and is a valid instance, it
         * gets destroyed and replaced by the handles of the source object.
         * @param src Specifies the buffer to move.
         */
        inline Buffer(Buffer&& src) noexcept;
        inline Buffer& operator= (Buffer&& src) noexcept;

        /// @brief Destroys all created vulkan handles.
        virtual inline ~Buffer(void);

        /**
         * @brief Create the buffer.
         * @param device Specifies a valid device.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the create info for the buffer.
         * @throw std::runtime_error Is thrown, if creating the buffer, allocating the memory or binding the memory
         * failed.
         */
        inline void create(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

        /**
         * @brief Destroys all created vulkan handles.
         * @details After the handles are destroyed they are reset to VK_NULL_HANDLE. Moreover, parent handles are not
         * destroyed by this function.
         */
        inline void destroy(void) noexcept;

        /**
         * @brief Maps the buffer's memory.
         * @details The region of the buffer to map is given by an offset and a size in bytes. Moreover, this function
         * can not return 'nullptr'. If mapping the buffer failed, an exception is thrown.
         * @param offset Specifies the offset of the region to map.
         * @param size Specifies the size of the region to map.
         * @return Returns a pointer to the memory of the mapped buffer.
         * @throw std::runtime_error Is thrown, if mapping the buffer failed.
         */
        inline void* map(VkDeviceSize offset, VkDeviceSize size) noexcept;

        /**
         * @brief Unmaps all mapped ranges of the buffer.
         * @details If the buffer is not mapped or if the buffer is invalid, this function does nothing.
         */
        inline void unmap(void) noexcept;

        /**
         * @brief Records the required command for the copy operation.
         * @details The source and the destination buffer ('this') must be valid in order to perform the copy operation.
         * Otherwise, the function does nothing. Additionally, the source buffer must have the usage flag
         * VK_BUFFER_USAGE_TRANSFER_SRC_BIT set and the destination buffer must have the usage flag
         * VK_BUFFER_USAGE_TRANSFER_DST_BIT set.\n
         * The whole content if the source buffer is copied into the destination buffer and the destination buffer's
         * size must be greater than or equal to the source buffer's size. More formally: 'dst.size() >= src.size()'.
         * @param cbo Specifies a valid command buffer in which the copy command is recorded.
         * @param src Specifies the buffer to copy.
         */
        inline void copy(VkCommandBuffer cbo, const Buffer& src) noexcept;

        /**
         * @brief Records the required command for copying a region of a buffer.
         * @details Has the same functionality as the function copy(). Instead of copying the whole buffer, a region can
         * be specified that should be copied. The copy operation works as follows:\n\n
         * If VkBufferCopy::size equals 0, everything from 'VkBufferCopy::srcOffset' to 'src.size()' is copied. The size
         * of the destination buffer must then be at least:\n
         * 'VkBufferCopy::dstOffset + (src.size() - VkBufferCopy::srcOffset)' bytes.\n
         * More formally:\n
         * 'dst.size() >= VkBufferCopy::dstOffset + (src.size() - VkBufferCopy::srcOffset)'\n\n
         * If VkBufferCopy::size is greater than 0, 'VkBufferCopy::size' bytes are copied. The size of the destination
         * buffer must be at least:\n
         * 'VkBufferCopy::dstOffset + VkBufferCopy::size' bytes.\n
         * More formally:\n
         * 'dst.size() >= VkBufferCopy::dstOffset + VkBufferCopy::size'
         * @param cbo Specifies a valid command buffer in which the copy command is recorded.
         * @param src Specifies the buffer to copy.
         * @param region Specifies the region of the buffer to copy.
         * @note See the vulkan documentation for VkBufferCopy and vkCmdCopyBuffer as it also applies to this copy
         * operation as well, except for the one special case.
         */
        inline void copy_region(VkCommandBuffer cbo, const Buffer& src, const VkBufferCopy& region) noexcept;

        /// @return Returns the size in bytes of the buffer.
        inline VkDeviceSize size(void) const noexcept;

        /// @return Returns the vulkan VkBuffer handle.
        inline VkBuffer handle(void) const noexcept;

        /// @return Returns true, if the Buffer is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;

        /**
         * @brief Checks, if a buffer copy is invalid.
         * @details Those checks are not included in the copy functions as it is already done by the vulkan API
         * (validation layers). Usually, when a copy is invalid a VK_ERROR_DEVICE_LOST error is returned after the
         * command has been submitted. However, if you wish to check yourself, if a copy is valid or not, you can call
         * this function.
         * @param src Specifies the source buffer of a copy operation.
         * @param dst Specifies the destination buffer of a copy operation.
         * @param region Optionally specifies a copy region. This is only required, if a region-copy should be checked.
         * @return Returns 'true', if a copy operation would be invalid and 'false', if it would be valid.
         * @note If a buffer has the correct usage flags set, is not checked here. Those flags are not stored after the
         * buffer has been created.
         */
        static inline bool is_copy_invalid(const Buffer& src, const Buffer& dst, const VkBufferCopy* region = nullptr) noexcept;

        /**
         * @brief Checks, if multiple buffer copies are invalid.
         * @details All arrays must at least be 'count' elements in length.
         * @param count Specifies the number of buffers to copy.
         * @param src Specifies the source buffers of the copy operations.
         * @param dst Specifies the destination buffers of the copy operation.
         * @param results Specifies an array in which to store the results of the checks.
         * @param regions Optionally specifies copy regions. This is only required, if a region-copies should be
         * checked.
         * @note Also read the description of the "single" version of this function.
         */
        static inline void is_copy_invalid(uint32_t count, const Buffer* src, const Buffer* dst, bool* results, const VkBufferCopy* regions = nullptr) noexcept;
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_buffer_impl.inl"
#endif
#include "vka_buffer_inline_impl.inl"
