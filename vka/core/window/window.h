#pragma once

namespace vka
{
    struct WindowFrame
    {
        uint32_t left;
        uint32_t top;
        uint32_t right;
        uint32_t bottom;
    };

    struct WindowContentScale
    {
        float x;
        float y;
    };

    struct WindowCursorPos
    {
        double x;
        double y;
    };

    class Window
    {
        using Parent = detail::window::Parent;
        using Handle = detail::window::Handle;

    public:
        explicit Window(VkDevice device, VkInstance instance, const WindowCreateInfo& create_info);

        /// @return Returns whether the window is valid.
        explicit constexpr operator bool() const noexcept;

        inline bool should_close() const noexcept;

        inline const char* title() const noexcept;

        inline VkOffset2D pos() const noexcept;

        inline VkExtent2D size() const noexcept;

        inline VkExtent2D framebuffer_size() const noexcept;

        inline WindowFrame frame_size() const noexcept;

        inline WindowContentScale content_scale() const noexcept;

        inline float opacity() const noexcept;

        inline GLFWmonitor* monitor() const noexcept;

        inline uint32_t attribute(uint32_t attribute) const noexcept;

        inline void* user_pointer() const noexcept;

        inline uint32_t input_mode(uint32_t mode) const noexcept;

        inline bool key_state(uint32_t key) const noexcept;

        inline bool mouse_button(uint32_t button) const noexcept;

        inline WindowCursorPos cursor_pos() const noexcept;

        const char* clipboard_string() const noexcept;

        /// @return Returns the instance with which the surface was created.
        constexpr VkInstance instance() const noexcept;

        /// @return Returns the device with which the swapchain was created.
        constexpr VkDevice device() const noexcept;

        /// @return Returns the GLFW <c>GLFWwindow*</c> handle.
        constexpr GLFWwindow* handle() const noexcept;

        /// @return Returns the vulkan <c>VkSurfaceKHR</c> handle.
        constexpr VkSurfaceKHR surface() const noexcept;

        /// @return Returns the vulkan <c>VkSwapchainKHR</c> handle.
        constexpr VkSwapchainKHR swapchain() const noexcept;

        inline void set_should_close(bool should_close) noexcept;

        inline void set_title(std::string_view title) noexcept;

        inline void set_icon(uint32_t count, GLFWimage* images);

        inline void set_pos(VkOffset2D pos) noexcept;

        inline void set_size(VkExtent2D size) noexcept;

        inline void set_size_limits(VkExtent2D min_size, VkExtent2D max_size) noexcept;

        inline void set_aspect_ratio(uint32_t numerator, uint32_t denominator) noexcept;

        inline void set_opacity(float opacity) noexcept;

        inline void set_monitor(GLFWmonitor* monitor, VkOffset2D pos, VkExtent2D size, uint32_t refresh_rate);

        inline void set_attribute(uint32_t attribute, uint32_t value) noexcept;

        inline void set_user_pointer(void* user_pointer) noexcept;

        inline void set_input_mode(uint32_t mode, uint32_t value) noexcept;

        inline void set_cursor_pos(WindowCursorPos pos) noexcept;

        inline void set_cursor(GLFWcursor* cursor) noexcept;

        inline void set_clipboard_string(std::string_view string) noexcept;

        inline void set_pos_callback(GLFWwindowposfun callback) noexcept;

        inline void set_size_callback(GLFWwindowsizefun callback) noexcept;

        inline void set_close_callback(GLFWwindowclosefun callback) noexcept;

        inline void set_refresh_callback(GLFWwindowrefreshfun callback) noexcept;

        inline void set_focus_callback(GLFWwindowfocusfun callback) noexcept;

        inline void set_iconify_callback(GLFWwindowiconifyfun callback) noexcept;

        inline void set_maximize_callback(GLFWwindowmaximizefun callback) noexcept;

        inline void set_framebuffer_size_callback(GLFWframebuffersizefun callback) noexcept;

        inline void set_content_scale_callback(GLFWwindowcontentscalefun callback) noexcept;

        inline void set_key_callback(GLFWkeyfun callback) noexcept;

        inline void set_char_callback(GLFWcharfun callback) noexcept;

        inline void set_char_mods_callback(GLFWcharmodsfun callback) noexcept;

        inline void set_mouse_button_callback(GLFWmousebuttonfun callback) noexcept;

        inline void set_cursor_pos_callback(GLFWcursorposfun callback) noexcept;

        inline void set_cursor_enter_callback(GLFWcursorenterfun callback) noexcept;

        inline void set_scroll_callback(GLFWscrollfun callback) noexcept;

        inline void set_drop_callback(GLFWdropfun callback) noexcept;

        inline void iconify() noexcept;

        inline void restore() noexcept;

        inline void maximize() noexcept;

        inline void show() noexcept;

        inline void hide() noexcept;

        inline void focus() noexcept;

        inline void request_attention() noexcept;

        inline void update();

        /// Destroys the window. After destroying the window is empty and therefore invalid.
        constexpr void destroy() noexcept;

        // default:
        Window() = default;
        Window(Window&&) = default;
        ~Window() = default;
        Window& operator= (Window&&) = default;
    private:
        static constexpr const char* MSG_WINDOW_CREATE_FAILED       = "[vka::Window]: Failed to create window.";
        static constexpr const char* MSG_SURFACE_CREATE_FAILED      = "[vka::Window]: Failed to create surface.";
        static constexpr const char* MSG_SWAPCHAIN_CREATE_FAILED    = "[vka::Window]: Failed to create swapchain.";

        unique_handle<Handle> m_window;
    };
}