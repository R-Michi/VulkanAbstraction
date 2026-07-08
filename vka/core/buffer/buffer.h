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
     * Structure specifying the parameters of a newly created buffer object. Parameters prefixed with <c>buffer</c> or
     * <c>pBuffer</c> correspond to the parameters of a
     * <a href="https://docs.vulkan.org/refpages/latest/refpages/source/VkBufferCreateInfo.html">VkBufferCreateInfo</a>.
     * - <c>pMemoryNext</c> specifies the <c>pNext</c> parameter of a
     * <a href="https://docs.vulkan.org/refpages/latest/refpages/source/VkMemoryAllocateInfo.html">
     * VkMemoryAllocateInfo</a>.
     * - <c>memoryType</c> specifies the type of memory where the buffer is allocated.
     */
    struct BufferCreateInfo
    {
        const void*             pBufferNext;
        VkBufferCreateFlags     bufferFlags;
        VkDeviceSize            bufferSize;
        VkBufferUsageFlags      bufferUsage;
        VkSharingMode           bufferSharingMode;
        uint32_t                bufferQueueFamilyIndexCount;
        const uint32_t*         bufferQueueFamilyIndices;
        const void*             pMemoryNext;
        VkMemoryPropertyFlags   memoryType;
    };

    /**
     * Abstraction to simplify the creation of buffers. Contains the vulkan <c>VkBuffer</c> and the corresponding
     * <c>VkDeviceMemory</c> handle.
     *
     * <b>Default initialization:</b>\n
     * The default constructor creates an <b>empty</b> buffer. This empty object is invalid and cannot perform any
     * actions (see below for a brief list of actions). Any member function returning a vulkan handle returns
     * <c>VK_NULL_HANDLE</c>. Querying the device pointer returns <c>0</c> (equivalent to <c>nullptr</c> / <c>NULL</c>).
     * Calling <c>size()</c> returns <c>0</c>. Calling <c>destroy()</c> does nothing.
     *
     * <b>Initialization:</b>\n
     * The initialization constructor creates a valid buffer that can perform any action, if no exception was thrown.
     *
     * <b>Copy behaviour:</b>\n
     * The copy constructor and operator are deleted. In order to copy a buffer you have to call <c>copy()</c> or
     * <c>copy_region()</c> which records a copy command for the buffer. This command must then be submitted via a
     * command buffer to a queue that supports <c>VK_QUEUE_TRANSFER_BIT</c>.
     *
     * <b>Moving behaviour:</b>\n
     * When calling the move constructor or operator, the moved object is invalidated and performing any operation on it
     * is unsafe. This may lead to undefined behaviour or even a crash. If an already valid object is replaced by a
     * move, the current object is destroyed.
     *
     * <b>Destroy behaviour:</b>\n
     * Destroys all vulkan handles and sets everything back to default values. After destroying the object is an
     * <b>empty</b> buffer.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>mapping</b> -- Invoked by <c>map()</c> maps the buffer to a memory region into which the host can write.
     * Calling <c>unmap()</c> unmaps the buffer again.
     * - <b>copy</b> -- Invoked by <c>copy()</c> or <c>copy_region()</c> copies a buffer.
     * - <b>update</b> -- Invoked by <c>update()</c> or <c>update_region()</c> directly copies memory into a buffer.
     * - <b>fill</b> -- Invoked by <c>fill()</c> or <c>fill_region()</c> fills the buffer with a single value.
     */
    class Buffer final
    {
        using Handle = detail::buffer::Handle;

    public:
        /// Creates an empty buffer. This buffer is invalid.
        constexpr Buffer() noexcept;

        /**
         * Create the buffer. The buffer is valid if no exception was thrown.
         * @param device Device with which the buffer is created.
         * @param properties Memory properties of the physical device.
         * @param create_info Create-info for the buffer.
         * @throw std::runtime_error Is thrown, if creating the buffer, allocating the memory or binding the memory
         * failed.
         */
        explicit Buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);

        /// Moves a buffer. The source buffer becomes invalidated and using to results in undefined behaviour.
        constexpr Buffer(Buffer&& src) noexcept;

        /// Unmaps the buffer if mapped.
        constexpr ~Buffer();

        /**
         * Moves a buffer. The source buffer becomes invalidated and using to results in undefined behaviour. An already
         * allocated buffer is destroyed.
         */
        constexpr Buffer& operator= (Buffer&& src) noexcept;

        /// @return Returns whether the buffer is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the allocated size in bytes.
        constexpr VkDeviceSize size() const noexcept;

        /// @return Returns the parent handle.
        constexpr VkDevice parent() const noexcept;

        /// @return Returns the vulkan <c>VkBuffer</c> handle.
        constexpr VkBuffer handle() const noexcept;

        /// @return Returns the device (GPU-side) pointer to the buffer.
        inline VkDeviceAddress device_ptr() const noexcept;

        /// Destroys the buffer. After destroying the buffer is empty and therefore invalid.
        constexpr void destroy() noexcept;

        /**
         * Maps the whole buffer.
         * @return Returns a pointer to the memory of the mapped buffer. If the buffer is invalid, the returned pointer
         * is <c>nullptr</c>.
         * @throw std::runtime_error Is thrown if mapping the buffer failed.
         */
        constexpr void* map();

        /**
         * Maps a specific region of the buffer.
         * @param offset Offset of the region to map.
         * @param size Size of the region to map.
         * @return Returns a pointer to the memory of the mapped buffer region. If the buffer is invalid, the returned
         * pointer is <c>nullptr</c>.
         * @throw std::runtime_error Is thrown if mapping the buffer failed.
         */
        constexpr void* map(VkDeviceSize offset, VkDeviceSize size);

        /// Unmaps the buffer. If the buffer is invalid, this function does nothing.
        constexpr void unmap() noexcept;

        /**
         * Records the command to copy the whole buffer. For correct usage see the vulkan documentation of
         * <a href="https://docs.vulkan.org/refpages/latest/refpages/source/vkCmdCopyBuffer.html">vkCmdCopyBuffer</a>.
         * @param cbo Command buffer in which the copy command is recorded.
         * @param src Buffer to copy.
         */
        inline void copy(VkCommandBuffer cbo, const Buffer& src) noexcept;

        /**
         * Records the command to copy a specific region of the buffer. For correct usage see the vulkan documentation
         * of <a href="https://docs.vulkan.org/refpages/latest/refpages/source/vkCmdCopyBuffer.html">
         * vkCmdCopyBuffer</a>.
         * @param cbo Command buffer in which the copy command is recorded.
         * @param src Buffer to copy.
         * @param region Region of the buffer to copy.
         */
        inline void copy_region(VkCommandBuffer cbo, const Buffer& src, const VkBufferCopy& region) noexcept;

        /**
         * Records the command to update the whole buffer. For correct usage see the vulkan documentation of
         * <a href="https://docs.vulkan.org/refpages/latest/refpages/source/vkCmdUpdateBuffer.html">
         * vkCmdUpdateBuffer</a>.
         * @param cbo Command buffer in which the update command is recorded.
         * @param data Data to copy. Must point to memory which is at least as large as the buffer.
         * @note The data which is copied into the buffer is directly recorded into the command buffer. Using this
         * functionality is only recommended for small copy sizes (e.g. for small uniform buffers).
         */
        inline void update(VkCommandBuffer cbo, const void* data) noexcept;

        /**
         * Records the command to update a specific region of the buffer. For correct usage see the vulkan documentation
         * of <a href="https://docs.vulkan.org/refpages/latest/refpages/source/vkCmdUpdateBuffer.html">
         * vkCmdUpdateBuffer</a>.
         * @param cbo Command buffer in which the update command is recorded.
         * @param data Data to copy. Must point to memory which is at least <c>size</c> bytes large.
         * @param offset Offset in bytes in the destination buffer.
         * @param size Number of bytes to update.
         * @note The data which is copied into the buffer is directly recorded into the command buffer. Using this
         * functionality is only recommended for small copy sizes (e.g. for small uniform buffers).
         */
        inline void update_region(VkCommandBuffer cbo, const void* data, VkDeviceSize offset, VkDeviceSize size) noexcept;

        /**
         * Records the command to fill the whole buffer with a single value. For correct usage see the vulkan
         * documentation of <a href="https://docs.vulkan.org/refpages/latest/refpages/source/vkCmdFillBuffer.html">
         * vkCmdFillBuffer</a>. This command behaves like <c>memset</c>.
         * @param cbo Command buffer in which the fill command is recorded.
         * @param value Value with which the buffer is filled.
         */
        inline void fill(VkCommandBuffer cbo, uint32_t value) noexcept;

        /**
         * Records the command to fill a specific region of the buffer with a single value. For correct usage see the
         * vulkan documentation of
         * <a href="https://docs.vulkan.org/refpages/latest/refpages/source/vkCmdFillBuffer.html">vkCmdFillBuffer</a>.
         * This command behaves like <c>memset</c>.
         * @param cbo Command buffer in which the fill command is recorded.
         * @param value Value with which the buffer is filled.
         * @param offset Offset in bytes in the destination buffer.
         * @param size Number of bytes to fill.
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

        unique_handle<Handle> m_buffer;
        VkDeviceSize m_size;
        void* m_map;

        /// Unmaps the memory without resetting the status.
        constexpr void unmap_memory() const noexcept;

        /// Creates the buffer and its associated memory.
        static unique_handle<Handle> create_buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info);
    };
}
