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
    /// Simplifies creating buffers in vulkan.
    class Buffer
    {
        using BufferHandle = detail::buffer::Handle;

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
        explicit inline Buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

        /**
         * Moves a buffer object. A currently valid buffer is destroyed. DO NOT use the source buffer after a move! This
         * may lead to unexpected or even undefined behavior.
         */
        constexpr Buffer(Buffer&& src) noexcept;

        /// Unmaps the memory if mapped.
        constexpr ~Buffer();

        /**
         * Moves a buffer object. A currently valid buffer is destroyed. DO NOT use the source buffer after a move! This
         * may lead to unexpected or even undefined behavior.
         */
        constexpr Buffer& operator= (Buffer&& src) noexcept;

        /// @return Returns true whether the buffer is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the allocated size in bytes.
        constexpr VkDeviceSize size() const noexcept;

        /// @return Returns the vulkan VkBuffer handle.
        constexpr VkBuffer handle() const noexcept;

        /// @return Reruns the parent handle.
        constexpr VkDevice parent() const noexcept;

        /// @return Returns the device address of the buffer.
        inline VkDeviceAddress device_address() const noexcept;

        /**
         * Maps the memory of the whole buffer.
         * @return Returns a pointer to the memory of the mapped buffer. The returned pointer is not nullptr.
         * @throw std::runtime_error Is thrown if mapping the buffer failed.
         */
        constexpr void* map();

        /**
         * Maps a specific region of the memory.
         * @param offset Specifies the offset of the region to map.
         * @param size Specifies the size of the region to map.
         * @return Returns a pointer to the memory of the mapped buffer. The returned pointer is not nullptr.
         * @throw std::runtime_error Is thrown if mapping the buffer failed.
         */
        constexpr void* map(VkDeviceSize offset, VkDeviceSize size);

        /// Unmaps the memory.
        constexpr void unmap() noexcept;

        /**
         * Records the command to copy the whole buffer. For correct usage see the vulkan documentation of
         * <c>vkCmdCopyBuffer</c>.
         * @param cbo Specifies the command buffer in which the copy command is recorded.
         * @param src Specifies the buffer to copy.
         */
        inline void copy(VkCommandBuffer cbo, const Buffer& src) noexcept;

        /**
         * Records the command to copy a specific region of the buffer. For correct usage see the vulkan documentation
         * of <c>vkCmdCopyBuffer</c>.
         * @param cbo Specifies the command buffer in which the copy command is recorded.
         * @param src Specifies the buffer to copy.
         * @param region Specifies the copy region.
         */
        inline void copy_region(VkCommandBuffer cbo, const Buffer& src, const VkBufferCopy& region) noexcept;

        /**
         * Records the command to update the whole buffer. For correct usage see the vulkan documentation of
         * <c>vkCmdUpdateBuffer</c>.
         * @param cbo Specifies the command buffer in which the update command is recorded.
         * @param data Specifies the data to copy. Must point to memory which is at least as large as the buffer.
         * @note The data which is copied into the buffer is directly recorded into the command buffer. Using this
         * functionality is only recommended for small copy sizes (e.g. for small uniform buffers).
         */
        inline void update(VkCommandBuffer cbo, const void* data) noexcept;

        /**
         * Records the command to update a specific region of the buffer. For correct usage see the vulkan documentation
         * of <c>vkCmdUpdateBuffer</c>.
         * @param cbo Specifies the command buffer in which the update command is recorded.
         * @param data Specifies the data to copy. Must point to memory which is at least <c>size</c> bytes large.
         * @param offset Specifies the byte offset in the destination buffer.
         * @param size Specifies the number of bytes to update.
         * @note The data which is copied into the buffer is directly recorded into the command buffer. Using this
         * functionality is only recommended for small copy sizes (e.g. for small uniform buffers).
         */
        inline void update_region(VkCommandBuffer cbo, const void* data, VkDeviceSize offset, VkDeviceSize size) noexcept;

        /**
         * Records the command to fill the whole buffer with a single value. For correct usage see the vulkan
         * documentation of <c>vkCmdFillBuffer</c>. This command behaves like <c>memset</c>.
         * @param cbo Specifies the command buffer in which the fill command is recorded.
         * @param value Specifies the value with which the buffer is filled.
         */
        inline void fill(VkCommandBuffer cbo, uint32_t value) noexcept;

        /**
         * Records the command to fill a specific region of the buffer with a single value. For correct usage see the
         * vulkan documentation of <c>vkCmdFillBuffer</c>. This command behaves like <c>memset</c>.
         * @param cbo Specifies the command buffer in which the fill command is recorded.
         * @param value Specifies the value with which the buffer is filled.
         * @param offset Specifies the byte offset in the destination buffer.
         * @param size Specifies the number of bytes to fill.
         */
        inline void fill_region(VkCommandBuffer cbo, uint32_t value, VkDeviceSize offset, VkDeviceSize size) noexcept;

        // deleted
        Buffer(const Buffer&) = delete;
        Buffer& operator= (const Buffer&) = delete;

    private:
        static constexpr const char* BUFFER_CREATE_FAILED = "[vka::Buffer]: Failed to create buffer handle.";
        static constexpr const char* ALLOC_MEMORY_FAILED = "[vka::Buffer]: Failed to allocate memory.";
        static constexpr const char* BIND_MEMORY_FAILED = "[vka::Buffer]: Failed to bind memory to buffer.";
        static constexpr const char* MAP_MEMORY_FAILED = "[vka::Buffer]: Failed to map memory of buffer";

        unique_handle<BufferHandle> m_buffer;
        VkDeviceSize m_size;
        void* m_map;

        /// Unmaps the memory without resetting the status.
        constexpr void unmap_memory() const noexcept;

        /// Creates the buffer and its associated memory.
        static BufferHandle create_buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);
    };
}
