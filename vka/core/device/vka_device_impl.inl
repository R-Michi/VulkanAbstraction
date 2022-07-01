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

void vka::device::properties(const std::vector<VkPhysicalDevice>& devices, std::vector<VkPhysicalDeviceProperties>& properties)
{
    properties.clear();
    for (const VkPhysicalDevice& device : devices)
    {
        VkPhysicalDeviceProperties prop;
        vkGetPhysicalDeviceProperties(device, &prop);
        properties.push_back(prop);
    }
}

size_t vka::device::find(const std::vector<VkPhysicalDevice>& devices, size_t begin, const PhysicalDeviceFilter& filter)
{
    if(begin >= devices.size()) return VKA_NPOS;
    
    std::vector<size_t> candidates;
    std::vector<VkPhysicalDeviceProperties> device_properties;
    properties(devices, device_properties);
    
    for(size_t i = begin; i < devices.size(); i++)
    {
        VkPhysicalDeviceMemoryProperties mem_prop = {};
        vkGetPhysicalDeviceMemoryProperties(devices.at(i), &mem_prop);

        uint32_t n_queue_families;
        vkGetPhysicalDeviceQueueFamilyProperties(devices.at(i), &n_queue_families, nullptr);
        VkQueueFamilyProperties queue_family_properties[n_queue_families];
        vkGetPhysicalDeviceQueueFamilyProperties(devices.at(i), &n_queue_families, queue_family_properties);

        uint16_t failed = 0x0000;

        failed |= detail::device::has_sequence(device_properties.at(i), filter.sequence);
        failed |= detail::device::has_memory_properties(mem_prop, filter.memoryPropertyFlags);
        failed |= detail::device::has_queue_flags(queue_family_properties, n_queue_families, filter.queueFamilyFlags);

        // ignore these checks if no surface is used
        if(filter.pSurface != nullptr)
        {
            VkSurfaceCapabilitiesKHR surface_capabilities = {};
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices.at(i), *filter.pSurface, &surface_capabilities);

            uint32_t n_formats;
            vkGetPhysicalDeviceSurfaceFormatsKHR(devices.at(i), *filter.pSurface, &n_formats, nullptr);
            VkSurfaceFormatKHR formats[n_formats];
            vkGetPhysicalDeviceSurfaceFormatsKHR(devices.at(i), *filter.pSurface, &n_formats, formats);

            uint32_t n_present_modes;
            vkGetPhysicalDeviceSurfacePresentModesKHR(devices.at(i), *filter.pSurface, &n_present_modes, nullptr);
            VkPresentModeKHR present_modes[n_present_modes];
            vkGetPhysicalDeviceSurfacePresentModesKHR(devices.at(i), *filter.pSurface, &n_present_modes, present_modes);

            failed |= detail::device::has_min_image_count(surface_capabilities, filter.swapchainMinImageCount);
            failed |= detail::device::has_max_image_count(surface_capabilities, filter.swapchainMaxImageCount);
            failed |= detail::device::has_image_usage_flags(surface_capabilities, filter.surfaceImageUsageFlags);
            failed |= detail::device::has_color_formats(formats, n_formats, filter.surfaceColorFormats);
            failed |= detail::device::has_color_spaces(formats, n_formats, filter.surfaceColorSpaces);
            failed |= detail::device::has_present_modes(present_modes, n_present_modes, filter.surfacePresentModes);
        }
        if(failed == 0x0000) candidates.push_back(i);
    }

    // find best matching from the selected candidates
    size_t idx = VKA_NPOS;
    for(size_t i = 0; i < filter.deviceTypeHirachy.size() && idx == VKA_NPOS; i++)
    {
        for(size_t cur_idx : candidates)
        {
            if(device_properties.at(cur_idx).deviceType == filter.deviceTypeHirachy.at(i) && idx == VKA_NPOS)
                idx = cur_idx;
        }
    }
    return idx;
}

bool vka::device::is_layer_supported(VkPhysicalDevice device, const char* layer_name, VkLayerProperties* _property)
{
    uint32_t n_layers;
    VkResult res = vkEnumerateDeviceLayerProperties(device, &n_layers, nullptr);    // get number of layers
    if (res != VK_SUCCESS) return false;

    VkLayerProperties properties[n_layers];
    res = vkEnumerateDeviceLayerProperties(device, &n_layers, properties);  // get layer properties
    if (res != VK_SUCCESS) return false;

    for (uint32_t i = 0; i < n_layers; i++)
    {
        if (strcmp(layer_name, properties[i].layerName) == 0)
        {
            if (_property != nullptr)
                *_property = properties[i];
            return true;
        }
    }

    return false;
}

bool vka::device::are_layers_supported(VkPhysicalDevice device, const std::vector<const char*>& layer_names, size_t& idx, std::vector<VkLayerProperties>* _properties)
{
    uint32_t n_layers;
    VkResult res = vkEnumerateDeviceLayerProperties(device, &n_layers, nullptr);    // get number of layers
    if (res != VK_SUCCESS) return false;

    VkLayerProperties properties[n_layers];
    res = vkEnumerateDeviceLayerProperties(device, &n_layers, properties);  // get layer properties
    if (res != VK_SUCCESS) return false;

    // For all given layers...
    for (size_t i = 0; i < layer_names.size(); i++)
    {
        // ...check if the current one is contained within all instance layers.
        bool contains = false;
        for (uint32_t j = 0; j < n_layers; j++)
        {
            // If layer is supported, the loop ends and its properties are stored.
            if (strcmp(layer_names[i], properties[j].layerName) == 0)
            {
                contains = true;
                if (_properties != nullptr)
                    _properties->push_back(properties[j]);  // add, if layer is contained
            }
        }

        // If the current layer is not contained, clear all properties and return the not
        // supported index within the layer names vector.
        if (!contains)
        {
            if (_properties != nullptr)
                _properties->clear();

            idx = i;
            return false;
        }
    }

    // max size_t value for invalid index
    idx = VKA_INVALID_SIZE;
    return true;
}

bool vka::device::is_extension_supported(VkPhysicalDevice device, const char* extension_name, VkExtensionProperties* _property)
{
    uint32_t n_extensions;
    VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, nullptr);   // get number of extensions
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties properties[n_extensions];
    res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, properties); // get extension properties
    if (res != VK_SUCCESS) return false;

    for (uint32_t i = 0; i < n_extensions; i++)
    {
        if (strcmp(extension_name, properties[i].extensionName) == 0)
        {
            if (_property != nullptr)
                *_property = properties[i];
            return true;
        }
    }

    return false;
}

bool vka::device::are_extensions_supported(VkPhysicalDevice device, const std::vector<const char*>& extension_names, size_t& idx, std::vector<VkExtensionProperties>* _properties)
{
    uint32_t n_extensions;
    VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, nullptr); // get number of extensions
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties properties[n_extensions];
    res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, properties); // get extension properties
    if (res != VK_SUCCESS) return false;

    // For all given extensions...
    for (size_t i = 0; i < extension_names.size(); i++)
    {
        // ...check if the current one is contained within all instance extensions.
        bool contains = false;
        for (uint32_t j = 0; j < n_extensions; j++)
        {
            // If extension is supported, the loop ends and its properties are stored.
            if (strcmp(extension_names[i], properties[j].extensionName) == 0)
            {
                contains = true;
                if (_properties != nullptr)
                    _properties->push_back(properties[j]);  // add, if extension is contained
            }
        }

        // If the current extension is not contained, clear all properties and return the not
        // supported index within the extension names vector.
        if (!contains)
        {
            if (_properties != nullptr)
                _properties->clear();

            idx = i;
            return false;
        }
    }

    idx = VKA_INVALID_SIZE;
    return true;
}
