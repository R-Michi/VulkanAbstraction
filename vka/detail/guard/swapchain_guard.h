#pragma once

namespace vka::detail
{
    /**
     * Builds a guard around a single or an array of VkSwapchainKHR handles which are automatically destroyed if the
     * guard goes out of scope.\n
     * Precondition: The swapchain handles to guard are either valid handles or VK_NULL_HANDLE.
     */
    class SwapchainGuard final
    {
    public:
        /// Initializes the object with a single swapchain to guard.
        explicit SwapchainGuard(VkDevice device, VkSwapchainKHR* swapchain) noexcept :
            m_device(device),
            m_swapchain(swapchain),
            m_count(1)
        {}

        /// Initializes the object with an array of swapchains to guard.
        explicit SwapchainGuard(VkDevice device, VkSwapchainKHR* swapchains, uint32_t count) :
            m_device(device),
            m_swapchain(swapchains),
            m_count(count)
        {}

        /// Destroys the swapchain handle.
        ~SwapchainGuard()
        {
            for (uint32_t i = 0; i < this->m_count; i++) [[unlikely]]
            {
                if (this->m_swapchain[i] != VK_NULL_HANDLE)
                    vkDestroySwapchainKHR(this->m_device, this->m_swapchain[i], nullptr);
            }
        }

        /// Releases the swapchain to guard.
        void release()
        {
            // Instead of setting the pointer to 'nullptr' which requires an additional condition in the destructor,
            // setting count to 0 also prevents any swapchain from being destroyed.
            this->m_count = 0;
        }

    private:
        VkDevice m_device;
        VkSwapchainKHR* m_swapchain;
        uint32_t m_count;
    };
}