/**
* @file     vka_utility.h
* @brief    Useful utility functions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../detail/utility/vka_utility.h"

namespace vka
{
    namespace utility
    {
        /**
        * @brief                        Finds the memory type index of a given memoryTypeBits bit mask. Used for VkMemoryRequierements struct.
        * @param[in] physical_device    physical device of the operation
        * @param[in] bits               memoryTypeBits bit mask from VkMemoryRequierements struct
        * @param[in] req_flags          memory property flags to search the index for
        * @return                       The memory type index.
        *                               Maximum value of uint32_t (4294967296) if no index could be found.
        */
        uint32_t find_memory_type_index(VkPhysicalDevice physical_device, uint32_t bits, VkMemoryPropertyFlags req_flags);
        
        /**
        * @brief                        Checks if a format feature is supported for a given image tiling and format feature.
        * @param[in] physical_device    physical device of the operation
        * @param[in] format             image
        * @param[in] tiling             image tiling
        * @param[in] format_featute     format feature to check
        * @return                       Boolean wether a format feature is supported.
        */
        bool is_format_feature_supported(VkPhysicalDevice physical_device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature);
        
        /**
        * @param[out] formats Stores all supported formats for either color, depth, stencil or depth/stencil combined.
        */
        void get_color_formats(std::vector<VkFormat>& formats);
        void get_depth_formats(std::vector<VkFormat>& formats);
        void get_stencil_formats(std::vector<VkFormat>& formats);
        void get_depth_stencil_formats(std::vector<VkFormat>& formats);
        
        /**
        * @brief                        Returns all supported formats for a given image tiling and format feature.
        * @param[in]  physical_device   physical device of the operation
        * @param[in]  tiling            image tiling
        * @param[in]  format_feature    format feature
        * @param[out] formats           vector where the formats are stored
        */
        void get_supported_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);
        
        /**
        * @brief                    Converts format feature flags to image use flags.
        * @param[in] format_feature format feature flags
        * @return                   image usage flags
        */
        VkImageUsageFlags format_feature_to_image_usage(VkFormatFeatureFlags format_feature);
        
        /**
        * @brief                    Converts image usage flags ti format feature flags.
        * @param[in] image_usage    image usage flags
        * @return                   format feature flags
        */
        VkFormatFeatureFlags image_usage_to_format_feature(VkImageUsageFlags image_usage);

        /**
         * @brief               Executes multiple secondary command buffers. This function is especially used to
         *                      execute buffer copy, image copy and image layout transision commans, enqueued by 
         *                      vka::Buffer::enqueue_copy, vka::Image::enqueue_copy, vka::Image::enqueue_change_layout.
         * @param[in] device    logical device
         * @param[in] pool      command pool
         * @param[in] queue     command queue to submit the commands to
         * @param[in] count     number of secondary command buffers to execute
         * @param[in] scbs      secondary command buffers
         * @return              Vulkan result wether the execution was successful.
         */
        VkResult execute_scb(VkDevice device, VkCommandPool pool, VkQueue queue, uint32_t count, const VkCommandBuffer* scbs);

        /**
         * @brief           Converts a vulkan format to a size in bytes.
         * @param format    Vulkan format.
         * @return          Size of the format in bytes.
         */
        size_t format_sizeof(VkFormat format) noexcept;
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_utility_impl.inl"
#endif
