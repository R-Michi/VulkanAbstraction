/**
 * @brief Implementation for the device functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

std::vector<VkPhysicalDevice> vka::device::get(VkInstance instance)
{
    std::vector<VkPhysicalDevice> devices;
    uint32_t count;

    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    devices.resize(count);
    vkEnumeratePhysicalDevices(instance, &count, devices.data());

    return devices;
}

VkPhysicalDevice vka::device::find(
    [[maybe_unused]] VkInstance instance,
    const std::vector<VkPhysicalDevice>& devices,
    const PhysicalDeviceFilter& filter,
    VkPhysicalDeviceProperties* prop,
    VkPhysicalDeviceMemoryProperties* mem_prop
)
{
    VkPhysicalDeviceProperties* const properties = (VkPhysicalDeviceProperties*)alloca(devices.size() * sizeof(VkPhysicalDeviceProperties));
    VkPhysicalDeviceMemoryProperties memory_properties;
    std::vector<size_t> candidates;
    for(size_t i = 0; i < devices.size(); i++)
    {
        // get properties and memory properties of all physical devices
        vkGetPhysicalDeviceProperties(devices[i], properties + i);
        vkGetPhysicalDeviceMemoryProperties(devices[i], &memory_properties);

        // get to queue family properties
        uint32_t n_queue_families;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &n_queue_families, nullptr);
        VkQueueFamilyProperties* const queue_family_properties = (VkQueueFamilyProperties*)alloca(n_queue_families * sizeof(VkQueueFamilyProperties));
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &n_queue_families, queue_family_properties);

        // check support for everything specified in the filter
        uint8_t failed = 0;
        failed |= detail::device::has_sequence(properties[i], filter.sequence);
        failed |= detail::device::has_memory_properties(memory_properties, filter.memoryPropertyFlags);
        failed |= detail::device::has_queue_flags(queue_family_properties, n_queue_families, filter.queueFamilyFlags);
#ifdef VKA_GLFW_ENABLE
        if (filter.surfaceSupport)
            failed |= detail::device::has_surface_support(instance, devices[i], n_queue_families);
#endif

        // If no check has failed, the current device is a candidate to be used for the application.
        if(failed == 0) candidates.push_back(i);
    }

    // find the best matching from the selected candidates
    for (const VkPhysicalDeviceType type : filter.deviceTypeHierarchy)
    {
        for (const size_t cur_idx : candidates)
        {
            VkPhysicalDevice cur_device = devices[cur_idx];
            if (properties[cur_idx].deviceType == type)
            {
                if (prop != nullptr) *prop = properties[cur_idx];
                if (mem_prop != nullptr) vkGetPhysicalDeviceMemoryProperties(cur_device, mem_prop);
                return cur_device;
            }
        }
    }
    return VK_NULL_HANDLE;
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

size_t vka::device::supports_layers(VkPhysicalDevice device, const std::vector<std::string>& layer_names, VkLayerProperties* properties) noexcept
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

size_t vka::device::supports_extensions(VkPhysicalDevice device, const std::vector<std::string>& extension_names, VkExtensionProperties* properties) noexcept
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
