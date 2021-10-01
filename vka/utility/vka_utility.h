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

#include "../vka_global.h"

namespace vka
{
    namespace utility
    {
        /**
        * @brief Finds the memory type index of a given memoryTypeBits bit mask. Used for VkMemoryRequierements struct.
        * @param[in] physical_device: physical device of the operation
        * @param[in] bits: memoryTypeBits bit mask from VkMemoryRequierements struct
        * @param[in] req_flags: memory property flags to search the index for
        * @return The memory type index of the @param req_flags.
        *   Maximum value of uint32_t (4294967296) if no index could be found.
        */
        uint32_t find_memory_type_index(VkPhysicalDevice physical_device, uint32_t bits, VkMemoryPropertyFlags req_flags);
        
        /**
        * @brief Checks if a format is supported for a given image tiling and format feature.
        * @param[in] physical_device: physical device of the operation
        * @param[in] format: format to check
        * @param[in] tiling: image tiling
        * @param[in] format_featute: format feature
        * @return 'true' if it is supported and 'false' if it is not supported.
        */
        bool is_image_format_supported(VkPhysicalDevice physical_device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature);
        
        /**
        * @param[out] formats: Stores all supported formats for either color, depth, stencil or depth/stencil combined.
        */
        void get_color_formats(std::vector<VkFormat>& formats);
        void get_depth_formats(std::vector<VkFormat>& formats);
        void get_stencil_formats(std::vector<VkFormat>& formats);
        void get_depth_stencil_formats(std::vector<VkFormat>& formats);
        
        /**
        * @brief Returns all supported formats for a given image tiling and format feature.
        * @param[in] physical_device: physical device of the operation
        * @param[in] tiling: image tiling
        * @param[in] format_feature: format feature
        * @param[out] formats: vector where the formats are stored
        */
        void get_supported_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);
        
        /**
        * @brief Converts a format feature flag bit to a image usage flag bit.
        * @param[in] format_feature: format feature flag.
        * @return image usage flag
        */
        VkImageUsageFlags format_feature_to_image_usage(VkFormatFeatureFlags format_feature);
        
        /**
        * @brief Converts a image usage flag bit to a format feature flag bit.
        * @param[in] image_usage: image usage flag.
        * @return format feature flag
        */
        VkFormatFeatureFlags image_usage_to_format_feature(VkImageUsageFlags image_usage);
    }
}