#pragma once

namespace vka::detail
{
    /**
     * Defines a guard around a VkSwapchainKHR handle which automatically destroys it if the guard goes out of scope.
     * Precondition: The swapchain to guard is a valid swapchain or VK_NULL_HANDLE.
     */
    class SwapchainGuard final
    {
    public:
        /// Initializes the object with the swapchain to guard.
        explicit SwapchainGuard(VkDevice device, VkSwapchainKHR swapchain) :
            m_device(device),
            m_swapchain(swapchain)
        {}

        /// Destroys the swapchain handle.
        ~SwapchainGuard()
        {
            if (this->m_swapchain != VK_NULL_HANDLE) [[unlikely]]
                vkDestroySwapchainKHR(this->m_device, this->m_swapchain, nullptr);
        }

        /// Releases the swapchain to guard.
        void release()
        {
            this->m_swapchain = VK_NULL_HANDLE;
        }

    private:
        VkDevice m_device;
        VkSwapchainKHR m_swapchain;
    };
}