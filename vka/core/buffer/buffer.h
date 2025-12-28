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
    class Buffer
    {
        using BufferMemory = detail::buffer::Handle;
    public:
        /// Default initialization = empty buffer.
        constexpr Buffer() noexcept;

        /**
         * Create the buffer.
         * @param device Specifies the device with which the buffer is created.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the create-info for the buffer.
         * @throw std::runtime_error Is thrown, if creating the buffer, allocating the memory or binding the memory
         * failed.
         */
        explicit Buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

        /// Moves a buffer object. A currently held buffer is destroyed.
        constexpr Buffer(Buffer&& src) noexcept;

        /// Unmaps the memory if mapped.
        constexpr ~Buffer();

        /// Moves a buffer object. A currently held buffer is destroyed.
        constexpr Buffer& operator= (Buffer&& src) noexcept;

        /// Returns true if the buffer is valid.
        constexpr explicit operator bool() const noexcept;

        /**
         * Maps the memory of the whole buffer.
         * @return Returns a pointer to the memory of the mapped buffer. The returned pointer is not nullptr.
         * @throw std::runtime_error Is thrown if mapping the buffer failed.
         */
        inline void* map();

        /**
         * Maps a specific region of the memory.
         * @param offset Specifies the offset of the region to map.
         * @param size Specifies the size of the region to map.
         * @return Returns a pointer to the memory of the mapped buffer. The returned pointer is not nullptr.
         * @throw std::runtime_error Is thrown if mapping the buffer failed.
         */
        inline void* map(VkDeviceSize offset, VkDeviceSize size);

        /// Unmaps the memory.
        inline void unmap() noexcept;

        /**
         * Records the commands to copy the whole buffer.\n
         * For correct usage see the vulkan documentation of vkCmdCopyBuffer.
         * @param cbo Specifies a valid command buffer in which the copy command is recorded.
         * @param src Specifies the buffer to copy.
         */
        inline void copy(VkCommandBuffer cbo, const Buffer& src) noexcept;

        /**
         * Records the commands to copy a specific region of the buffer.\n
         * For correct usage see the vulkan documentation of vkCmdCopyBuffer.
         * @param cbo Specifies a valid command buffer in which the copy command is recorded.
         * @param src Specifies the buffer to copy.
         * @param region Specifies the region of the buffer to copy.
         */
        inline void copy_region(VkCommandBuffer cbo, const Buffer& src, const VkBufferCopy& region) noexcept;

        /// @return Returns the allocated size in bytes.
        constexpr VkDeviceSize size() const noexcept;

        /// @return Returns the vulkan VkBuffer handle.
        constexpr VkBuffer handle() const noexcept;

        // deleted
        Buffer(const Buffer&) = delete;
        Buffer& operator= (const Buffer&) = delete;

    private:
        static constexpr char BUFFER_CREATE_FAILED[] = "[vka::Buffer::create]: Failed to create buffer handle.";
        static constexpr char ALLOC_MEMORY_FAILED[] = "[vka::Buffer::create]: Failed to allocate memory.";
        static constexpr char BIND_MEMORY_FAILED[] = "[vka::Buffer::create]: Failed to bind memory to buffer.";
        static constexpr char MAP_MEMORY_FAILED[] = "[vka::Buffer::map]: Failed to map memory of buffer";

        unique_handle<BufferMemory> m_buffer;
        VkDeviceSize m_size;
        bool m_map_status;

        /// Unmaps the memory without resetting the status.
        constexpr void unmap_memory() const noexcept;

        /// Creates the buffer and its associated memory.
        static BufferMemory create_buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);
    };
}
