#pragma once

namespace vka::detail::window
{
    struct Parent
    {
        VkInstance instance;
        VkDevice device;
    };

    /// Defines the window handle which includes the GLFW window alongside with its context.
    struct Handle
    {
        GLFWwindow* window;
        VkSurfaceKHR surface;
        VkSwapchainKHR swapchain;

        explicit constexpr operator bool() const noexcept { return this->window != nullptr; }
    };

    /// Destroys the window handle.
    inline void destroy(Parent parent, const Handle& handle, const VkAllocationCallbacks* allocator);
}