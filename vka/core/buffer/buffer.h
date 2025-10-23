/**
 * @brief Helper class for creating buffers in vulkan.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
    /**
     * @brief This class is a helper for creating buffers.
     * @note Buffers cannot be copied via the copy constructor or operator. Instead, a dedicated copy function is used
     * to RECORD copy commands for buffers.
     */
    class Buffer final
    {
    private:
        static constexpr char BUFFER_CREATE_FAILED[] = "[vka::Buffer::create]: Failed to create buffer handle.";
        static constexpr char ALLOC_MEMORY_FAILED[] = "[vka::Buffer::create]: Failed to allocate memory.";
        static constexpr char BIND_MEMORY_FAILED[] = "[vka::Buffer::create]: Failed to bind memory to buffer.";
        static constexpr char MAP_MEMORY_FAILED[] = "[vka::Buffer::map]: Failed to map memory of buffer";

        VkDevice m_device;
        VkBuffer m_buffer;
        VkDeviceMemory m_memory;
        VkDeviceSize m_size;
        bool m_mapped;

        /// @brief Destroys all created vulkan handles.
        void destroy_handles() const noexcept;

        /**
         * @brief Creates the buffer.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the create-info for the buffer.
         * @throw std::runtime_error Is thrown, if creating the buffer, allocating the memory or binding the memory
         * failed.
         */
        void internal_create(const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

    public:
        /// @details Every vulkan handle is initialized to VK_NULL_HANDLE.
        inline Buffer() noexcept;

        /**
         * @brief Create the buffer.
         * @param device Specifies a valid device.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the create-info for the buffer.
         * @throw std::runtime_error Is thrown, if creating the buffer, allocating the memory or binding the memory
         * failed.
         * @details This constructor has the same functionality as the function create().
         */
        explicit inline Buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

        /**
         * @brief If you want to copy a buffer, use the function 'copy()'. For more information, see the description of
         * this function.
         */
        Buffer(const Buffer&) = delete;
        Buffer& operator= (const Buffer&) = delete;

        /**
         * @brief Moves the ownership of a buffer to a different object.
         * @param src Specifies the buffer to move.
         * @details The source buffer becomes invalidated. If the move destination has been created before and is a
         * valid object, it gets destroyed.
         */
        inline Buffer(Buffer&& src) noexcept;
        inline Buffer& operator= (Buffer&& src) noexcept;

        /// @brief Destroys all created vulkan handles.
        inline ~Buffer();

        /**
         * @brief Create the buffer.
         * @param device Specifies a valid device.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the create-info for the buffer.
         * @throw std::runtime_error Is thrown, if creating the buffer, allocating the memory or binding the memory
         * failed.
         */
        inline void create(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

        /**
         * @brief Destroys all created vulkan handles.
         * @details After the handles are destroyed, they are reset to VK_NULL_HANDLE. Moreover, this function does not
         * destroy parent handles.
         */
        inline void destroy() noexcept;

        /**
         * @brief Maps the buffer's memory.
         * @param offset Specifies the offset of the region to map.
         * @param size Specifies the size of the region to map.
         * @return Returns a pointer to the memory of the mapped buffer.
         * @throw std::runtime_error Is thrown if mapping the buffer failed.
         * @details The region of the buffer to map is given by an offset and a size in bytes. Moreover, this function
         * cannot return 'nullptr'. If mapping the buffer failed, an exception is thrown.
         */
        inline void* map(VkDeviceSize offset, VkDeviceSize size) noexcept;

        /**
         * @brief Unmaps all mapped ranges of the buffer.
         * @details If the buffer is not mapped or if the buffer is invalid, this function does nothing.
         */
        inline void unmap() noexcept;

        /**
         * @brief Records the required command for the copy operation.
         * @param cbo Specifies a valid command buffer in which the copy command is recorded.
         * @param src Specifies the buffer to copy.
         * @details The source and the destination buffer ('this') must be valid to perform the copy operation.
         * Otherwise, the function does nothing. Additionally, the source buffer must have the usage flag
         * VK_BUFFER_USAGE_TRANSFER_SRC_BIT set and the destination buffer must have the usage flag
         * VK_BUFFER_USAGE_TRANSFER_DST_BIT set.\n
         * The whole content if the source buffer is copied into the destination buffer and the destination buffer's
         * size must be greater than or equal to the source buffer's size. More formally: 'dst.size() >= src.size()'.
         */
        inline void copy(VkCommandBuffer cbo, const Buffer& src) noexcept;

        /**
         * @brief Records the required command for copying a region of a buffer.
         * @param cbo Specifies a valid command buffer in which the copy command is recorded.
         * @param src Specifies the buffer to copy.
         * @param region Specifies the region of the buffer to copy.
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
         * @note See the vulkan documentation for VkBufferCopy and vkCmdCopyBuffer as it applies to this copy operation
         * as well, except for the one special case.
         */
        inline void copy_region(VkCommandBuffer cbo, const Buffer& src, const VkBufferCopy& region) noexcept;

        /// @return Returns the size in bytes of the buffer.
        inline VkDeviceSize size() const noexcept;

        /// @return Returns the vulkan VkBuffer handle.
        inline VkBuffer handle() const noexcept;

        /// @return Returns true if the Buffer is a valid object and false otherwise.
        inline bool is_valid() const noexcept;

        /**
         * @brief Checks if a buffer copy is invalid.
         * @param src Specifies the source buffer of a copy operation.
         * @param dst Specifies the destination buffer of a copy operation.
         * @param region Optionally specifies a copy region. This is only required if a region-copy should be checked.
         * @return Returns 'true' if a copy operation is invalid and 'false' if it is valid.
         * @details Those checks are not included in the copy functions as it is already done by the vulkan API
         * (validation layers). Usually, when a copy is invalid, a VK_ERROR_DEVICE_LOST error is returned after the
         * command has been submitted. However, if you wish to check yourself, if a copy is valid or not, you can call
         * this function.
         * @note If a buffer has the correct usage flags set, is not checked here. Those flags are not stored after the
         * buffer has been created.
         */
        static inline bool is_copy_invalid(const Buffer& src, const Buffer& dst, const VkBufferCopy* region = nullptr) noexcept;

        /**
         * @brief Checks if multiple buffer copies are invalid.
         * @param count Specifies the number of buffers to copy.
         * @param src Specifies the source buffers of the copy operations.
         * @param dst Specifies the destination buffers of the copy operation.
         * @param results Specifies an array in which to store the results of the checks.
         * @param regions Optionally specifies copy regions. This is only required if region-copies should be checked.
         * @details All arrays must at least be 'count' elements in length.
         * @note The description of "is_copy_invalid" also applies to this function.
         */
        static inline void is_copy_invalid(uint32_t count, const Buffer* src, const Buffer* dst, bool* results, const VkBufferCopy* regions = nullptr) noexcept;
    };
}
