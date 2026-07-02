#pragma once

namespace vka
{
    class Window
    {
        using Parent = detail::window::Parent;
        using Handle = detail::window::Handle;

    public:
    private:
        static constexpr const char* MSG_WINDOW_CREATE_FAILED       = "[vka::Window]: Failed to create window.";
        static constexpr const char* MSG_SURFACE_CREATE_FAILED      = "[vka::Window]: Failed to create surface.";
        static constexpr const char* MSG_SWAPCHAIN_CREATE_FAILED    = "[vka::Window]: Failed to create swapchain.";

        unique_handle<Handle> m_window;
    };
}