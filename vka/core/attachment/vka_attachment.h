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
        VkPhysicalDevice _physical_device;
        VkDevice _device;
        
        VkImageCreateInfo _ici, _backup_ici;
        VkImageViewCreateInfo _vci, _backup_vci;
        VkMemoryPropertyFlags _properties;
        
        VkImage _image;
        VkDeviceMemory _memory;
        VkImageView _view;

        size_t _mem_size;

        static void init_ici(VkImageCreateInfo& ci);
        static void init_vci(VkImageViewCreateInfo& ci);
    
    public:
        AttachmentImage(void) noexcept;
        
        /**
        * @param[in] physical_device: physical device
        * @param[in] device: logical device
        */
        explicit AttachmentImage(VkPhysicalDevice physical_device, VkDevice device) noexcept;
        
        AttachmentImage(const AttachmentImage&) = delete;
        AttachmentImage& operator= (const AttachmentImage&) = delete;
        
        /** 
         * @brief   Move constructor and move operator. 
         * @note    Both the move constructor and move operator
         *          copy all the properties of the image and transfer
         *          ownership of the image object's handles.
         *          Therefore, the source image will become cleared.
         */
        AttachmentImage(AttachmentImage&& src);
        AttachmentImage& operator= (AttachmentImage&& src);
        
        virtual ~AttachmentImage(void);
        
        /** @brief Setter for the parameter of the VkImageCreateInfo struct. */
        void set_image_format(VkFormat format) noexcept;
        void set_image_extent(VkExtent2D extent) noexcept;
        void set_image_extent(uint32_t w, uint32_t h) noexcept;
        void set_image_samples(VkSampleCountFlagBits samples) noexcept;
        void set_image_usage(VkImageUsageFlags usage) noexcept;
        void set_image_queue_family_index(uint32_t index) noexcept;
        
        /** @brief Setter for the parameter of the VkImageViewCreateInfo struct. */
        void set_view_format(VkFormat format) noexcept;
        void set_view_components(VkComponentMapping component_mapping) noexcept;
        void set_view_aspect_mask(VkImageAspectFlags aspect_mask) noexcept;
        
        /** @param[in] physical_device: physical device */
        void set_physical_device(VkPhysicalDevice physical_device) noexcept;
        
        /** @param[in] device: logical device */
        void set_device(VkDevice device) noexcept;
        
        /**
        * @brief Creates the attachment image.
        * @return vulkan result
        */
        VkResult create(void);
        
        /** @brief Cleares the attachment image. The attachment image is no longer aviable after a clear. */
        void clear(void);

        /** @return image create info at the time point when the attachment image was created */
        const VkImageCreateInfo& get_image_create_info(void) const noexcept;

        /** @return image view create info at the time point when the attachment image was created */
        const VkImageViewCreateInfo& get_view_create_info(void) const noexcept;

        /** @return extent of the created attachment image */
        VkExtent2D extent(void) const noexcept;

        /** @return number of pixels stored inside the image */
        uint32_t count(void) const noexcept;

        /** @return allocated memory size of the image in bytes */
        size_t mem_size(void) const noexcept;

        /** @return image handle */
        VkImage image(void) const noexcept;
        
        /** @return image view handle */
        VkImageView view(void) const noexcept;

        /** @return Boolean wether the attachment image has already been created. */
        bool is_created(void) const noexcept;
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_attachment_impl.inl"
#endif
