// ReSharper disable CppMemberFunctionMayBeConst
#pragma once

#include "window.h"

constexpr vka::Window::Window() noexcept :
    m_callbacks{},
    m_user_ptr(nullptr)
{}

constexpr vka::Window::Window(Window&& src) noexcept :
    m_window(std::move(src.m_window)),
    m_surface(std::move(src.m_surface)),
    m_swapchain(std::move(src.m_swapchain)),
    m_images(std::move(src.m_images)),
    m_callbacks(src.m_callbacks),
    m_user_ptr(src.m_user_ptr)
{
    src.m_callbacks = {};
    src.m_user_ptr = nullptr;
}

constexpr vka::Window& vka::Window::operator= (Window&& src) noexcept
{
    this->m_window = std::move(src.m_window);
    this->m_surface = std::move(src.m_surface);
    this->m_swapchain = std::move(src.m_swapchain);
    this->m_images = std::move(src.m_images);
    this->m_callbacks = src.m_callbacks;
    this->m_user_ptr = src.m_user_ptr;
    src.m_callbacks = {};
    src.m_user_ptr = nullptr;
    return *this;
}

constexpr vka::Window::operator bool() const noexcept
{
    return (bool)this->m_swapchain;
}

inline VkImage vka::Window::operator[] (uint32_t idx) const noexcept
{
    return this->m_images[idx];
}

inline bool vka::Window::should_close() const noexcept
{
    return (bool)glfwWindowShouldClose(this->m_window.get());
}

inline const char* vka::Window::title() const noexcept
{
    return glfwGetWindowTitle(this->m_window.get());
}

inline VkOffset2D vka::Window::pos() const noexcept
{
    int x, y;
    glfwGetWindowPos(this->m_window.get(), &x, &y);
    return { x, y };
}

inline VkExtent2D vka::Window::size() const noexcept
{
    int width, height;
    glfwGetWindowSize(this->m_window.get(), &width, &height);
    return { (uint32_t)width, (uint32_t)height };
}

inline VkExtent2D vka::Window::surface_size() const noexcept
{
    int width, height;
    glfwGetFramebufferSize(this->m_window.get(), &width, &height);
    return { (uint32_t)width, (uint32_t)height };
}

inline vka::WindowFrame vka::Window::frame_size() const noexcept
{
    int left, top, right, bottom;
    glfwGetWindowFrameSize(this->m_window.get(), &left, &top, &right, &bottom);
    return { (uint32_t)left, (uint32_t)top, (uint32_t)right, (uint32_t)bottom };
}

inline vka::WindowContentScale vka::Window::content_scale() const noexcept
{
    float x, y;
    glfwGetWindowContentScale(this->m_window.get(), &x, &y);
    return { x, y };
}

inline float vka::Window::opacity() const noexcept
{
    return glfwGetWindowOpacity(this->m_window.get());
}

inline GLFWmonitor* vka::Window::monitor() const noexcept
{
    return glfwGetWindowMonitor(this->m_window.get());
}

inline uint32_t vka::Window::attribute(uint32_t attribute) const noexcept
{
    return glfwGetWindowAttrib(this->m_window.get(), attribute);
}

inline void* vka::Window::user_pointer() const noexcept
{
    return this->m_user_ptr;
}

inline uint32_t vka::Window::input_mode(uint32_t mode) const noexcept
{
    return glfwGetInputMode(this->m_window.get(), mode);
}

inline bool vka::Window::key_state(uint32_t key) const noexcept
{
    return glfwGetKey(this->m_window.get(), key) == GLFW_PRESS;
}

inline bool vka::Window::mouse_button(uint32_t button) const noexcept
{
    return glfwGetMouseButton(this->m_window.get(), button) == GLFW_PRESS;
}

inline vka::WindowCursorPos vka::Window::cursor_pos() const noexcept
{
    double x, y;
    glfwGetCursorPos(this->m_window.get(), &x, &y);
    return { x, y };
}

inline const char* vka::Window::clipboard_string() const noexcept
{
    return glfwGetClipboardString(this->m_window.get());
}

constexpr uint32_t vka::Window::image_count() const noexcept
{
    return this->m_images.size();
}

constexpr VkInstance vka::Window::instance() const noexcept
{
    return this->m_surface.parent();
}

constexpr VkDevice vka::Window::device() const noexcept
{
    return this->m_swapchain.parent();
}

constexpr GLFWwindow* vka::Window::handle() const noexcept
{
    return this->m_window.get();
}

constexpr VkSurfaceKHR vka::Window::surface() const noexcept
{
    return this->m_surface.get();
}

constexpr VkSwapchainKHR vka::Window::swapchain() const noexcept
{
    return this->m_swapchain.get();
}

inline VkImage vka::Window::image(uint32_t idx) const noexcept
{
    return this->m_images.at(idx);
}

inline void vka::Window::set_should_close(bool should_close) noexcept
{
    glfwSetWindowShouldClose(this->m_window.get(), should_close);
}

inline void vka::Window::set_title(std::string_view title) noexcept
{
    glfwSetWindowTitle(this->m_window.get(), title.data());
}

inline void vka::Window::set_icon(uint32_t count, const GLFWimage* images) noexcept
{
    glfwSetWindowIcon(this->m_window.get(), count, images);
}

inline void vka::Window::set_pos(VkOffset2D pos) noexcept
{
    glfwSetWindowPos(this->m_window.get(), pos.x, pos.y);
}

inline void vka::Window::set_size(VkExtent2D size) noexcept
{
    glfwSetWindowSize(this->m_window.get(), size.width, size.height);
}

inline void vka::Window::set_size_limits(VkExtent2D min_size, VkExtent2D max_size) noexcept
{
    glfwSetWindowSizeLimits(this->m_window.get(), min_size.width, min_size.height, max_size.width, max_size.height);
}

inline void vka::Window::set_aspect_ratio(uint32_t numerator, uint32_t denominator) noexcept
{
    glfwSetWindowAspectRatio(this->m_window.get(), numerator, denominator);
}

inline void vka::Window::set_opacity(float opacity) noexcept
{
    glfwSetWindowOpacity(this->m_window.get(), opacity);
}

inline void vka::Window::set_monitor(GLFWmonitor* monitor, VkOffset2D pos, VkExtent2D size, uint32_t refresh_rate) noexcept
{
    glfwSetWindowMonitor(this->m_window.get(), monitor, pos.x, pos.y, size.width, size.height, refresh_rate);
}

inline void vka::Window::set_attribute(uint32_t attribute, uint32_t value) noexcept
{
    glfwSetWindowAttrib(this->m_window.get(), attribute, value);
}

inline void vka::Window::set_user_pointer(void* user_pointer) noexcept
{
    if (this->operator bool())
        this->m_user_ptr = user_pointer;
}

inline void vka::Window::set_input_mode(uint32_t mode, uint32_t value) noexcept
{
    glfwSetInputMode(this->m_window.get(), mode, value);
}

inline void vka::Window::set_cursor_pos(WindowCursorPos pos) noexcept
{
    glfwSetCursorPos(this->m_window.get(), pos.x, pos.y);
}

inline void vka::Window::set_cursor(GLFWcursor* cursor) noexcept
{
    glfwSetCursor(this->m_window.get(), cursor);
}

inline void vka::Window::set_clipboard_string(std::string_view string) noexcept
{
    glfwSetClipboardString(this->m_window.get(), string.data());
}

inline void vka::Window::set_pos_callback(WindowPosCallback callback) noexcept
{
    this->m_callbacks.pos = callback;
    glfwSetWindowPosCallback(this->m_window.get(), pos_callback);
}

inline void vka::Window::set_size_callback(WindowSizeCallback callback) noexcept
{
    this->m_callbacks.size = callback;
    glfwSetWindowSizeCallback(this->m_window.get(), size_callback);
}

inline void vka::Window::set_close_callback(WindowCloseCallback callback) noexcept
{
    this->m_callbacks.close = callback;
    glfwSetWindowCloseCallback(this->m_window.get(), close_callback);
}

inline void vka::Window::set_refresh_callback(WindowRefreshCallback callback) noexcept
{
    this->m_callbacks.refresh = callback;
    glfwSetWindowRefreshCallback(this->m_window.get(), refresh_callback);
}

inline void vka::Window::set_focus_callback(WindowFocusCallback callback) noexcept
{
    this->m_callbacks.focus = callback;
    glfwSetWindowFocusCallback(this->m_window.get(), focus_callback);
}

inline void vka::Window::set_iconify_callback(WindowIconifyCallback callback) noexcept
{
    this->m_callbacks.iconify = callback;
    glfwSetWindowIconifyCallback(this->m_window.get(), iconify_callback);
}

inline void vka::Window::set_maximize_callback(WindowMaximizeCallback callback) noexcept
{
    this->m_callbacks.maximize = callback;
    glfwSetWindowMaximizeCallback(this->m_window.get(), maximize_callback);
}

inline void vka::Window::set_surface_size_callback(WindowSurfaceSizeCallback callback) noexcept
{
    this->m_callbacks.surface_size = callback;
    glfwSetFramebufferSizeCallback(this->m_window.get(), framebuffer_size_callback);
}

inline void vka::Window::set_content_scale_callback(WindowContentScaleCallback callback) noexcept
{
    this->m_callbacks.content_scale = callback;
    glfwSetWindowContentScaleCallback(this->m_window.get(), content_scale_callback);
}

inline void vka::Window::set_key_callback(WindowKeyCallback callback) noexcept
{
    this->m_callbacks.key = callback;
    glfwSetKeyCallback(this->m_window.get(), key_callback);
}

inline void vka::Window::set_char_callback(WindowCharCallback callback) noexcept
{
    this->m_callbacks.character = callback;
    glfwSetCharCallback(this->m_window.get(), char_callback);
}

inline void vka::Window::set_char_mods_callback(WindowCharModsCallback callback) noexcept
{
    this->m_callbacks.char_mods = callback;
    glfwSetCharModsCallback(this->m_window.get(), char_mods_callback);
}

inline void vka::Window::set_mouse_button_callback(WindowMouseButtonCallback callback) noexcept
{
    this->m_callbacks.mouse_button = callback;
    glfwSetMouseButtonCallback(this->m_window.get(), mouse_button_callback);
}

inline void vka::Window::set_cursor_pos_callback(WindowCursorPosCallback callback) noexcept
{
    this->m_callbacks.cursor_pos = callback;
    glfwSetCursorPosCallback(this->m_window.get(), cursor_position_callback);
}

inline void vka::Window::set_cursor_enter_callback(WindowCursorEnterCallback callback) noexcept
{
    this->m_callbacks.cursor_enter = callback;
    glfwSetCursorEnterCallback(this->m_window.get(), cursor_enter_callback);
}

inline void vka::Window::set_scroll_callback(WindowScrollCallback callback) noexcept
{
    this->m_callbacks.scroll = callback;
    glfwSetScrollCallback(this->m_window.get(), scroll_callback);
}

inline void vka::Window::set_drop_callback(WindowDropCallback callback) noexcept
{
    this->m_callbacks.drop = callback;
    glfwSetDropCallback(this->m_window.get(), drop_callback);
}

inline void vka::Window::iconify() noexcept
{
    glfwIconifyWindow(this->m_window.get());
}

inline void vka::Window::restore() noexcept
{
    glfwRestoreWindow(this->m_window.get());
}

inline void vka::Window::maximize() noexcept
{
    glfwMaximizeWindow(this->m_window.get());
}

inline void vka::Window::show() noexcept
{
    glfwShowWindow(this->m_window.get());
}

inline void vka::Window::hide() noexcept
{
    glfwHideWindow(this->m_window.get());
}

inline void vka::Window::focus() noexcept
{
    glfwFocusWindow(this->m_window.get());
}

inline void vka::Window::request_attention() noexcept
{
    glfwRequestWindowAttention(this->m_window.get());
}

constexpr void vka::Window::destroy() noexcept
{
    this->m_swapchain.destroy();
    this->m_surface.destroy();
    this->m_window.destroy();
    this->m_images.clear();
    this->m_user_ptr = nullptr;
    this->m_callbacks = {};
}