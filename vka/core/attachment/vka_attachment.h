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

        void destroy_handles(void) noexcept;

        void internal_create(VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

    public:
        /*
        * Initialization constructor which initializes 'this' with a device. The device does not
        * have to be valid at initialization, it must be valid at creation. This constructor has
        * the same functionality as the .init() function. Furthermore, this constructor is also
        * used as the default constructor, where the device is initialized to a VK_NULL_HANDLE.
        * All other handles are initialized to a VK_NULL_HANDLE and every other member variable
        * contains its default initialization.
        */

        inline AttachmentImage(void) noexcept;

        explicit inline AttachmentImage(VkDevice device, VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

        // The AttachmentImage must not be copied.
        AttachmentImage(const AttachmentImage&) = delete;
        AttachmentImage& operator= (const AttachmentImage&) = delete;

        /*
        * Moves another object of AttachmentImage into 'this'. 'This' now holds the ownership of
        * all the handles of the source object. The source object will become invalidated and
        * contains its default initialization except for the device. The device will be copied and
        * is preserved in the moved object. If 'this' was created and is a valid object, 'this' is
        * destroyed and replaced by the handles of the moved object.
        */
        inline AttachmentImage(AttachmentImage&& src) noexcept;
        inline AttachmentImage& operator= (AttachmentImage&& src) noexcept;

        // The destructor destroys all the vulkan handles.
        virtual inline ~AttachmentImage(void);

        /*
        * This function creates the AttachmentImage and the internal handles are now valid, if no
        * error occurred. If an error occured while creating, a std::runtime_error exception is
        * thrown with an appropriate message about the error. The attachment image is created with
        * an AttachmentImageCreateInfo structure which is used for the creation of the image and
        * image view handle. The create-info is specified by 'create_info'. Additionally, the
        * physical device and the memory properties of the physical device are required and are
        * specified by 'device' and 'properties' respectively. A std::invalid_argument exception
        * is thrown, if 'this' has not been initialized with a device or if the image format
        * specified in the create-info is not supported.
        */
        inline void create(VkDevice device, VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

        /*
        * Destroys the AttachmentImage object. After destroying, 'this' holds its default
        * initialization except for the device. The device will be preserved after destroying and
        * 'this' does not need to be reinitialized. This is also done by the destructor.
        */
        inline void destroy(void) noexcept;

        // Returns the size/extent of the image.
        inline VkExtent2D size(void) const noexcept;

        // Returns the image handle.
        inline VkImage handle(void) const noexcept;

        // Returns the image view handle.
        inline VkImageView view(void) const noexcept;

        // Returns true, if the AttachmentImage is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_attachment_impl.inl"
#endif
#include "vka_attachment_inline_impl.inl"
