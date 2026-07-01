#pragma once

namespace vka::detail::attachment
{
    struct Handle
    {
        VkImage image;
        VkDeviceMemory memory;
        VkImageView view;

        explicit constexpr operator bool() const noexcept { return this->image != VK_NULL_HANDLE; }
    };

    /// Destroys the attachment image handles.
    inline void destroy(VkDevice device, const Handle& handle, const VkAllocationCallbacks* allocator);
}