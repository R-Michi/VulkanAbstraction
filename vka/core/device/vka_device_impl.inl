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
    physical_devices.clear();
    
    uint32_t n_devices;
    vkEnumeratePhysicalDevices(instance, &n_devices, nullptr);
    VkPhysicalDevice devices[n_devices];
    vkEnumeratePhysicalDevices(instance, &n_devices, devices);
    
    for (size_t i = 0; i < n_devices; i++)
        physical_devices.push_back(devices[i]);
}

size_t vka::device::find(VkInstance instance, const std::vector<VkPhysicalDevice>& devices, const PhysicalDeviceFilter& filter)
{
    VkPhysicalDeviceProperties properties[devices.size()];
    for (size_t i = 0; i < devices.size(); i++)
        vkGetPhysicalDeviceProperties(devices[i], properties + i);

    std::vector<size_t> candidates;
    for(size_t i = 0; i < devices.size(); i++)
    {
        VkPhysicalDeviceMemoryProperties mem_prop = {};
        vkGetPhysicalDeviceMemoryProperties(devices[i], &mem_prop);

        uint32_t n_queue_families;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &n_queue_families, nullptr);
        VkQueueFamilyProperties queue_family_properties[n_queue_families];
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &n_queue_families, queue_family_properties);

        uint16_t failed = 0x0000;

        failed |= detail::device::has_sequence(properties[i], filter.sequence);
        failed |= detail::device::has_memory_properties(mem_prop, filter.memoryPropertyFlags);
        failed |= detail::device::has_queue_flags(queue_family_properties, n_queue_families, filter.queueFamilyFlags);
#ifdef VKA_GLFW_ENABLE
        if (filter.surfaceSupport)
            failed |= detail::device::has_surface_support(instance, devices[i], n_queue_families);
#endif

        if(failed == 0x0000) candidates.push_back(i);
    }

    // find best matching from the selected candidates
    size_t idx = vka::NPOS;
    for(size_t i = 0; i < filter.deviceTypeHirachy.size() && idx == vka::NPOS; i++)
    {
        for(size_t cur_idx : candidates)
        {
            if(properties[cur_idx].deviceType == filter.deviceTypeHirachy[i] && idx == vka::NPOS)
                idx = cur_idx;
        }
    }
    return idx;
}

bool vka::device::supports_layer(VkPhysicalDevice device, const std::string& layer_name, VkLayerProperties* properties)
{
    uint32_t layer_count;
    VkResult res = vkEnumerateDeviceLayerProperties(device, &layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties layer_properties[layer_count];
    res = vkEnumerateDeviceLayerProperties(device, &layer_count, layer_properties);
    if (res != VK_SUCCESS) return false;

    for (uint32_t i = 0; i < layer_count; i++)
    {
        if (layer_name == layer_properties[i].layerName)
        {
            if (properties != nullptr)
                *properties = layer_properties[i];
            return true;
        }
    }
    return false;
}

size_t vka::device::supports_layers(VkPhysicalDevice device, const std::vector<std::string>& layer_names, std::vector<VkLayerProperties>* properties)
{
    uint32_t layer_count;
    VkResult res = vkEnumerateDeviceLayerProperties(device, &layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties layer_properties[layer_count];
    res = vkEnumerateDeviceLayerProperties(device, &layer_count, layer_properties);
    if (res != VK_SUCCESS) return false;

    if (properties != nullptr)
        properties->clear();

    for (size_t i = 0; i < layer_names.size(); i++)
    {
        bool found = false;
        for (uint32_t j = 0; j < layer_count && !found; j++)
        {
            if (layer_names[i] == layer_properties[j].layerName)
            {
                found = true;
                if (properties != nullptr)
                    properties->push_back(layer_properties[j]);
            }
        }
        if (!found) return i;
    }
    return vka::NPOS;
}

bool vka::device::supports_extension(VkPhysicalDevice device, const std::string& extension_name, VkExtensionProperties* properties)
{
    uint32_t extension_count;
    VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties extension_properties[extension_count];
    res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extension_properties);
    if (res != VK_SUCCESS) return false;

    for (uint32_t i = 0; i < extension_count; i++)
    {
        if (extension_name == extension_properties[i].extensionName)
        {
            if (properties != nullptr)
                *properties = extension_properties[i];
            return true;
        }
    }
    return false;
}

size_t vka::device::supports_extensions(VkPhysicalDevice device, const std::vector<std::string>& extension_names, std::vector<VkExtensionProperties>* properties)
{
    uint32_t extension_count;
    VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties extension_properties[extension_count];
    res = vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extension_properties);
    if (res != VK_SUCCESS) return false;

    if (properties != nullptr)
        properties->clear();

    for (size_t i = 0; i < extension_names.size(); i++)
    {
        bool found = false;
        for (uint32_t j = 0; j < extension_count && !found; j++)
        {
            if (extension_names[i] == extension_properties[j].extensionName)
            {
                found = true;
                if (properties != nullptr)
                    properties->push_back(extension_properties[j]);
            }
        }
        if (!found) return i;
    }
    return vka::NPOS;
}
