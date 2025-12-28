#pragma once

namespace vka::detail::buffer
{
    struct Handle
    {
        VkBuffer buffer;
        VkDeviceMemory memory;

        constexpr explicit operator bool() const { return memory != VK_NULL_HANDLE; }
    };

    /// Destroys the buffer and frees the memory.
    inline void destroy(VkDevice device, Handle handle, const VkAllocationCallbacks* allocator);
}