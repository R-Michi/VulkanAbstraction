#include <vka/vka.h>

#ifdef VKA_GLFW_ENABLE

vka::Window::Window(VkInstance instance, VkDevice device, const WindowCreateInfo& create_info) :
    m_window(create_window(create_info)),
    m_surface(create_surface(instance)),
    m_swapchain(create_swapchain(device, create_info)),
    m_images(get_images()),
    m_callbacks{},
    m_user_ptr(nullptr)
{
    glfwSetWindowUserPointer(this->m_window.get(), this);
}

void vka::Window::update(const WindowUpdateInfo& update_info)
{
    const VkSwapchainCreateInfoKHR swapchain_create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = update_info.flags,
        .surface = this->m_surface.get(),
        .minImageCount = update_info.min_image_count,
        .imageFormat = update_info.image_format,
        .imageColorSpace = update_info.image_color_space,
        .imageExtent = this->surface_size(),
        .imageArrayLayers = update_info.image_array_layers,
        .imageUsage = update_info.image_usage,
        .imageSharingMode = update_info.image_sharing_mode,
        .queueFamilyIndexCount = update_info.queue_family_index_count,
        .pQueueFamilyIndices = update_info.p_queue_family_indices,
        .preTransform = update_info.pre_transform,
        .compositeAlpha = update_info.composite_alpha,
        .presentMode = update_info.present_mode,
        .clipped = update_info.clipped,
        .oldSwapchain = this->m_swapchain.get()
    };

    VkSwapchainKHR swapchain;
    check_result(vkCreateSwapchainKHR(this->m_swapchain.parent(), &swapchain_create_info, nullptr, &swapchain), MSG_SWAPCHAIN_UPDATE_FAILED);
    this->m_swapchain.reset(swapchain);
    this->m_images = get_images();
}

vka::detail::unique_window vka::Window::create_window(const WindowCreateInfo& create_info)
{
    GLFWwindow* window = glfwCreateWindow((int)create_info.size.width, (int)create_info.size.height, create_info.title, create_info.monitor, create_info.share);
    if (window == nullptr) [[unlikely]]
        detail::error::throw_runtime_error(MSG_WINDOW_CREATE_FAILED);
    return detail::unique_window(window);
}

vka::unique_handle<VkSurfaceKHR> vka::Window::create_surface(VkInstance instance) const
{
    VkSurfaceKHR surface;
    check_result(glfwCreateWindowSurface(instance, this->m_window.get(), nullptr, &surface), MSG_SURFACE_CREATE_FAILED);
    return unique_handle(instance, surface);
}

vka::unique_handle<VkSwapchainKHR> vka::Window::create_swapchain(VkDevice device, const WindowCreateInfo& create_info) const
{
    int w, h;
    glfwGetFramebufferSize(this->m_window.get(), &w, &h);

    const VkSwapchainCreateInfoKHR swapchain_create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = create_info.flags,
        .surface = this->m_surface.get(),
        .minImageCount = create_info.min_image_count,
        .imageFormat = create_info.image_format,
        .imageColorSpace = create_info.image_color_space,
        .imageExtent = { (uint32_t)w, (uint32_t)h },
        .imageArrayLayers = create_info.image_array_layers,
        .imageUsage = create_info.image_usage,
        .imageSharingMode = create_info.image_sharing_mode,
        .queueFamilyIndexCount = create_info.queue_family_index_count,
        .pQueueFamilyIndices = create_info.p_queue_family_indices,
        .preTransform = create_info.pre_transform,
        .compositeAlpha = create_info.composite_alpha,
        .presentMode = create_info.present_mode,
        .clipped = create_info.clipped,
        .oldSwapchain = VK_NULL_HANDLE
    };
    VkSwapchainKHR swapchain;
    check_result(vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain), MSG_SWAPCHAIN_CREATE_FAILED);
    return unique_handle(device, swapchain);
}

std::vector<VkImage> vka::Window::get_images() const
{
    uint32_t image_count;
    vkGetSwapchainImagesKHR(this->m_swapchain.parent(), this->m_swapchain.get(), &image_count, nullptr);

    std::vector<VkImage> images(image_count);
    vkGetSwapchainImagesKHR(this->m_swapchain.parent(), this->m_swapchain.get(), &image_count, images.data());
    return images;
}

void vka::Window::pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.pos(*win, { xpos, ypos });
}

void vka::Window::size_callback(GLFWwindow* window, int width, int height)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.size(*win, { (uint32_t)width, (uint32_t)height });
}

void vka::Window::close_callback(GLFWwindow* window)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.close(*win);
}

void vka::Window::refresh_callback(GLFWwindow* window)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.refresh(*win);
}

void vka::Window::focus_callback(GLFWwindow* window, int focused)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.focus(*win, (bool)focused);
}

void vka::Window::iconify_callback(GLFWwindow* window, int iconified)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.iconify(*win, (bool)iconified);
}

void vka::Window::maximize_callback(GLFWwindow* window, int maximized)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.maximize(*win, (bool)maximized);
}

void vka::Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.surface_size(*win, { (uint32_t)width, (uint32_t)height });
}

void vka::Window::content_scale_callback(GLFWwindow* window, float xscale, float yscale)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.content_scale(*win, { xscale, yscale });
}

void vka::Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.key(*win, key, scancode, action, mods);
}

void vka::Window::char_callback(GLFWwindow* window, unsigned int codepoint)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.character(*win, codepoint);
}

void vka::Window::char_mods_callback(GLFWwindow* window, unsigned int codepoint, int mods)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.char_mods(*win, codepoint, mods);
}

void vka::Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.mouse_button(*win, button, action, mods);
}

void vka::Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.cursor_pos(*win, { xpos, ypos });
}

void vka::Window::cursor_enter_callback(GLFWwindow* window, int entered)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.cursor_enter(*win, (bool)entered);
}

void vka::Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.scroll(*win, { xoffset, yoffset });
}

void vka::Window::drop_callback(GLFWwindow* window, int path_count, const char* paths[])
{
    Window* const win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->m_callbacks.drop(*win, path_count, paths);
}

#endif