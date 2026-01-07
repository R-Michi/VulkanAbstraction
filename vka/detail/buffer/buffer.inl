#pragma once

#include "buffer.h"

inline void vka::detail::buffer::destroy(VkDevice device, Handle handle, const VkAllocationCallbacks* allocator)
{
    vkFreeMemory(device, handle.memory, allocator);
    vkDestroyBuffer(device, handle.buffer, allocator);
}