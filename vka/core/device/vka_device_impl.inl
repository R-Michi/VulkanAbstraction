/**
* @file     vka_device_impl.inl
* @brief    Implemenation of the device related functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

void vka::device::get(VkInstance instance, std::vector<VkPhysicalDevice>& physical_devices)
{
    uint32_t count;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    VkPhysicalDevice devices[count];
    vkEnumeratePhysicalDevices(instance, &count, devices);

    physical_devices.clear();
    physical_devices.insert(physical_devices.end(), devices, devices + count);
}

size_t vka::device::find(VkInstance instance, const std::vector<VkPhysicalDevice>& devices, const PhysicalDeviceFilter& filter, VkPhysicalDeviceProperties* prop, VkPhysicalDeviceMemoryProperties* mem_prop)
{
    VkPhysicalDeviceProperties properties[devices.size()];
    VkPhysicalDeviceMemoryProperties memory_properties;
    std::vector<size_t> candidates;
    for(size_t i = 0; i < devices.size(); i++)
    {
        // get properties and memory properties of all physical devices
        vkGetPhysicalDeviceProperties(devices[i], properties + i);
        vkGetPhysicalDeviceMemoryProperties(devices[i], &memory_properties);

        // get queue family properties
        uint32_t n_queue_families;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &n_queue_families, nullptr);
        VkQueueFamilyProperties queue_family_properties[n_queue_families];
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &n_queue_families, queue_family_properties);

        // check support for everything specified in the filter
        uint32_t failed = 0x0000;
        failed |= detail::device::has_sequence(properties[i], filter.sequence);
        failed |= detail::device::has_memory_properties(memory_properties, filter.memoryPropertyFlags);
        failed |= detail::device::has_queue_flags(queue_family_properties, n_queue_families, filter.queueFamilyFlags);
#ifdef VKA_GLFW_ENABLE
        if (filter.surfaceSupport)
            failed |= detail::device::has_surface_support(instance, devices[i], n_queue_families);
#endif

        // If no check has failed, the current device is a candidate to be used for the application.
        if(failed == 0x0000) candidates.push_back(i);
    }

    // find best matching from the selected candidates
    for(size_t i = 0; i < filter.deviceTypeHirachy.size(); i++)
    {
        for(size_t cur_idx : candidates)
        {
            if (properties[cur_idx].deviceType == filter.deviceTypeHirachy[i])
            {
                if (prop != nullptr) *prop = properties[cur_idx];
                if (mem_prop != nullptr) vkGetPhysicalDeviceMemoryProperties(devices[cur_idx], mem_prop);
                return cur_idx;
            }
        }
    }
    return vka::NPOS;
}

bool vka::device::supports_layer(VkPhysicalDevice device, std::string_view layer_name, VkLayerProperties* properties) noexcept
{
    uint32_t layer_count;
    VkResult res = vkEnumerateDeviceLayerProperties(device, &layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties layer_properties[layer_count];
    res = vkEnumerateDeviceLayerProperties(device, &layer_count, layer_properties);
    if (res != VK_SUCCESS) return false;

    // supports_layer from detail::instance can be reused
    const size_t idx = detail::instance::supports_layer(layer_properties, layer_count, layer_name);
    const bool b = (idx != vka::NPOS);
    if (b && properties != nullptr)
        *properties = layer_properties[idx];
    return b;
}

size_t vka::device::supports_layers(VkPhysicalDevice device, const std::vector<std::string>& layer_names, VkLayerProperties* properties) noexcept
{
    uint32_t layer_count;
    VkResult res = vkEnumerateDeviceLayerProperties(device, &layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties layer_properties[layer_count];
    res = vkEnumerateDeviceLayerProperties(device, &layer_count, layer_properties);
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < layer_names.size(); i++)
    {
        size_t j;
        if ((j = detail::instance::supports_layer(layer_properties, layer_count, layer_names[i])) == vka::NPOS)
            return i;
        if (properties != nullptr)
            properties[i] = layer_properties[j];
    }
    return vka::NPOS;
}

bool vka::device::supports_extension(VkPhysicalDevice device, std::string_view extension_name, VkExtensionProperties* properties) noexcept
{
    uint32_t extension_count;
    VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties extension_properties[extension_count];
    res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extension_properties);
    if (res != VK_SUCCESS) return false;

    const size_t idx = detail::instance::supports_extension(extension_properties, extension_count, extension_name);
    const bool b = (idx != vka::NPOS);
    if (b && properties != nullptr)
        *properties = extension_properties[idx];
    return b;
}

size_t vka::device::supports_extensions(VkPhysicalDevice device, const std::vector<std::string>& extension_names, VkExtensionProperties* properties) noexcept
{
    uint32_t extension_count;
    VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties extension_properties[extension_count];
    res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extension_properties);
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < extension_names.size(); i++)
    {
        size_t j;
        if ((j = detail::instance::supports_extension(extension_properties, extension_count, extension_names[i])) == vka::NPOS)
            return i;
        if (properties != nullptr)
            properties[i] = extension_properties[j];
    }
    return vka::NPOS;
}
