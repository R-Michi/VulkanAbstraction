/**
 * @brief Implementation for the device functions.
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
#include <vka/vka.h>

std::vector<VkPhysicalDevice> vka::device::get(VkInstance instance)
{
    std::vector<VkPhysicalDevice> devices;
    uint32_t count;

    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    devices.resize(count);
    vkEnumeratePhysicalDevices(instance, &count, devices.data());

    return devices;
}

bool vka::device::check_requirements(
    const PhysicalDeviceRequirements& requirements,
    [[maybe_unused]] VkInstance instance,
    VkPhysicalDevice device,
    VkPhysicalDeviceProperties* prop,
    VkPhysicalDeviceMemoryProperties* mem_prop
) noexcept
{
    // physical device properties and memory properties
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceMemoryProperties(device, &memory_properties);

    // get all queue family properties
    uint32_t queue_family_count;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
    VkQueueFamilyProperties* const queue_family_properties = (VkQueueFamilyProperties*)alloca(queue_family_count * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_family_properties);

    // check the requirements
    if (properties.deviceType != requirements.type)
        return false;

    if (!detail::device::check_memory_properties(memory_properties, requirements.memoryPropertyFlags, requirements.memoryPropertyFlagsCount))
        return false;

    if (!detail::device::check_queue_flags(queue_family_properties, queue_family_count, requirements.queueFamilyFlags, requirements.queueFamilyFlagsCount))
        return false;

#ifdef _VKA_GLFW
    if (requirements.surfaceSupport && !detail::device::check_surface_support(instance, device, queue_family_count))
        return false;
#endif

    if (!detail::device::check_sequence(properties, requirements.sequence))
        return false;

    // optionally return properties
    if (prop != nullptr)
        *prop = properties;
    if (mem_prop != nullptr)
        *mem_prop = memory_properties;

    return true;
}

uint32_t vka::device::find(
    [[maybe_unused]] VkInstance instance,
    const std::vector<VkPhysicalDevice>& devices,
    const PhysicalDeviceRequirements& requirements,
    VkPhysicalDeviceProperties* prop,
    VkPhysicalDeviceMemoryProperties* mem_prop
) noexcept
{
    for(size_t i = 0; i < devices.size(); i++)
    {
        if (check_requirements(requirements, instance, devices[i], prop, mem_prop))
            return i;
    }
    return NPOS;
}

bool vka::device::supports_layer(VkPhysicalDevice device, std::string_view layer_name, VkLayerProperties* properties) noexcept
{
    uint32_t layer_count;
    VkResult res = vkEnumerateDeviceLayerProperties(device, &layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties* const layer_properties = (VkLayerProperties*)alloca(layer_count * sizeof(VkLayerProperties));
    res = vkEnumerateDeviceLayerProperties(device, &layer_count, layer_properties);
    if (res != VK_SUCCESS) return false;

    // supports_layer from detail::instance can be reused
    const size_t idx = detail::instance::supports_layer(layer_properties, layer_count, layer_name);
    const bool b = (idx != NPOS);
    if (b && properties != nullptr)
        *properties = layer_properties[idx];
    return b;
}

uint32_t vka::device::supports_layers(VkPhysicalDevice device, const std::vector<std::string>& layer_names, VkLayerProperties* properties) noexcept
{
    uint32_t layer_count;
    VkResult res = vkEnumerateDeviceLayerProperties(device, &layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties* const layer_properties = (VkLayerProperties*)alloca(layer_count * sizeof(VkLayerProperties));
    res = vkEnumerateDeviceLayerProperties(device, &layer_count, layer_properties);
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < layer_names.size(); i++)
    {
        size_t j;
        if ((j = detail::instance::supports_layer(layer_properties, layer_count, layer_names[i])) == NPOS)
            return i;
        if (properties != nullptr)
            properties[i] = layer_properties[j];
    }
    return NPOS;
}

bool vka::device::supports_extension(VkPhysicalDevice device, std::string_view extension_name, VkExtensionProperties* properties) noexcept
{
    uint32_t extension_count;
    VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties* const extension_properties = (VkExtensionProperties*)alloca(extension_count * sizeof(VkExtensionProperties));
    res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extension_properties);
    if (res != VK_SUCCESS) return false;

    const size_t idx = detail::instance::supports_extension(extension_properties, extension_count, extension_name);
    const bool b = (idx != NPOS);
    if (b && properties != nullptr)
        *properties = extension_properties[idx];
    return b;
}

uint32_t vka::device::supports_extensions(VkPhysicalDevice device, const std::vector<std::string>& extension_names, VkExtensionProperties* properties) noexcept
{
    uint32_t extension_count;
    VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties* const extension_properties = (VkExtensionProperties*)alloca(extension_count * sizeof(VkExtensionProperties));
    res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extension_properties);
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < extension_names.size(); i++)
    {
        size_t j;
        if ((j = detail::instance::supports_extension(extension_properties, extension_count, extension_names[i])) == NPOS)
            return i;
        if (properties != nullptr)
            properties[i] = extension_properties[j];
    }
    return NPOS;
}

