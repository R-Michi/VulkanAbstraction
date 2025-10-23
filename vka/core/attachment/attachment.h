/**
 * @brief Helper class for creating attachment images.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once


namespace vka
{
    /**
     * @brief This class is a helper to create attachment images. Attachment images are used inside a framebuffer to
     * store color, depth, stencil, etc. information.
     */
    class AttachmentImage final
    {
    private:
        static constexpr char IMAGE_CREATE_FAILED[] = "[vka::AttachmentImage::create]: Failed to create image handle.";
        static constexpr char ALLOC_MEMORY_FAILED[] = "[vka::AttachmentImage::create]: Failed to allocate memory.";
        static constexpr char BIND_MEMORY_FAILED[] = "[vka::AttachmentImage::create]: Failed to bind memory to image.";
        static constexpr char VIEW_CREATE_FAILED[] = "[vka::AttachmentImage::create]: Failed to create image view.";

        VkDevice m_device;
        VkDeviceMemory m_memory;
        VkImage m_image;
        VkImageView m_view;
        VkExtent2D m_extent;

        /// @brief Destroys all created vulkan handles.
        void destroy_handles() const noexcept;

        /**
         * @brief Creates the attachment image and its image view.
         * @param properties Specifies the memory properties of the physical device.
         * @param create_info Specifies the create-info for the attachment image.
         * @throw std::runtime_error Is thrown, if creating the image or image view, allocating the memory or binding
         * the memory to the image failed.
         */
        void internal_create(const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

    public:
        /// @brief Every vulkan handle is initialized to VK_NULL_HANDLE.
        inline AttachmentImage() noexcept;

        /**
         * @brief Creates the attachment image.
         * @param device Specifies a valid device.
         * @param properties Specifies the memory properties of the physical device.
         * @param create_info Specifies the create-info for the attachment image.
         * @throw std::runtime_error Is thrown, if creating the image or image view, allocating the memory or binding
         * the memory to the image failed.
         * @details This constructor has the same functionality as the function create().
         */
        explicit inline AttachmentImage(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

        // The AttachmentImage must not be copied.
        AttachmentImage(const AttachmentImage&) = delete;
        AttachmentImage& operator= (const AttachmentImage&) = delete;

        /**
         * @brief Moves the ownership of an attachment image to a different object.
         * @param src Specifies the attachment image to move.
         * @details The source attachment image becomes invalidated. If the move destination has been created before and
         * is a valid object, it gets destroyed.
         */
        inline AttachmentImage(AttachmentImage&& src) noexcept;
        inline AttachmentImage& operator= (AttachmentImage&& src) noexcept;

        /// @brief Destroys all created vulkan handles.
        inline ~AttachmentImage();

        /**
         * @brief Creates the attachment image.
         * @param device Specifies a valid device.
         * @param properties Specifies the memory properties of the physical device.
         * @param create_info Specifies the create-info for the attachment image.
         * @throw std::runtime_error Is thrown, if creating the image or image view, allocating the memory or binding
         * the memory to the image failed.
         */
        inline void create(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

        /**
         * @brief Destroys all created vulkan handles.
         * @details After the handles are destroyed, they are reset to VK_NULL_HANDLE. Moreover, this function does not
         * destroy parent handles.
         */
        inline void destroy() noexcept;

        /// @return Returns the size/extent of the image.
        inline VkExtent2D size() const noexcept;

        /// @return Returns the image handle.
        inline VkImage handle() const noexcept;

        /// @return Returns the image view handle.
        inline VkImageView view() const noexcept;

        /// @return Returns 'true' if the attachment image is a valid object and 'false' otherwise.
        inline bool is_valid() const noexcept;
    };
}
