#pragma once

#include "attachment.h"

inline void vka::detail::attachment::destroy(VkDevice device, const Handle& handle, const VkAllocationCallbacks* allocator)
{
    vkDestroyImageView(device, handle.view, allocator);
    vkFreeMemory(device, handle.memory, allocator);
    vkDestroyImage(device, handle.image, allocator);
}