/**
* @file     vka_device_impl.inl
* @brief    Detailed implemenation of device functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

uint16_t vka::detail::device::has_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence)
{
    if (sequence == nullptr) return 0x0000;
    
    std::string device_name = properties.deviceName;
    return ((device_name.find(sequence) == std::string::npos) ? 0x0001 : 0x0000);
}
uint16_t vka::detail::device::has_memory_properties(const VkPhysicalDeviceMemoryProperties& mem_prop, const std::vector<VkMemoryPropertyFlags>& req_flags)
{
    for (VkMemoryPropertyFlags req_flag : req_flags)
    {
        bool flag_found = false;
        for (uint32_t i = 0; i < mem_prop.memoryTypeCount; i++)
        {
            if (mem_prop.memoryTypes[i].propertyFlags != 0 && (mem_prop.memoryTypes[i].propertyFlags & req_flag) == req_flag)
                flag_found = true;
        }
        if (!flag_found)
            return 0x0002;
    }
    return 0x0000;
}
uint16_t vka::detail::device::has_min_image_count(const VkSurfaceCapabilitiesKHR& surface_capabilities, uint32_t req_min_image_count)
{
    return ((surface_capabilities.minImageCount > req_min_image_count) ? 0x0004 : 0x0000);
}
uint16_t vka::detail::device::has_max_image_count(const VkSurfaceCapabilitiesKHR& surface_capabilities, uint32_t req_max_image_count)
{
    return ((surface_capabilities.maxImageCount < req_max_image_count) ? 0x0008 : 0x0000);
}
uint16_t vka::detail::device::has_image_usage_flags(const VkSurfaceCapabilitiesKHR& surface_capabilities, VkImageUsageFlags flags)
{
    return (((surface_capabilities.supportedUsageFlags & flags) != flags) ? 0x0010 : 0x0000);
}
uint16_t vka::detail::device::has_color_formats(const VkSurfaceFormatKHR* surface_format, size_t n, const std::vector<VkFormat>& formats)
{
    for (VkFormat format : formats)
    {
        bool found = false;
        for (size_t i = 0; i < n; i++)
        {
            if (surface_format[i].format == format)
                found = true;
        }
        if (!found)
            return 0x0020;
    }
    return 0x0000;
}
uint16_t vka::detail::device::has_color_spaces(const VkSurfaceFormatKHR* surface_format, size_t n, const std::vector<VkColorSpaceKHR> color_spaces)
{
    for (VkColorSpaceKHR space : color_spaces)
    {
        bool found = false;
        for (size_t i = 0; i < n; i++)
        {
            if (surface_format[i].colorSpace == space)
                found = true;
        }
        if (!found)
            return 0x0040;
    }
    return 0x0000;
}
uint16_t vka::detail::device::has_present_modes(const VkPresentModeKHR* present_modes, size_t n, const std::vector<VkPresentModeKHR>& req_present_modes)
{
    for (const VkPresentModeKHR& mode : req_present_modes)
    {
        bool found = false;
        for (size_t i = 0; i < n; i++)
        {
            if (present_modes[i] == mode)
                found = true;
        }
        if (!found)
            return 0x0080;
    }
    return 0x0000;
}
uint16_t vka::detail::device::has_queue_flags(const VkQueueFamilyProperties* queue_family_properties, size_t n, const std::vector<VkQueueFlags>& req_queue_familiy_flags)
{
    for (VkQueueFlags flags : req_queue_familiy_flags)
    {
        bool found = false;
        for (size_t i = 0; i < n; i++)
        {
            if ((queue_family_properties[i].queueFlags & flags) == flags)
                found = true;
        }
        if (!found)
            return 0x0100;
    }
    return 0x0000;
}
