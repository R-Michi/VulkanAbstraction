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
        * This class stores a command buffer object and automatically destroyes it in the destructor.
        * A template argument can be specified which indicates that the stored command buffer is
        * already "safe". This means that the command buffer is already a valid handle and can
        * never be VK_NULL_HANDLE. This template argument specified by 'Safe' is false by default.
        */
        template<bool Safe = false>
        class CommandBufferGuard final
        {
        private:
            VkDevice device;
            VkCommandPool pool;
            VkCommandBuffer cbo;

        public:
            inline CommandBufferGuard(VkDevice device, VkCommandPool pool, VkCommandBuffer cbo) noexcept;
            inline ~CommandBufferGuard(void);
        };

        /*
        * Searches for a supported memory type index for a given memoryTypeBits bit-mask in the
        * physical device's memory types. The memory properties that contains all supported memory
        * types of the physical device is specified by 'properties'. The memory specific memory
        * types is given by a bit mask and specified by 'bits'. For every aviable memory type a bit
        * is set in the mask at the corresponding index of the memory type. This mask can be
        * queried from VkMemoryRequierements::memoryTypeBits. The memory type flags that are
        * requiered are specified by 'req_flags'. If the requiered flags are supported by one
        * memory type, the index of the memory type is returned. Otherwise, if the flags are not
        * supported, vka::NPOS32 is returned.
        */
        uint32_t find_memory_type_index(const VkPhysicalDeviceMemoryProperties& properties, uint32_t bits, VkMemoryPropertyFlags req_flags) noexcept;

        /*
        * Checks if a format feature is supported for a given image tiling. The format properties
        * that contains all supported format features is specified by 'properties'. The requiered
        * format feature flags are specified by 'format_feature'. If the requiered format features
        * are supported, true is returned and false otherwise.
        */
        bool supports_format_feature(const VkFormatProperties& properties, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;

        /*
        * This function does the same as supports_format_feature(). Additionally, it queries the
        * format properties internally and requieres a physical device and a format from which the
        * format properties are queried. The physical device is specified by 'device' and the
        * format is specified by 'format'.
        */
        bool supports_format_feature2(VkPhysicalDevice device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept;
        
        /*
        * Returns all standard color formats, that are not extension formats nor compressed
        * formats. More formally returns all formats in the interval of
        * [static_cast<VkFormat>(1), static_cast<VkFormat>(123)]. The formats are stored in the
        * array specified by 'formats'. This array must have a length of at least 123 elements.
        */
        constexpr void get_color_formats(VkFormat* formats) noexcept;

        /*
        * Returns all standard depth formats, that are not extension formats nor compressed
        * formats. More formally returns all formats in the interval of
        * [static_cast<VkFormat>(124), static_cast<VkFormat>(130)] \ static_cast<VkFormat>(127).
        * The formats are stored in the array specified by 'formats'. This array must have a length
        * of at least 6 elements.
        */
        constexpr void get_depth_formats(VkFormat* formats) noexcept;

        /*
        * Returns all standard stencil formats, that are not extension formats nor compressed
        * formats. More formally returns all formats in the interval of
        * [static_cast<VkFormat>(127), static_cast<VkFormat>(130)]. The formats are stored in the
        * array specified by 'formats'. This array must have a length of at least 4 elements.
        */
        constexpr void get_stencil_formats(VkFormat* formats) noexcept;

        /*
        * Returns all standard depth-stencil formats, that are not extension formats nor compressed
        * formats. More formally returns all formats in the interval of
        * [static_cast<VkFormat>(128), static_cast<VkFormat>(130)]. The formats are stored in the
        * array specified by 'formats'. This array must have a length of at least 3 elements.
        */
        constexpr void get_depth_stencil_formats(VkFormat* formats) noexcept;

        /*
        * Returns all supported formats for a given image tiling and format feature. A physical
        * device is requiered and is given by 'device'. The image tiling is specified by 'tiling'
        * and the format feature is specified by 'format_feature'. The returned formats are stored
        * in the 'formats' vector.
        * NOTE: A std::vector is used because the number of formats is not known in advance.
        * To speed up the querying, memory can be reserved by the vector to minimize reallocations.
        */
        void get_supported_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);

        /*
        * Converts format feature flags to image usage flags. The format feature flags to convert
        * are specified by 'format_feature'. The corresponding image usage flags are returned.
        * NOTE: The function name stands for "convert format feature to image usage".
        */
        constexpr VkImageUsageFlags cvt_ff2iu(VkFormatFeatureFlags format_feature) noexcept;

        /*
        * Converts image usage flags to format feature flags. The image usage flags to convert are
        * specified by 'image_usage'. The corresponding format feature flags are returned.
        * NOTE: The function name stands for "convert image usage to format feature".
        */
        constexpr VkFormatFeatureFlags cvt_iu2ff(VkImageUsageFlags image_usage) noexcept;

        /*
        * Converts a vector of std::string strings to an array of const char*. The vector of std
        * strings is given by 'std_in' and the resulting output vector is stored in 'ccp_out'.
        * This function is mainly used to convert extension and layer names from std::string to 
        * const char* in order to pass them to the corresponding create infos. The size of the
        * array must correspond to the size of the vector.
        * NOTE: The function name stands for "convert std::string to const char pointer vector"
        */
        void cvt_stdstr2ccpv(const std::vector<std::string>& std_in, const char** ccp_out) noexcept;

        /*
        * Converts a vulkan format to a size in bytes. The format to convert is specified by
        * 'format'. The size in bytes is returned.
        */
        size_t format_sizeof(VkFormat format);
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_utility_impl.inl"
#endif
#include "vka_utility_inline_impl.inl"
