/**
* @file     vka_device.h
* @brief    Detailed declaration of device functions.
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
    namespace detail
    {
        namespace device
        {
            /*
            * Checks, if one combination of memory property flags, specified by 'req_flags', is
            * supported. Returns true, if those flags are supported and false, if not.
            */
            inline bool has_memory_property(const VkPhysicalDeviceMemoryProperties& properties, VkMemoryPropertyFlags req_flags) noexcept;

            /*
            * Checks, if one combination of queue flags, specified by 'req_flags', is supported.
            * Returns true, if those flags are supported and false, if not.
            */
            inline bool has_queue_flag(const VkQueueFamilyProperties* properties, size_t n, VkQueueFlags req_flags) noexcept;

            /*
            * Checks, if the device name contains a specific sequence. The sequence to search for
            * is specified by 'sequence' and the device name is contained within the
            * VkPhysicalDeviceProperties structure which is specified by 'properties'. 
            * Returns 0x0000, if the sequence was found and 0x0001, if not.
            */
            inline uint32_t has_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence) noexcept;
            
            /*
            * Checks, if all combinations of memory propertiy flags, specified by 'req_flags', are
            * supported. Returns 0x0000, if all flag combinations are supported and 0x0002, if not.
            */
            inline uint32_t has_memory_properties(const VkPhysicalDeviceMemoryProperties& properties, const std::vector<VkMemoryPropertyFlags>& req_flags) noexcept;

            /*
            * Checks, if all combinations of queue flags, specified by 'req_flags', are supported.
            * Returns 0x0000, if all flag combinations are supported and 0x0004, if not.
            */
            inline uint32_t has_queue_flags(const VkQueueFamilyProperties* properties, size_t n, const std::vector<VkQueueFlags>& req_flags) noexcept;

#ifdef VKA_GLFW_ENABLE // requieres glfw
            /*
            * Checks, if any queue family of a given physical device has surface support.
            * The physical device is specified by 'device', the number of queue families is
            * specified by 'qfamily_count'. Furthermore the vulkan instance is requiered and is
            * specified by 'instance'. If any queue family has surface support, 0x0000 is returned
            * and 0x0008 otherwise.
            */
            inline uint32_t has_surface_support(const VkInstance instance, const VkPhysicalDevice device, uint32_t qfamily_count) noexcept;
#endif

        } // namespace device 
    } // namespace detail
} // namespace vka

#ifdef VKA_IMPLEMENTATION
    #include "vka_device_impl.inl"
#endif
