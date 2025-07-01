/**
 * @brief Implementation details for the device functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#pragma once

inline bool vka::detail::device::has_memory_property(const VkPhysicalDeviceMemoryProperties& properties, VkMemoryPropertyFlags req_flags) noexcept
{
    for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
    {
        if ((properties.memoryTypes[i].propertyFlags & req_flags) == req_flags)
            return true;
    }
    return false;
}

inline bool vka::detail::device::has_queue_flag(const VkQueueFamilyProperties* properties, size_t n, VkQueueFlags req_flags) noexcept
{
    for (uint32_t i = 0; i < n; i++)
    {
        if ((properties[i].queueFlags & req_flags) == req_flags)
            return true;
    }
    return false;
}

inline uint8_t vka::detail::device::has_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence) noexcept
{
    if (sequence == nullptr) return 0;

    const std::string_view device_name = properties.deviceName;
    return device_name.find(sequence) == std::string::npos ? 0x01 : 0;
}

inline uint8_t vka::detail::device::has_memory_properties(const VkPhysicalDeviceMemoryProperties& properties, const std::vector<VkMemoryPropertyFlags>& req_flags) noexcept
{
    // all memory property flags must be supported
    for (const VkMemoryPropertyFlags flags : req_flags)
    {
        if (!has_memory_property(properties, flags))
            return 0x02;
    }
    return 0;
}

inline uint8_t vka::detail::device::has_queue_flags(const VkQueueFamilyProperties* properties, size_t n, const std::vector<VkQueueFlags>& req_flags) noexcept
{
    // all queue flags must be supported
    for (const VkQueueFlags flags : req_flags)
    {
        if (!has_queue_flag(properties, n, flags))
            return 0x04;
    }
    return 0;
}

#ifdef VKA_GLFW_ENABLE
inline uint8_t vka::detail::device::has_surface_support(VkInstance instance, VkPhysicalDevice device, uint32_t queue_family_count) noexcept
{
    // check if at least one queue family has surface support
    for (uint32_t i = 0; i < queue_family_count; i++)
    {
        if (glfwGetPhysicalDevicePresentationSupport(instance, device, i))
            return 0;
    }
    return 0x08;
}
#endif


