#pragma once

namespace vka::detail
{
    class unique_window
    {
    public:
        /// Default initialization. Initialized the managed window handle to <c>nullptr</c>.
        constexpr unique_window() noexcept;

        /**
         * Initialize with a handle.
         * @param handle Must either be <c>nullptr</c> or a valid window handle.
         */
        constexpr unique_window(GLFWwindow* handle) noexcept;

        /// Transfers the ownership of a window handle to a different <c>unique_window</c> object.
        constexpr unique_window(unique_window&& src) noexcept;

        /// Destroys the current window handle.
        constexpr ~unique_window();

        /**
         * Transfers the ownership of a window handle to a different <c>unique_window</c> object. A current valid window
         * handle is destroyed.
         */
        constexpr unique_window& operator= (unique_window&& src) noexcept;

        /// @return Returns <c>true</c> if the object is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the current GLFW <c>GLFWwindow*</c> handle.
        constexpr GLFWwindow* get() const noexcept;

        /// Destroys the current window handle and sets it to <c>nullptr</c>.
        constexpr void destroy() noexcept;

        // deleted:
        unique_window(const unique_window&) = delete;
        unique_window& operator= (const unique_window&) = delete;

    private:
        GLFWwindow* m_window;

        /// Destroys the handle but does not invalidate it.
        constexpr void destroy_handle() const noexcept;
    };
}