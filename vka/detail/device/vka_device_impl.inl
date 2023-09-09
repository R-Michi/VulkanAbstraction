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

bool vka::detail::device::has_memory_property(const VkPhysicalDeviceMemoryProperties& properties, VkMemoryPropertyFlags req_flags)
{
    for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
    {
        if ((properties.memoryTypes[i].propertyFlags & req_flags) == req_flags)
            return true;
    }
    return false;
}

bool vka::detail::device::has_queue_flag(const VkQueueFamilyProperties* properties, size_t n, VkQueueFlags req_flags)
{
    for (uint32_t i = 0; i < n; i++)
    {
        if ((properties[i].queueFlags & req_flags) == req_flags)
            return true;
    }
    return false;
}


uint16_t vka::detail::device::has_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence)
{
    if (sequence == nullptr) return 0x0000;
    
    std::string device_name = properties.deviceName;
    return ((device_name.find(sequence) == std::string::npos) ? 0x0001 : 0x0000);
}

uint16_t vka::detail::device::has_memory_properties(const VkPhysicalDeviceMemoryProperties& properties, const std::vector<VkMemoryPropertyFlags>& req_flags)
{
    for (VkMemoryPropertyFlags flags : req_flags)
    {
        if (!has_memory_property(properties, flags))
            return 0x0002;
    }
    return 0x0000;
}

uint16_t vka::detail::device::has_queue_flags(const VkQueueFamilyProperties* properties, size_t n, const std::vector<VkQueueFlags>& req_flags)
{
    for (VkQueueFlags flags : req_flags)
    {
        if (!has_queue_flag(properties, n, flags))
            return 0x0004;
    }
    return 0x0000;
}

#ifdef VKA_GLFW_ENABLE
uint16_t vka::detail::device::has_surface_support(const VkInstance instance, const VkPhysicalDevice device, uint32_t qprop_count)
{
    // check if at least one queue family has surface support
    for (uint32_t i = 0; i < qprop_count; i++)
    {
        if (glfwGetPhysicalDevicePresentationSupport(instance, device, i))
            return 0x0000;
    }
    return 0x0200;
}
#endif
