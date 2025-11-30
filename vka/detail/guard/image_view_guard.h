#pragma once

namespace vka::detail
{
    /**
     * Builds a guard around a single or an array of VkImageView handles which are automatically destroyed if the guard
     * goes out of scope.\n
     * Precondition: The image view handles to guard are either valid handles or VK_NULL_HANDLE.
     */
    class ImageViewGuard final
    {
    public:
        /// Initializes the object with a single image view to guard.
        explicit ImageViewGuard(VkDevice device, VkImageView* view) :
            m_device(device),
            m_views(view),
            m_count(1)
        {}

        /// Initializes the object with an array of image views to guard.
        explicit ImageViewGuard(VkDevice device, VkImageView* views, uint32_t count) :
            m_device(device),
            m_views(views),
            m_count(count)
        {}

        /// Destroys all image views.
        ~ImageViewGuard()
        {
            for (uint32_t i = 0; i < this->m_count; i++) [[unlikely]]
            {
                if (this->m_views[i] != VK_NULL_HANDLE)
                    vkDestroyImageView(this->m_device, this->m_views[i], nullptr);
            }
        }

        /// Releases the image views to guard.
        void release()
        {
            // Instead of setting the pointer to 'nullptr' which requires an additional condition in the destructor,
            // setting count to 0 also prevents any image view from being destroyed.
            this->m_count = 0;
        }
    private:
        VkDevice m_device;
        VkImageView* m_views;
        uint32_t m_count;
    };
}