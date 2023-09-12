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
        VkDevice device;

        VkDeviceMemory memory;
        VkImage m_image;
        VkImageView m_view;
        VkExtent2D extent;

        /*
        * Checks if everything is correct at creation. Throws exceptions if anything was worng
        * initialized, or was not initialized.
        */
        inline void validate(VkPhysicalDevice pdevice, const AttachmentImageCreateInfo& create_info);
        inline void destroy_handles(void) noexcept;

    public:
        /*
        * Initialization constructor which initializes 'this' with a device. The device does not
        * have to be valid at initialization, it must be valid at creation. This constructor has
        * the same functionality as the .init() function. Furthermore, this constructor is also
        * used as the default constructor, where the device is initialized to a VK_NULL_HANDLE.
        * All other handles are initialized to a VK_NULL_HANDLE and every other member variable
        * contains its default initialization.
        */
        explicit AttachmentImage(VkDevice device = VK_NULL_HANDLE) noexcept;

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
        AttachmentImage(AttachmentImage&& src) noexcept;
        AttachmentImage& operator= (AttachmentImage&& src) noexcept;

        // The destructor destroyes all the vulkan handles.
        virtual ~AttachmentImage(void);

        /*
        * Initializes 'this' with a device. The device does not have to be valid at initialization.
        * However, it must be valid at creation. The initialization cannot be changed, if the
        * AttachmentImage is a valid object.
        */
        void init(VkDevice device) noexcept;

        /*
        * This function creates the AttachmentImage and the internal handles are now valid, if no
        * error occured. If an error occured, the vulkan result is returned. If no error occured,
        * VK_SUCCESS is returned. The attachment image is created with an AttachmentImageCreateInfo
        * structure which is used for the creation of the image and image view handle. The create
        * info is specified by 'create_info'. Additionally, the physical device and the memory
        * properties of the physical device are requiered and are specified by 'pdevice' and
        * 'properties' respectively. An std::invalid_argument exception is thrown, if 'this' has
        * not been initialized with a device or if the image format specified in the create info is
        * not supported.
        */
        VkResult create(VkPhysicalDevice pdevice, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info);

        /*
        * Destroyes the AttachmentImage object. After destroying, 'this' holds its default
        * initialization except for the device. The device will be preserved after destroying and
        * 'this' does not need to be reinitialized. This is also done by the destructor.
        */
        void destroy(void) noexcept;

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
