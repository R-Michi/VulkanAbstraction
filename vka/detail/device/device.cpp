/**
 * @brief Implementation details for the device functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifdef _VKA_GLFW
#define VKA_INCLUDE_GLFW
#define VKA_GLFW_ENABLE
#endif

#include <vulkan/vulkan.h>
#include "../../vka.h"

/**
 * @brief Checks if one combination of memory property flags is supported.
 * @param properties Specifies the memory properties.
 * @param flags Specifies the required flags.
 * @return Returns true if the flags are supported.
 */
static bool has_memory_property(const VkPhysicalDeviceMemoryProperties& properties, VkMemoryPropertyFlags flags) noexcept
{
    for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
    {
        if ((properties.memoryTypes[i].propertyFlags & flags) == flags)
            return true;
    }
    return false;
}

/**
 * @brief Checks if one combination of queue flags is supported.
 * @param properties Specifies the queue family properties.
 * @param queue_family_count Specifies the number of properties.
 * @param flags Specifies the required flags.
 * @return Returns true if the flags are supported.
 */
static bool has_queue_flag(const VkQueueFamilyProperties* properties, uint32_t queue_family_count, VkQueueFlags flags) noexcept
{
    for (uint32_t i = 0; i < queue_family_count; i++)
    {
        if ((properties[i].queueFlags & flags) == flags)
            return true;
    }
    return false;
}

/**
 * @brief Checks if all combinations of memory property flags are supported.
 * @param properties Specifies the memory properties.
 * @param flags Specifies the required flags.
 * @param count Specifies the number of flags.
 * @return Returns true if all flags are supported.
 */
static bool check_memory_properties(const VkPhysicalDeviceMemoryProperties& properties, const VkMemoryPropertyFlags* flags, uint32_t count) noexcept
{
    // all memory property flags must be supported
    for (uint32_t i = 0; i < count; i++)
    {
        if (!has_memory_property(properties, flags[i]))
            return false;
    }
    return true;
}

/**
 * @brief Checks if all combinations of queue flags are supported.
 * @param properties Specifies the queue family properties.
 * @param queue_family_count Specifies the number of properties.
 * @param flags Specifies the required flags.
 * @param count Specifies the number of flags.
 * @return Returns true if all flags are supported.
 */
static bool check_queue_flags(const VkQueueFamilyProperties* properties, uint32_t queue_family_count, const VkQueueFlags* flags, uint32_t count) noexcept
{
    // all queue flags must be supported
    for (uint32_t i = 0; i < count; i++)
    {
        if (!has_queue_flag(properties, queue_family_count, flags[i]))
            return false;
    }
    return true;
}

#ifdef _VKA_GLFW // requires glfw
/**
 * @brief Checks if any queue family has surface support.
 * @param instance Specifies the vulkan instance.
 * @param device Specifies the physical device.
 * @param queue_family_count Specifies the number of queue families.
 * @return Returns true if surfaces are supported.
 */
static bool check_surface_support(VkInstance instance, VkPhysicalDevice device, uint32_t queue_family_count) noexcept
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

/**
 * @brief Checks if the device name has a specific sequence.
 * @param properties Specifies the physical device properties containing the device name.
 * @param sequence Specifies the sequence the device name has to contain.
 * @return Returns true if it contains the specified sequence.
 */
static bool check_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence) noexcept
{
    if (sequence == nullptr) return true;

    const std::string_view device_name = properties.deviceName;
    return device_name.find(sequence) != std::string::npos;
}

bool vka::detail::device::check_requirements(
    const PhysicalDeviceRequirements& requirements,
    [[maybe_unused]] VkInstance instance,
    [[maybe_unused]] VkPhysicalDevice device,
    const VkPhysicalDeviceProperties& properties,
    const VkPhysicalDeviceMemoryProperties& memory_properties,
    const VkQueueFamilyProperties* queue_family_properties,
    uint32_t queue_family_count
) noexcept
{
    if (properties.deviceType != requirements.type) return false;
    if (!check_memory_properties(memory_properties, requirements.memoryPropertyFlags, requirements.memoryPropertyFlagsCount)) return false;
    if (!check_queue_flags(queue_family_properties, queue_family_count, requirements.queueFamilyFlags, requirements.queueFamilyFlagsCount)) return false;
#ifdef _VKA_GLFW
    if (requirements.surfaceSupport && !check_surface_support(instance, device, queue_family_count)) return false;
#endif
    if (!check_sequence(properties, requirements.sequence)) return false;
    return true;
}
