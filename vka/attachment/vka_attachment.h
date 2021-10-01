/**
* @file     vka_attachment.h
* @brief    Attachment image definition file.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../vka_global.h"

namespace vka
{
    class AttachmentImage
    {
    private:
        VkPhysicalDevice _physical_device;
        VkDevice _device;
        
        VkImageCreateInfo _image_create_info;
        VkImageViewCreateInfo _view_create_info;
        
        VkImage _image;
        VkDeviceMemory _memory;
        VkImageView _view;
        
        void _default_img_create_info(void) noexcept;
        void _default_view_create_info(void) noexcept;
        
        bool is_image_format_supported(void) const noexcept;
        void throw_unsupported_format(const std::vector<VkFormat>& formats, size_t block_size);
    
    public:
        AttachmentImage(void) noexcept;
        
        /**
        * @param[in] physical_device: physical device
        * @param[in] device: logical device
        */
        explicit AttachmentImage(VkPhysicalDevice physical_device, VkDevice device) noexcept;
        
        AttachmentImage(const AttachmentImage&) = delete;
        AttachmentImage& operator= (const AttachmentImage&) = delete;
        
        AttachmentImage(AttachmentImage&&) = delete;
        AttachmentImage& operator= (AttachmentImage&&) = delete;
        
        virtual ~AttachmentImage(void);
        
        /**
        * @brief Setter for the parameter of the VkImageCreateInfo struct.
        */
        void set_image_format(VkFormat format) noexcept;
        void set_image_extent(VkExtent2D extent) noexcept;
        void set_image_samples(VkSampleCountFlagBits samples) noexcept;
        void set_image_usage(VkImageUsageFlags usage) noexcept;
        void set_image_queue_family_index(uint32_t index) noexcept;
        
        /**
        * @brief Setter for the parameter of the VkImageViewCreateInfo struct.
        */
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
        
        /** @return image view handle */
        VkImageView view(void) const noexcept;
    };
}