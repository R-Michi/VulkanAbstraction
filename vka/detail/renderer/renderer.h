#pragma once

namespace vka::detail::renderer
{
    /// Defines all handles for the renderer.
    struct Handle
    {
        const VkFence* fences;          // contains fif_count fences
        const VkSemaphore* sem_acquire; // contains fif_count semaphores
        const VkSemaphore* sem_render;  // contains image_count semaphores
        uint32_t image_count;
        uint32_t fif_count;

        explicit constexpr operator bool() const noexcept { return this->fences != nullptr; }
    };

    /// Destroys the renderer handle.
    inline void destroy(VkDevice device, const Handle& handle, const VkAllocationCallbacks* allocator);
}