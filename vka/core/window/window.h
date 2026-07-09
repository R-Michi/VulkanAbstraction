#pragma once

namespace vka
{
    class Window;

    /**
     * Structure specifying the parameters of a newly created window object. The first 4 parameters correspond to the
     * parameters of <a href="https://www.glfw.org/docs/3.3/group__window.html#ga3555a418df92ad53f917597fe2f64aeb">
     * glfwCreateWindow</a>. The rest of the parameters correspond to the parameters of
     * <a href="https://docs.vulkan.org/refpages/latest/refpages/source/VkSwapchainCreateInfoKHR.html">
     * VkSwapchainCreateInfoKHR</a>.
     * - <b>physicalDevice</b> -- Physical device with which to check, if the specified presentation mode is supported.
     * - <b>presentMode</b> -- Presentation mode the swapchain will use. If the specified presentation mode is not
     * supported, the presentation mode <c>VK_PRESENT_MODE_FIFO_KHR</c> will be used instead. The Vulkan API guarantees
     * that <c>VK_PRESENT_MODE_FIFO_KHR</c> exists, if the extension <c>VK_KHR_swapchain</c> is supported.
     */
    struct WindowCreateInfo
    {
        VkExtent2D                      size;
        const char*                     title;
        GLFWmonitor*                    monitor;
        GLFWwindow*                     share;
        VkSwapchainCreateFlagsKHR       flags;
        VkPhysicalDevice                physicalDevice;
        uint32_t                        minImageCount;
        VkFormat                        imageFormat;
        VkColorSpaceKHR                 imageColorSpace;
        uint32_t                        imageArrayLayers;
        VkImageUsageFlags               imageUsage;
        VkSharingMode                   imageSharingMode;
        uint32_t                        queueFamilyIndexCount;
        const uint32_t*                 pQueueFamilyIndices;
        VkSurfaceTransformFlagBitsKHR   preTransform;
        VkCompositeAlphaFlagBitsKHR     compositeAlpha;
        VkPresentModeKHR                presentMode;
        VkBool32                        clipped;
    };

    /**
     * Structure specifying the parameters of the new swapchain when updating the window.
     * The parameters correspond to the parameters of
     * <a href="https://docs.vulkan.org/refpages/latest/refpages/source/VkSwapchainCreateInfoKHR.html">
     * VkSwapchainCreateInfoKHR</a>.
     * - <b>physicalDevice</b> -- Physical device with which to check, if the specified presentation mode is supported.
     * - <b>presentMode</b> -- Presentation mode the swapchain will use. If the specified presentation mode is not
     * supported, the presentation mode <c>VK_PRESENT_MODE_FIFO_KHR</c> will be used instead. The Vulkan API guarantees
     * that <c>VK_PRESENT_MODE_FIFO_KHR</c> exists, if the extension <c>VK_KHR_swapchain</c> is supported.
     */
    struct WindowUpdateInfo
    {
        VkSwapchainCreateFlagsKHR       flags;
        VkPhysicalDevice                physicalDevice;
        uint32_t                        minImageCount;
        VkFormat                        imageFormat;
        VkColorSpaceKHR                 imageColorSpace;
        uint32_t                        imageArrayLayers;
        VkImageUsageFlags               imageUsage;
        VkSharingMode                   imageSharingMode;
        uint32_t                        queueFamilyIndexCount;
        const uint32_t*                 pQueueFamilyIndices;
        VkSurfaceTransformFlagBitsKHR   preTransform;
        VkCompositeAlphaFlagBitsKHR     compositeAlpha;
        VkPresentModeKHR                presentMode;
        VkBool32                        clipped;
    };

    /**
     * Defines the window frame size according to
     * <a href="https://www.glfw.org/docs/3.3/group__window.html#ga1a9fd382058c53101b21cf211898f1f1">
     * glfwGetWindowFrameSize</a>
     */
    struct WindowFrame
    {
        uint32_t left;
        uint32_t top;
        uint32_t right;
        uint32_t bottom;
    };

    /// Defines the <c>{X, Y}</c> scale of the window contents.
    struct WindowContentScale
    {
        float x;
        float y;
    };

    /// Defines the <c>{X, Y}</c> cursor position relative to the window.
    struct WindowCursorPos
    {
        double x;
        double y;
    };

    /// Defines the <c>{X, Y}</c> scroll offset.
    struct WindowScrollOffset
    {
        double x;
        double y;
    };

    /// Defines the window callbacks. For the signature see the description of the corresponding setter functions.
    using WindowPosCallback             = void(*)(Window& window, VkOffset2D pos);
    using WindowSizeCallback            = void(*)(Window& window, VkExtent2D size);
    using WindowCloseCallback           = void(*)(Window& window);
    using WindowRefreshCallback         = void(*)(Window& window);
    using WindowFocusCallback           = void(*)(Window& window, bool focused);
    using WindowIconifyCallback         = void(*)(Window& window, bool iconified);
    using WindowMaximizeCallback        = void(*)(Window& window, bool maximized);
    using WindowSurfaceSizeCallback     = void(*)(Window& window, VkExtent2D size);
    using WindowContentScaleCallback    = void(*)(Window& window, WindowContentScale scale);
    using WindowKeyCallback             = void(*)(Window& window, uint32_t key, uint32_t scancode, uint32_t action, uint32_t mods);
    using WindowCharCallback            = void(*)(Window& window, uint32_t codepoint);
    using WindowCharModsCallback        = void(*)(Window& window, uint32_t codepoint, uint32_t mods);
    using WindowMouseButtonCallback     = void(*)(Window& window, uint32_t button, uint32_t action, uint32_t mods);
    using WindowCursorPosCallback       = void(*)(Window& window, WindowCursorPos pos);
    using WindowCursorEnterCallback     = void(*)(Window& window, bool entered);
    using WindowScrollCallback          = void(*)(Window& window, WindowScrollOffset offset);
    using WindowDropCallback            = void(*)(Window& window, uint32_t path_count, const char* paths[]);

    /// Structure containing a list of all callbacks. For internal use only!
    struct WindowCallbacks
    {
        WindowPosCallback pos;
        WindowSizeCallback size;
        WindowCloseCallback close;
        WindowRefreshCallback refresh;
        WindowFocusCallback focus;
        WindowIconifyCallback iconify;
        WindowMaximizeCallback maximize;
        WindowSurfaceSizeCallback surface_size;
        WindowContentScaleCallback content_scale;
        WindowKeyCallback key;
        WindowCharCallback character;
        WindowCharModsCallback char_mods;
        WindowMouseButtonCallback mouse_button;
        WindowCursorPosCallback cursor_pos;
        WindowCursorEnterCallback cursor_enter;
        WindowScrollCallback scroll;
        WindowDropCallback drop;
    };

    /**
     * Abstraction to simplify the creation of windows and the associated context. Contains the GLFW <c>GLFWwindow*</c>
     * handle and the associated vulkan context. The context consists of the vulkan <c>VkSurfaceKHR</c> and
     * <c>VkSwapchainKHR</c> handle.
     *
     * <b>Default initialization:</b>\n
     * The default constructor creates an <b>empty</b> window.  This empty object is invalid and cannot perform any
     * actions (see below for a brief list of actions). Any member returning a GLFW handle returns <c>nullptr</c>. Any
     * member function returning a vulkan handle returns <c>VK_NULL_HANDLE</c>. Invoking any function that calls a GLFW
     * function is undefined behaviour and may result in a crash. Specified callbacks on empty objects no nothing.
     * Calling <c>destroy()</c> does nothing.
     *
     * <b>Initialization:</b>\n
     * The initialization constructor creates a valid window that can perform any action, if no exception was thrown.
     *
     *  <b>Copy behaviour:</b>\n
     * The copy constructor and operator are deleted.
     *
     * <b>Moving behaviour:</b>\n
     * When calling the move constructor or operator, the moved object is invalidated and performing any operation on it
     * is unsafe. This may lead to undefined behaviour or even a crash. If an already valid object is replaced by a
     * move, the current object is destroyed.
     *
     * <b>Destroy behaviour:</b>\n
     * Destroys all GLFW and vulkan handles and sets everything back to default values. After destroying the object is
     * an <b>empty</b> window.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * Functions that call a GLFW function internally underlie the threading specification of that GLFW function. All
     * other functions and operators can be called from any thread. However, if you use this class across multiple
     * threads, actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>iconifying</b> -- Invoked by <c>iconify()</c>. See the description of this function.
     * - <b>restoring</b> -- Invoked by <c>restore()</c>. See the description of this function.
     * - <b>maximizing</b> -- Invoked by <c>maximize()</c>. See the description of this function.
     * - <b>showing</b> -- Invoked by <c>show()</c>. See the description of this function.
     * - <b>hiding</b> -- Invoked by <c>hide()</c>. See the description of this function.
     * - <b>focusing</b> -- Invoked by <c>focus()</c>. See the description of this function.
     * - <b>requesting attention</b> -- Invoked by <c>request_attention()</c>. See the description of this function.
     * - <b>updating</b> -- Invoked by <c>update()</c> updates the internal swapchain.
     */
    class Window final
    {
    public:
        /// Creates an empty window. This window is invalid.
        constexpr Window() noexcept;

        /**
         * Creates the window and its associated context. The context includes the window surface and the swapchain.
         * The window and the context are valid if no exception was thrown.
         * @param instance Instance with which the window surface is created.
         * @param device Device with which the swapchain is created.
         * @param create_info Create-info for the window and its associated context.
         * @throw std::runtime_error Is thrown, if creating the window, surface or swapchain failed.
         */
        explicit Window(VkInstance instance, VkDevice device, const WindowCreateInfo& create_info);

        /// Moves a window. The source window becomes invalidated and using to results in undefined behaviour.
        constexpr Window(Window&& src) noexcept;

        /**
         * Moves a window. The source window becomes invalidated and using to results in undefined behaviour. An already
         * created window is destroyed.
         */
        constexpr Window& operator= (Window&& src) noexcept;

        /**
         * No range check is performed on the index.
         * @return Returns the vulkan <c>VkImage</c> handle of the swapchain at the specified index.
         */
        inline VkImage operator[] (uint32_t idx) const noexcept;

        /// @return Returns whether the window is valid.
        explicit constexpr operator bool() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5">
         * glfwWindowShouldClose</a> on the current window.
         */
        inline bool should_close() const noexcept;

        /**
         * Calls <c>glfwGetWindowTitle</c>.
         * @return Returns the title of the window.
         */
        inline const char* title() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga73cb526c000876fd8ddf571570fdb634">
         * glfwGetWindowPos</a> on the current window.
         */
        inline VkOffset2D pos() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#gaeea7cbc03373a41fb51cfbf9f2a5d4c6">
         * glfwGetWindowSize</a> on the current window.
         */
        inline VkExtent2D size() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga0e2637a4161afb283f5300c7f94785c9">
         * glfwGetFramebufferSize</a> on the current window. This is equivalent to the surface size queried from
         * <c>VkSurfaceCapabilitiesKHR::currentExtent</c> without the special value <c>{0xFFFFFFFF, 0xFFFFFFFF}</c>.
         */
        inline VkExtent2D surface_size() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga1a9fd382058c53101b21cf211898f1f1">
         * glfwGetWindowFrameSize</a> on the current window.
         */
        inline WindowFrame frame_size() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#gaf5d31de9c19c4f994facea64d2b3106c">
         * glfwGetWindowContentScale</a> on the current window.
         */
        inline WindowContentScale content_scale() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#gad09f0bd7a6307c4533b7061828480a84">
         * glfwGetWindowOpacity</a> on the current window.
         */
        inline float opacity() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga4d766499ac02c60f02221a9dfab87299">
         * glfwGetWindowMonitor</a> on the current window.
         */
        inline GLFWmonitor* monitor() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#gacccb29947ea4b16860ebef42c2cb9337">
         * glfwGetWindowAttrib</a> on the current window.
         */
        inline uint32_t attribute(uint32_t attribute) const noexcept;

        /**
         * @return Returns the user pointer that was set for this window. If no user pointer was set or the window is
         * invalid, <c>nullptr</c> is returned.
         */
        inline void* user_pointer() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/latest/group__input.html#gaf5b859dbe19bdf434e42695ea45cc5f4">
         * glfwGetInputMode</a> on the current window.
         */
        inline uint32_t input_mode(uint32_t mode) const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/latest/group__input.html#gadd341da06bc8d418b4dc3a3518af9ad2">
         * glfwGetKey</a> on the current window.
         * @return Returns whether the key was pressed.
         */
        inline bool key_state(uint32_t key) const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/latest/group__input.html#gac1473feacb5996c01a7a5a33b5066704">
         * glfwGetMouseButton</a> on the current window.
         * @return Returns whether the mouse button was pressed.
         */
        inline bool mouse_button(uint32_t button) const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/latest/group__input.html#ga01d37b6c40133676b9cea60ca1d7c0cc">
         * glfwGetCursorPos</a> on the current window.
         */
        inline WindowCursorPos cursor_pos() const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/latest/group__input.html#ga71a5b20808ea92193d65c21b82580355">
         * glfwGetClipboardString</a> on the current window.
         */
        inline const char* clipboard_string() const noexcept;

        /// @return Returns the number of created swapchain images.
        constexpr uint32_t image_count() const noexcept;

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

        /**
         * Performs a range check on the index.
         * @return Returns the vulkan <c>VkImage</c> handle of the swapchain at the specified index.
         */
        inline VkImage image(uint32_t idx) const noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga49c449dde2a6f87d996f4daaa09d6708">
         * glfwSetWindowShouldClose</a> on the current window.
         */
        inline void set_should_close(bool should_close) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga5d877f09e968cef7a360b513306f17ff">
         * glfwSetWindowTitle</a> on the current window.
         */
        inline void set_title(std::string_view title) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#gadd7ccd39fe7a7d1f0904666ae5932dc5">
         * glfwSetWindowIcon</a> on the current window.
         */
        inline void set_icon(uint32_t count, const GLFWimage* images) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga1abb6d690e8c88e0c8cd1751356dbca8">
         * glfwSetWindowPos</a> on the current window.
         */
        inline void set_pos(VkOffset2D pos) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga371911f12c74c504dd8d47d832d095cb">
         * glfwSetWindowSize</a> on the current window.
         */
        inline void set_size(VkExtent2D size) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#gac314fa6cec7d2d307be9963e2709cc90">
         * glfwSetWindowSizeLimits</a> on the current window.
         */
        inline void set_size_limits(VkExtent2D min_size, VkExtent2D max_size) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga72ac8cb1ee2e312a878b55153d81b937">
         * glfwSetWindowAspectRatio</a> on the current window.
         */
        inline void set_aspect_ratio(uint32_t numerator, uint32_t denominator) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#gac31caeb3d1088831b13d2c8a156802e9">
         * glfwSetWindowOpacity</a> on the current window.
         */
        inline void set_opacity(float opacity) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga81c76c418af80a1cce7055bccb0ae0a7">
         * glfwSetWindowMonitor</a> on the current window.
         */
        inline void set_monitor(GLFWmonitor* monitor, VkOffset2D pos, VkExtent2D size, uint32_t refresh_rate) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#gace2afda29b4116ec012e410a6819033e">
         * glfwSetWindowAttrib</a> on the current window.
         */
        inline void set_attribute(uint32_t attribute, uint32_t value) noexcept;

        /**
         * Sets the user pointer for this window.
         * @param user_pointer Pointer pointing to some user-defined data.
         */
        inline void set_user_pointer(void* user_pointer) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/latest/group__input.html#gaa92336e173da9c8834558b54ee80563b">
         * glfwSetInputMode</a> on the current window.
         */
        inline void set_input_mode(uint32_t mode, uint32_t value) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/latest/group__input.html#ga04b03af936d906ca123c8f4ee08b39e7">
         * glfwSetCursorPos</a> on the current window.
         */
        inline void set_cursor_pos(WindowCursorPos pos) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/latest/group__input.html#gad3b4f38c8d5dae036bc8fa959e18343e">
         * glfwSetCursor</a> on the current window.
         */
        inline void set_cursor(GLFWcursor* cursor) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/latest/group__input.html#gaba1f022c5eb07dfac421df34cdcd31dd">
         * glfwSetClipboardString</a> on the current window.
         */
        inline void set_clipboard_string(std::string_view string) noexcept;

        /**
         * Sets the position callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, VkOffset2D pos)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/3.3/group__window.html#ga08bdfbba88934f9c4f92fd757979ac74">
         * glfwSetWindowPosCallback</a>
         * @param callback Callback function.
         */
        inline void set_pos_callback(WindowPosCallback callback) noexcept;

        /**
         * Sets the size callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, VkExtent2D size)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/3.3/group__window.html#gad91b8b047a0c4c6033c38853864c34f8">
         * glfwSetWindowSizeCallback</a>
         * @param callback Callback function.
         */
        inline void set_size_callback(WindowSizeCallback callback) noexcept;

        /**
         * Sets the close callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/3.3/group__window.html#gada646d775a7776a95ac000cfc1885331">
         * glfwSetWindowCloseCallback</a>
         * @param callback Callback function.
         */
        inline void set_close_callback(WindowCloseCallback callback) noexcept;

        /**
         * Sets the refresh callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/3.3/group__window.html#ga1c5c7eb889c33c7f4d10dd35b327654e">
         * glfwSetWindowRefreshCallback</a>
         * @param callback Callback function.
         */
        inline void set_refresh_callback(WindowRefreshCallback callback) noexcept;

        /**
         * Sets the focus callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, bool focused)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/3.3/group__window.html#gac2d83c4a10f071baf841f6730528e66c">
         * glfwSetWindowFocusCallback</a>
         * @param callback Callback function.
         */
        inline void set_focus_callback(WindowFocusCallback callback) noexcept;

        /**
         * Sets the iconify callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, bool iconified)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/3.3/group__window.html#gac793e9efd255567b5fb8b445052cfd3e">
         * glfwSetWindowIconifyCallback</a>
         * @param callback Callback function.
         */
        inline void set_iconify_callback(WindowIconifyCallback callback) noexcept;

        /**
         * Sets the maximize callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, bool maximized)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/3.3/group__window.html#gacbe64c339fbd94885e62145563b6dc93">
         * glfwSetWindowMaximizeCallback</a>
         * @param callback Callback function.
         */
        inline void set_maximize_callback(WindowMaximizeCallback callback) noexcept;

        /**
         * Sets the surface size callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, VkExtent2D size)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/3.3/group__window.html#gab3fb7c3366577daef18c0023e2a8591f">
         * glfwSetFramebufferSizeCallback</a>
         * @param callback Callback function.
         */
        inline void set_surface_size_callback(WindowSurfaceSizeCallback callback) noexcept;

        /**
         * Sets the content scale callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, WindowContentScale scale)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/3.3/group__window.html#gaf2832ebb5aa6c252a2d261de002c92d6">
         * glfwSetWindowContentScaleCallback</a>
         * @param callback Callback function.
         */
        inline void set_content_scale_callback(WindowContentScaleCallback callback) noexcept;

        /**
         * Sets the key callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, uint32_t key, uint32_t scancode, uint32_t action, uint32_t mods)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/latest/group__input.html#ga1caf18159767e761185e49a3be019f8d">
         * glfwSetKeyCallback</a>
         * @param callback Callback function.
         */
        inline void set_key_callback(WindowKeyCallback callback) noexcept;

        /**
         * Sets the character callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, uint32_t codepoint)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/latest/group__input.html#gab25c4a220fd8f5717718dbc487828996">
         * glfwSetCharCallback</a>
         * @param callback Callback function.
         */
        inline void set_char_callback(WindowCharCallback callback) noexcept;

        /**
         * Sets the character modifier callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, uint32_t codepoint, uint32_t mods)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/latest/group__input.html#ga0b7f4ad13c2b17435ff13b6dcfb4e43c">
         * glfwSetCharModsCallback</a>
         * @param callback Callback function.
         */
        inline void set_char_mods_callback(WindowCharModsCallback callback) noexcept;

        /**
         * Sets the mouse button callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, uint32_t button, uint32_t action, uint32_t mods)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/latest/group__input.html#ga6ab84420974d812bee700e45284a723c">
         * glfwSetMouseButtonCallback</a>
         * @param callback Callback function.
         */
        inline void set_mouse_button_callback(WindowMouseButtonCallback callback) noexcept;

        /**
         * Sets the cursor position callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, WindowCursorPos pos)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/latest/group__input.html#gac1f879ab7435d54d4d79bb469fe225d7">
         * glfwSetCursorPosCallback</a>
         * @param callback Callback function.
         */
        inline void set_cursor_pos_callback(WindowCursorPosCallback callback) noexcept;

        /**
         * Sets the cursor boundary crossing callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, bool entered)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/latest/group__input.html#gad27f8ad0142c038a281466c0966817d8">
         * glfwSetCursorEnterCallback</a>
         * @param callback Callback function.
         */
        inline void set_cursor_enter_callback(WindowCursorEnterCallback callback) noexcept;

        /**
         * Sets the scroll callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, WindowScrollOffset offset)
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/latest/group__input.html#ga571e45a030ae4061f746ed56cb76aede">
         * glfwSetScrollCallback</a>
         * @param callback Callback function.
         */
        inline void set_scroll_callback(WindowScrollCallback callback) noexcept;

        /**
         * Sets the path drop callback for this window.\n
         * Function signature:
         * @code
         * void function_name(Window& window, uint32_t path_count, const char* paths[])
         * @endcode
         * Also see: <a href="https://www.glfw.org/docs/latest/group__input.html#gab773f0ee0a07cff77a210cea40bc1f6b">
         * glfwSetDropCallback</a>
         * @param callback Callback function.
         */
        inline void set_drop_callback(WindowDropCallback callback) noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga1bb559c0ebaad63c5c05ad2a066779c4">
         * glfwIconifyWindow</a> on the current window.
         */
        inline void iconify() noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga52527a5904b47d802b6b4bb519cdebc7">
         * glfwRestoreWindow</a> on the current window.
         */
        inline void restore() noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga3f541387449d911274324ae7f17ec56b">
         * glfwMaximizeWindow</a> on the current window.
         */
        inline void maximize() noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga61be47917b72536a148300f46494fc66">
         * glfwShowWindow</a> on the current window.
         */
        inline void show() noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga49401f82a1ba5f15db5590728314d47c">
         * glfwHideWindow</a> on the current window.
         */
        inline void hide() noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga873780357abd3f3a081d71a40aae45a1">
         * glfwFocusWindow</a> on the current window.
         */
        inline void focus() noexcept;

        /**
         * Calls <a href="https://www.glfw.org/docs/3.3/group__window.html#ga2f8d59323fc4692c1d54ba08c863a703">
         * glfwRequestWindowAttention</a> on the current window.
         */
        inline void request_attention() noexcept;

        /**
         * Updates the swapchain associated with the window.
         * @param update_info Update-info specifying the parameters of the new swapchain.
         * @throw std::runtime_error Is thrown, if creating the new swapchain failed.
         */
        void update(const WindowUpdateInfo& update_info);

        /// Destroys the window. After destroying the window is empty and therefore invalid.
        constexpr void destroy() noexcept;

        // default:
        ~Window() = default;

    private:
        static constexpr const char* MSG_WINDOW_CREATE_FAILED       = "[vka::Window]: Failed to create window.";
        static constexpr const char* MSG_SURFACE_CREATE_FAILED      = "[vka::Window]: Failed to create surface.";
        static constexpr const char* MSG_SWAPCHAIN_CREATE_FAILED    = "[vka::Window]: Failed to create swapchain.";
        static constexpr const char* MSG_SWAPCHAIN_UPDATE_FAILED    = "[vka::Window]: Failed to update swapchain.";

        detail::unique_window m_window;
        unique_handle<VkSurfaceKHR> m_surface;
        unique_handle<VkSwapchainKHR> m_swapchain;
        std::vector<VkImage> m_images;
        WindowCallbacks m_callbacks;
        void* m_user_ptr;

        /// Creates the window handle.
        static detail::unique_window create_window(const WindowCreateInfo& create_info);

        /// Creates the surface handle.
        unique_handle<VkSurfaceKHR> create_surface(VkInstance instance) const;

        /// Creates the swapchain handle.
        unique_handle<VkSwapchainKHR> create_swapchain(VkDevice device, const WindowCreateInfo& create_info) const;

        /// Queries the swapchain images.
        std::vector<VkImage> get_images() const;

        /// Checks if the specified presentation mode is supported and falls back to VK_PRESENT_MODE_FIFO_KHR.
        VkPresentModeKHR get_present_mode(VkPhysicalDevice physical_device, VkPresentModeKHR mode) const noexcept;

        /// Window position callback that is called by GLFW.
        static void pos_callback(GLFWwindow* window, int xpos, int ypos);

        /// Window size callback that is called by GLFW.
        static void size_callback(GLFWwindow* window, int width, int height);

        /// Window close callback that is called by GLFW.
        static void close_callback(GLFWwindow* window);

        /// Window refresh callback that is called by GLFW.
        static void refresh_callback(GLFWwindow* window);

        /// Window focus callback that is called by GLFW.
        static void focus_callback(GLFWwindow* window, int focused);

        /// Window iconify callback that is called by GLFW.
        static void iconify_callback(GLFWwindow* window, int iconified);

        /// Window maximize callback that is called by GLFW.
        static void maximize_callback(GLFWwindow* window, int maximized);

        /// Window framebuffer size callback that is called by GLFW.
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        /// Window content scale callback that is called by GLFW.
        static void content_scale_callback(GLFWwindow* window, float xscale, float yscale);

        /// Key callback that is called by GLFW.
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

        /// Character callback that is called by GLFW.
        static void char_callback(GLFWwindow* window, unsigned int codepoint);

        /// Character modifier callback that is called by GLFW.
        static void char_mods_callback(GLFWwindow* window, unsigned int codepoint, int mods);

        /// Mouse button callback that is called by GLFW.
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

        /// Cursor position callback that is called by GLFW.
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

        /// Cursor enter callback that is called by GLFW.
        static void cursor_enter_callback(GLFWwindow* window, int entered);

        /// Scroll callback that is called by GLFW.
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

        /// Drop callback that is called by GLFW.
        static void drop_callback(GLFWwindow* window, int path_count, const char* paths[]);
    };
}