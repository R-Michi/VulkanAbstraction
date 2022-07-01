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
            uint16_t has_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence);
            uint16_t has_memory_properties(const VkPhysicalDeviceMemoryProperties& mem_prop, const std::vector<VkMemoryPropertyFlags>& req_flags);
            uint16_t has_min_image_count(const VkSurfaceCapabilitiesKHR& surface_capabilities, uint32_t req_min_image_count);
            uint16_t has_max_image_count(const VkSurfaceCapabilitiesKHR& surface_capabilities, uint32_t req_max_image_count);
            uint16_t has_image_usage_flags(const VkSurfaceCapabilitiesKHR& surface_capabilities, VkImageUsageFlags flags);
            uint16_t has_color_formats(const VkSurfaceFormatKHR* surface_format, size_t n, const std::vector<VkFormat>& formats);
            uint16_t has_color_spaces(const VkSurfaceFormatKHR* surface_format, size_t n, const std::vector<VkColorSpaceKHR> color_spaces);
            uint16_t has_present_modes(const VkPresentModeKHR* present_modes, size_t n, const std::vector<VkPresentModeKHR>& req_present_modes);
            uint16_t has_queue_flags(const VkQueueFamilyProperties* queue_family_properties, size_t n, const std::vector<VkQueueFlags>& req_queue_familiy_flags);
        } // namespace device 
    } // namespace detail
} // namespace vka

#ifdef VKA_IMPLEMENTATION
    #include "vka_device_impl.inl"
#endif
