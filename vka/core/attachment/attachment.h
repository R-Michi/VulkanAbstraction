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
     * Helper class to create attachment images. Attachment images are used inside a framebuffer to store color, depth,
     * stencil, etc. information.
     */
    class AttachmentImage
    {
        using AttachmentHandle = detail::attachment::Handle;

    public:
        /// Creates an empty attachment image. This attachment image is invalid.
        constexpr AttachmentImage() noexcept;

        /**
         * Creates the attachment image. The attachment image is valid if no exception was thrown.
         * @param device Device with which the attachment image is created.
         * @param properties Memory properties of the physical device.
         * @param create_info Create-info for the attachment image.
         * @throw std::runtime_error Is thrown, if creating the image, allocating the memory, binding the memory or
         * creating the corresponding image view failed.
         */
        explicit AttachmentImage(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

        /// @return Returns whether the attachment image is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the size of the attachment image.
        constexpr VkExtent2D size() const noexcept;

        /// @return Returns the parent handle.
        constexpr VkDevice parent() const noexcept;

        /// @return Returns the vulkan <c>VkImage</c> handle.
        constexpr VkImage handle() const noexcept;

        /// @return Returns the vulkan <c>VkImageView</c> handle.
        constexpr VkImageView view() const noexcept;

        /// Destroys the attachment image. After destroying the attachment image is empty and therefore invalid.
        constexpr void destroy() noexcept;

        // default:
        AttachmentImage(AttachmentImage&&) = default;
        ~AttachmentImage() = default;
        AttachmentImage& operator= (AttachmentImage&&) = default;

    private:
        static constexpr char IMAGE_CREATE_FAILED[] = "[vka::AttachmentImage]: Failed to create image handle.";
        static constexpr char ALLOC_MEMORY_FAILED[] = "[vka::AttachmentImage]: Failed to allocate memory.";
        static constexpr char BIND_MEMORY_FAILED[] = "[vka::AttachmentImage]: Failed to bind memory to image.";
        static constexpr char VIEW_CREATE_FAILED[] = "[vka::AttachmentImage]: Failed to create image view.";

        unique_handle<AttachmentHandle> m_image;
        VkExtent2D m_extent;

        /// Creates the attachment image.
        static unique_handle<AttachmentHandle> create_attachment(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);
    };
}
