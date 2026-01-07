/**
 * @brief Implementation details for the device functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include <vka/vka.h>

static bool has_memory_property(const VkPhysicalDeviceMemoryProperties& properties, VkMemoryPropertyFlags flags) noexcept
{
    for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
    {
        if ((properties.memoryTypes[i].propertyFlags & flags) == flags)
            return true;
    }
    return false;
}

static bool has_queue_flag(const VkQueueFamilyProperties* properties, uint32_t queue_family_count, VkQueueFlags flags) noexcept
{
    for (uint32_t i = 0; i < queue_family_count; i++)
    {
        if ((properties[i].queueFlags & flags) == flags)
            return true;
    }
    return false;
}

bool vka::detail::device::check_memory_properties(const VkPhysicalDeviceMemoryProperties& properties, const VkMemoryPropertyFlags* flags, uint32_t count) noexcept
{
    // all memory property flags must be supported
    for (uint32_t i = 0; i < count; i++)
    {
        if (!has_memory_property(properties, flags[i]))
            return false;
    }
    return true;
}

bool vka::detail::device::check_queue_flags(const VkQueueFamilyProperties* properties, uint32_t queue_family_count, const VkQueueFlags* flags, uint32_t count) noexcept
{
    // all queue flags must be supported
    for (uint32_t i = 0; i < count; i++)
    {
        if (!has_queue_flag(properties, queue_family_count, flags[i]))
            return false;
    }
    return true;
}

#ifdef VKA_GLFW_ENABLE // requires glfw
bool vka::detail::device::check_surface_support(VkInstance instance, VkPhysicalDevice device, uint32_t queue_family_count) noexcept
{
    // check if at least one queue family has surface support
    for (uint32_t i = 0; i < queue_family_count; i++)
    {
        if (glfwGetPhysicalDevicePresentationSupport(instance, device, i))
            return true;
    }
    return false;
}
#endif

bool vka::detail::device::check_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence) noexcept
{
    if (sequence == nullptr) return true;

    const std::string_view device_name = properties.deviceName;
    return device_name.find(sequence) != std::string::npos;
}
