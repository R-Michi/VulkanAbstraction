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
     * Structure specifying the parameters of a newly created attachment image object. Parameters prefixed with
     * <c>image</c> correspond to the parameters of a
     * <a href="https://docs.vulkan.org/refpages/latest/refpages/source/VkImageCreateInfo.html">VkImageCreateInfo</a>.
     * Parameters prefixed with <c>view</c> correspond to the parameters of a
     * <a href="https://docs.vulkan.org/refpages/latest/refpages/source/VkImageViewCreateInfo.html">
     * VkImageViewCreateInfo</a>.
     */
    struct AttachmentImageCreateInfo
    {
        VkFormat                imageFormat;
        VkExtent2D              imageExtent;
        VkSampleCountFlagBits   imageSamples;
        VkImageUsageFlags       imageUsage;
        VkSharingMode           imageSharingMode;
        uint32_t                imageQueueFamilyIndexCount;
        const uint32_t*         imageQueueFamilyIndices;
        VkFormat                viewFormat;
        VkComponentMapping      viewComponentMapping;
        VkImageAspectFlags      viewAspectMask;
    };

    /**
     * Abstraction of an image used as an attachment for framebuffers. Contains the vulkan <c>VkImage</c> and the
     * corresponding <c>VkImageView</c> handle.
     *
     * <b>Default initialization:</b>\n
     * The default constructor creates an <b>empty</b> attachment image. This empty object is invalid and cannot be used
     * as an attachment image. Any member function returning a vulkan handle returns <c>VK_NULL_HANDLE</c>. Calling
     * <c>size()</c> returns <c>{0, 0}</c>. Calling <c>destroy()</c> does nothing.
     *
     * <b>Initialization:</b>\n
     * The initialization constructor creates a valid object that can be used as an attachment image, if no exception
     * was thrown.
     *
     * <b>Copy behaviour:</b>\n
     * The copy constructor and operator are deleted.
     *
     * <b>Moving behaviour:</b>\n
     * When calling the move constructor or operator, the moved object is invalidated and performing any operation on it
     * is unsafe. This may lead to undefined behaviour or even a crash. If an already valid object is replaced by a
     * move, the current object is destroyed.
     *
     * <b>Destroy behaviour:</b>\n
     * Destroys all vulkan handles and sets everything back to default values. After destroying the object is an
     * <b>empty</b> attachment image.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     */
    class AttachmentImage final
    {
        using Handle = detail::attachment::Handle;

    public:
        /// Creates an empty attachment image. This attachment image is invalid.
        constexpr AttachmentImage() noexcept;

        /**
         * Creates the attachment image. The attachment image is valid if no exception was thrown.
         * @param device Device with which the attachment image is created.
         * @param properties Memory properties of the physical device.
         * @param create_info Create-info for the attachment image.
         * @throw std::runtime_error Is thrown, if creating the image, allocating memory for the image, binding the
         * memory to the image or creating the corresponding image view failed.
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

        unique_handle<Handle> m_image;
        VkExtent2D m_extent;

        /// Creates the attachment image.
        static unique_handle<Handle> create_attachment(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);
    };
}
