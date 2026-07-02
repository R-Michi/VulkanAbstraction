#pragma once

#include "window.h"

inline void vka::detail::window::destroy(Parent parent, const Handle& handle, const VkAllocationCallbacks* allocator)
{
    vkDestroySwapchainKHR(parent.device, handle.swapchain, allocator);
    vkDestroySurfaceKHR(parent.instance, handle.surface, allocator);
    glfwDestroyWindow(handle.window);
}