#pragma once

namespace vka::detail::buffer
{
    /// Defines the buffer handle which is used as a custom handle in <c>unique_handle</c>. Parent: <c>VkDevice</c>.
    struct Handle
    {
        VkBuffer buffer;
        VkDeviceMemory memory;

        explicit constexpr operator bool() const noexcept { return this->buffer != VK_NULL_HANDLE; }
    };

    /// Destroys the buffer and frees the memory.
    inline void destroy(VkDevice device, Handle handle, const VkAllocationCallbacks* allocator);
}