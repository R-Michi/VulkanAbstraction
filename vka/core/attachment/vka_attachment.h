/**
* @file     vka_attachment.h
* @brief    Attachment image definition file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka
{
    class AttachmentImage
    {
    private:
        static constexpr const char IMAGE_CREATE_FAILED[] = "[vka::AttachmentImage::create]: Failed to create image handle.";
        static constexpr const char ALLOC_MEMORY_FAILED[] = "[vka::AttachmentImage::create]: Failed to allocate memory.";
        static constexpr const char BIND_MEMORY_FAILED[] = "[vka::AttachmentImage::create]: Failed to bind memory to image.";
        static constexpr const char VIEW_CREATE_FAILED[] = "[vka::AttachmentImage::create]: Failed to create image view.";

        VkDevice m_device;
        VkDeviceMemory m_memory;
        VkImage m_image;
        VkImageView m_view;
        VkExtent2D m_extent;

        /// @brief Destroys all created vulkan handles.
        void destroy_handles(void) noexcept;

        /**
         * @brief Creates the attachment image and its image view.
         * @param pdevice Specifies a valid physical device.
         * @param properties Specifies the memory properties of the physical device.
         * @param create_info Specifies the create info for the attachment image.
         * @throw std::runtime_error Is thrown, if creating the image or image view, allocating the memory or binding
         * the memory to the image failed.
         */
        void internal_create(VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

    public:
        /// @details Every vulkan handle is initialized to VK_NULL_HANDLE.
        inline AttachmentImage(void) noexcept;

        /**
         * @brief Creates the attachment image.
         * @details This constructor has the same functionality as the function create().
         * @param device Specifies a valid device.
         * @param pdevice Specifies a valid physical device.
         * @param properties Specifies the memory properties of the physical device.
         * @param create_info Specifies the create info for the attachment image.
         * @throw std::runtime_error Is thrown, if creating the image or image view, allocating the memory or binding
         * the memory to the image failed.
         */
        explicit inline AttachmentImage(VkDevice device, VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

        // The AttachmentImage must not be copied.
        AttachmentImage(const AttachmentImage&) = delete;
        AttachmentImage& operator= (const AttachmentImage&) = delete;

        /**
         * @brief Transfers ownership of an attachment image to 'this'.
         * @details The source attachment image becomes invalidated. If 'this' has been created before and is a valid
         * instance, it gets destroyed and replaced by the handles of the source object.
         * @param src Specifies the attachment image to move.
         */
        inline AttachmentImage(AttachmentImage&& src) noexcept;
        inline AttachmentImage& operator= (AttachmentImage&& src) noexcept;

        /// @brief Destroys all created vulkan handles.
        virtual inline ~AttachmentImage(void);

        /**
         * @brief Creates the attachment image.
         * @param device Specifies a valid device.
         * @param pdevice Specifies a valid physical device.
         * @param properties Specifies the memory properties of the physical device.
         * @param create_info Specifies the create info for the attachment image.
         * @throw std::runtime_error Is thrown, if creating the image or image view, allocating the memory or binding
         * the memory to the image failed.
         */
        inline void create(VkDevice device, VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

        /**
         * @brief Destroys all created vulkan handles.
         * @details After the handles are destroyed they are reset to VK_NULL_HANDLE. Moreover, parent handles are not
         * destroyed by this function.
         */
        inline void destroy(void) noexcept;

        /// @return Returns the size/extent of the image.
        inline VkExtent2D size(void) const noexcept;

        /// @return Returns the image handle.
        inline VkImage handle(void) const noexcept;

        /// @return Returns the image view handle.
        inline VkImageView view(void) const noexcept;

        /// @return Returns 'true', if the attachment image is a valid object and 'false' otherwise.
        inline bool is_valid(void) const noexcept;
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_attachment_impl.inl"
#endif
#include "vka_attachment_inline_impl.inl"
