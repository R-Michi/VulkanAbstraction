#pragma once

namespace vka
{
    struct SwapchainFramebufferCreateInfo
    {
        VkImage swapchain_image;
        VkFormat view_format;
        VkExtent2D extent;
        VkComponentMapping components;
        VkImageSubresourceRange subresource_range;
        VkRenderPass render_pass;
    };

    struct SwapchainFramebuffer
    {
        VkImageView view;
        VkFramebuffer framebuffer;

        /**
         * framebuffer != VK_NULL_HANDLE <-> view != VK_NULL_HANDLE.\n
         * view == VK_NULL_HANDLE <-> framebuffer == VK_NULL_HANDLE
         */
        explicit constexpr operator bool() const noexcept { return this->framebuffer != VK_NULL_HANDLE; }
    };
}

namespace vka::swapchain
{
    /// Creates the swapchain framebuffer.
    inline VkResult create_framebuffer(
        VkDevice device,
        const SwapchainFramebufferCreateInfo* create_info,
        const VkAllocationCallbacks* allocator,
        SwapchainFramebuffer* fbo
    ) noexcept;

    /// Destroys the swapchain framebuffer.
    inline void destroy_framebuffer(VkDevice device, SwapchainFramebuffer fbo, const VkAllocationCallbacks* allocator) noexcept;
}