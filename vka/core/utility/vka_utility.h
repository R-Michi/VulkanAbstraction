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
        /*
        * Searches in the physical device's memory properties for the corresponding memory type index.
        * If a memory type index was found, the corresponding index is returned.
        * If no memory type index was found, vka::NPOS is returned.
        * The physical device is given by 'device', the bit mask is specified by 'bits'
        * the requiered memory property flags are specified by 'req_flags'.
        * The bit mask 'bits' is usually given by memory requierements that have a member called 'memoryTypeBits'.
        */
        uint32_t find_memory_type_index(VkPhysicalDevice device, uint32_t bits, VkMemoryPropertyFlags req_flags);

        /*
        * Checks if a format feature is supported for a given image tiling and format feature.
        * A physical device is requiered and is given by 'device'.
        * The image tiling is specified by 'tiling' and the format feature is
        * specified by 'format_feature'.
        * If the format feature is supported, true is returned and false otherwise.
        */
        bool supports_format_feature(VkPhysicalDevice device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature);
        
        /*
        * Returns all standard color formats, that are not extension
        * formats nor compressed formats.
        * More formally returns all formats from:
        * static_cast<VkFormat>(1) to static_cast<VkFormat>(123).
        * The color formats are stored in the 'formats' vector.
        */
        constexpr void get_color_formats(std::vector<VkFormat>& formats);

        /*
        * Returns all standard depth formats, that are not extension
        * formats nor compressed formats.
        * More formally returns all formats from:
        * static_cast<VkFormat>(124) to static_cast<VkFormat>(130)
        * excluding static_cast<VkFormat>(127).
        * The depth formats are stored in the 'formats' vector.
        */
        constexpr void get_depth_formats(std::vector<VkFormat>& formats);

        /*
        * Returns all standard stencil formats, that are not extension
        * formats nor compressed formats.
        * More formally returns all formats from:
        * static_cast<VkFormat>(127) to static_cast<VkFormat>(130).
        * The stencil formats are stored in the 'formats' vector.
        */
        constexpr void get_stencil_formats(std::vector<VkFormat>& formats);

        /*
        * Returns all standard depth-stencil formats, that are not extension
        * formats nor compressed formats.
        * More formally returns all formats from:
        * static_cast<VkFormat>(128) to static_cast<VkFormat>(130).
        * The depth-stencil formats are stored in the 'formats' vector.
        */
        constexpr void get_depth_stencil_formats(std::vector<VkFormat>& formats);

        /*
        * Returns all supported formats for a given image tiling and format feature.
        * A physical device is requiered and is given by 'device'.
        * The image tiling is specified by 'tiling' and the format feature is
        * specified by 'format_feature'.
        * The returned formats are stored in the 'formats' vector.
        */
        void get_supported_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);

        /*
        * Converts format feature flags to image usage flags.
        * The format feature flags to convert are specified by 'format_feature'.
        * The corresponding image usage flags are returned.
        * The function name stands for "convert format feature to image usage".
        */
        constexpr VkImageUsageFlags cvt_ff2iu(VkFormatFeatureFlags format_feature);

        /*
        * Converts image usage flags to format feature flags.
        * The image usage flags to convert are specified by 'image_usage'.
        * The corresponding format feature flags are returned.
        * The function name stands for "convert image usage to format feature".
        */
        constexpr VkFormatFeatureFlags cvt_iu2ff(VkImageUsageFlags image_usage);

        /*
        * Converts a vector of std::string strings to a vector of const char*.
        * The vector of std strings is given by 'std_in' and the resulting
        * output vector is stored in 'ccp_out'.
        * This function is mainly used to convert extension and layer names from std::string to const char*
        * in order to pass them to the corresponding create infos.
        * The function name stands for "convert std::string to const char pointer vector"
        */
        void cvt_stdstr2ccpv(const std::vector<std::string>& std_in, std::vector<const char*>& ccp_out);

        /*
        * Executes multiple secondary command buffers. This function is especially used to
        * execute buffer copy, image copy and image layout transision commands, enqueued by
        * vka::Buffer::enqueue_copy, vka::Image::enqueue_copy, vka::Image::enqueue_change_layout.
        * A logical device is requiered and is given by 'device'. The requiered command pool is
        * given by 'pool' the the requiered queue is given by 'queue'.
        * The number of secondary command buffers to execute is specified by 'count' and
        * the secondary command buffers are specified by 'scbs'.
        */
        VkResult execute_scb(VkDevice device, VkCommandPool pool, VkQueue queue, uint32_t count, const VkCommandBuffer* scbs);

        /*
        * Converts a vulkan format to a size in bytes.
        * The format to convert is specified by 'format'.
        * The size in bytes is returned.
        */
        size_t format_sizeof(VkFormat format) noexcept;
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_utility_impl.inl"
#endif
