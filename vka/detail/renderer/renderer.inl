#pragma once

#include "renderer.h"

inline void vka::detail::renderer::destroy(VkDevice device, const Handle& handle, const VkAllocationCallbacks* allocator)
{
    for (uint32_t i = 0; i < handle.fif_count; i++)
    {
        vkDestroyFence(device, handle.fences[i], allocator);
        vkDestroySemaphore(device, handle.sem_acquire[i], allocator);
    }

    for (uint32_t i = 0; i < handle.image_count; i++)
        vkDestroySemaphore(device, handle.sem_render[i], allocator);

    delete[] handle.fences;
    delete[] handle.sem_acquire;
    delete[] handle.sem_render;
}