/**
* @file     vka_device_impl.h
* @brief    Implemenation of the device related functions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "vka_device.h"

void vka::device::get(const VkInstance& instance, std::vector<VkPhysicalDevice>& physical_devices)
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

bool device_has_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence)
{
    if (sequence == nullptr) return true;
    
    std::string device_name = properties.deviceName;
    return (device_name.find(sequence) != std::string::npos);
}

bool device_has_memory_properties(const VkPhysicalDeviceMemoryProperties& mem_prop, const std::vector<VkMemoryPropertyFlags>& req_flags)
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
            return false;
    }
    return true;
}

bool device_has_min_image_count(const VkSurfaceCapabilitiesKHR& surface_capabilities, uint32_t req_min_image_count)
{
    return (surface_capabilities.minImageCount <= req_min_image_count);
}

bool device_has_max_image_count(const VkSurfaceCapabilitiesKHR& surface_capabilities, uint32_t req_max_image_count)
{
    return (surface_capabilities.maxImageCount >= req_max_image_count);
}

bool device_has_image_usage_flags(const VkSurfaceCapabilitiesKHR& surface_capabilities, VkImageUsageFlags flags)
{
    return ((surface_capabilities.supportedUsageFlags & flags) == flags);
}

bool device_has_color_spaces(const VkSurfaceFormatKHR* surface_format, size_t n, const std::vector<VkColorSpaceKHR> color_spaces)
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
            return false;
    }
    return true;
}

bool device_has_color_formats(const VkSurfaceFormatKHR* surface_format, size_t n, const std::vector<VkFormat>& formats)
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
            return false;
    }
    return true;
}

bool device_has_present_modes(const VkPresentModeKHR* present_modes, size_t n, const std::vector<VkPresentModeKHR>& req_present_modes)
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
            return false;
    }
    return true;
}

bool device_has_queue_flags(const VkQueueFamilyProperties* queue_family_properties, size_t n, const std::vector<VkQueueFlags>& req_queue_familiy_flags)
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
            return false;
    }
    return true;
}

vka::PhysicalDeviceError vka::device::find(const std::vector<VkPhysicalDevice>& devices, size_t begin, const PhysicalDeviceFilter& filter, size_t& index)
{
    if (begin >= devices.size()) return VKA_PYHSICAL_DEVICE_ERROR_INVALID_PARAMETER;
    
    using namespace vka;
    PhysicalDeviceError error;
    uint32_t error_mask = 0;
    
    std::vector<VkPhysicalDeviceProperties> device_properties;
    properties(devices, device_properties);
    
    std::vector<size_t> indices;
    
    for (size_t i = begin; i < devices.size(); i++)
    {
        // check for squence
        if (!device_has_sequence(device_properties.at(i), filter.sequence))
        {
            if (!(error_mask & VKA_PYHSICAL_DEVICE_ERROR_SEQUENCE_FAILED))
                error = VKA_PYHSICAL_DEVICE_ERROR_SEQUENCE_FAILED;
            continue;
        }
        else
        {
            error_mask |= VKA_PYHSICAL_DEVICE_ERROR_SEQUENCE_FAILED;
        }

        // check for memory properties
        VkPhysicalDeviceMemoryProperties mem_prop = {};
        vkGetPhysicalDeviceMemoryProperties(devices.at(i), &mem_prop);
        if (!device_has_memory_properties(mem_prop, filter.reqMemoryPropertyFlags))
        {
            if (!(error_mask & VKA_PYHSICAL_DEVICE_ERROR_MEMORY_PROPERTIES_FAILED))
                error = VKA_PYHSICAL_DEVICE_ERROR_MEMORY_PROPERTIES_FAILED;
            continue;
        }
        else
        {
            error_mask |= VKA_PYHSICAL_DEVICE_ERROR_MEMORY_PROPERTIES_FAILED;
        }

        // check for surface capabilities
        // ignore these checks if no surface is used
        if (filter.pSurface != nullptr)
        {
            VkSurfaceCapabilitiesKHR surface_capabilities = {};
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices.at(i), *filter.pSurface, &surface_capabilities);
            if (!device_has_min_image_count(surface_capabilities, filter.reqMinImageCount))
            {
                if (!(error_mask & VKA_PYHSICAL_DEVICE_ERROR_MIN_IMAGE_COUNT_FAILED))
                    error = VKA_PYHSICAL_DEVICE_ERROR_MIN_IMAGE_COUNT_FAILED;
                continue;
            }
            else
            {
                error_mask |= VKA_PYHSICAL_DEVICE_ERROR_MIN_IMAGE_COUNT_FAILED;
            }

            if (!device_has_max_image_count(surface_capabilities, filter.reqMaxImageCount))
            {
                if (!(error_mask & VKA_PYHSICAL_DEVICE_ERROR_MAX_IMAGE_COUNT_FAILED))
                    error = VKA_PYHSICAL_DEVICE_ERROR_MAX_IMAGE_COUNT_FAILED;
                continue;
            }
            else
            {
                error_mask |= VKA_PYHSICAL_DEVICE_ERROR_MAX_IMAGE_COUNT_FAILED;
            }

            if (!device_has_image_usage_flags(surface_capabilities, filter.reqSurfaceImageUsageFlags))
            {
                if (!(error_mask & VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_USAGE_FAILED))
                    error = VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_USAGE_FAILED;
                continue;
            }
            else
            {
                error_mask |= VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_USAGE_FAILED;
            }

            uint32_t n_formats;
            vkGetPhysicalDeviceSurfaceFormatsKHR(devices.at(i), *filter.pSurface, &n_formats, nullptr);
            VkSurfaceFormatKHR formats[n_formats];
            vkGetPhysicalDeviceSurfaceFormatsKHR(devices.at(i), *filter.pSurface, &n_formats, formats);
            if (!device_has_color_formats(formats, n_formats, filter.reqSurfaceColorFormats))
            {
                if (!(error_mask & VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_FORMATS_FAILED))
                    error = VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_FORMATS_FAILED;
                continue;
            }
            else
            {
                error_mask |= VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_FORMATS_FAILED;
            }

            if (!device_has_color_spaces(formats, n_formats, filter.reqSurfaceColorSpaces))
            {
                if (!(error_mask & VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_SPACE_FAILED))
                    error = VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_SPACE_FAILED;
                continue;
            }
            else
            {
                error_mask |= VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_SPACE_FAILED;
            }

            uint32_t n_present_modes;
            vkGetPhysicalDeviceSurfacePresentModesKHR(devices.at(i), *filter.pSurface, &n_present_modes, nullptr);
            VkPresentModeKHR present_modes[n_present_modes];
            vkGetPhysicalDeviceSurfacePresentModesKHR(devices.at(i), *filter.pSurface, &n_present_modes, present_modes);
            if (!device_has_present_modes(present_modes, n_present_modes, filter.reqPresentModes))
            {
                if (!(error_mask & VKA_PYHSICAL_DEVICE_ERROR_PRESENT_MODE_FAILD))
                    error = VKA_PYHSICAL_DEVICE_ERROR_PRESENT_MODE_FAILD;
                continue;
            }
            else
            {
                error_mask |= VKA_PYHSICAL_DEVICE_ERROR_PRESENT_MODE_FAILD;
            }
        }

        // check for queue family flags
        uint32_t n_queue_families;
        vkGetPhysicalDeviceQueueFamilyProperties(devices.at(i), &n_queue_families, nullptr);
        VkQueueFamilyProperties queue_family_properties[n_queue_families];
        vkGetPhysicalDeviceQueueFamilyProperties(devices.at(i), &n_queue_families, queue_family_properties);
        if (!device_has_queue_flags(queue_family_properties, n_queue_families, filter.reqQueueFamilyFlags))
        {
            if (!(error_mask & VKA_PYHSICAL_DEVICE_ERROR_QUEUE_FLAGS_FAILED))
                error = VKA_PYHSICAL_DEVICE_ERROR_QUEUE_FLAGS_FAILED;
            continue;
        }
        else
        {
            error_mask |= VKA_PYHSICAL_DEVICE_ERROR_QUEUE_FLAGS_FAILED;
        }
        
        indices.push_back(i);
        error = VKA_PYHSICAL_DEVICE_ERROR_NONE;
    }
    
    if (error != VKA_PYHSICAL_DEVICE_ERROR_NONE)
        return error;

    // get best matching device candidate
    bool found = false;
    for (size_t i = 0; i < filter.reqDeviceTypeHirachy.size() && !found; i++)
    {
        for (size_t idx : indices)
        {
            if (device_properties.at(idx).deviceType == filter.reqDeviceTypeHirachy.at(i) && !found)
            {
                found = true;
                index = idx;
            }
        }
    }
    if (!found)
        return VKA_PYHSICAL_DEVICE_ERROR_DEVICE_TYPE_FAILED;
    
    return VKA_PYHSICAL_DEVICE_ERROR_NONE;
}

const char* vka::device::strerror(vka::PhysicalDeviceError error)
{
    using namespace vka;
    
    switch (error)
    {
    case VKA_PYHSICAL_DEVICE_ERROR_NONE:
        return "No error!";
    case VKA_PYHSICAL_DEVICE_ERROR_SEQUENCE_FAILED:
        return "No physical device found with requiered sequence!";
    case VKA_PYHSICAL_DEVICE_ERROR_MEMORY_PROPERTIES_FAILED:
        return "No physical device found with required memory properties!";
    case VKA_PYHSICAL_DEVICE_ERROR_MIN_IMAGE_COUNT_FAILED:
        return "No physical device found with required minimal image count!";
    case VKA_PYHSICAL_DEVICE_ERROR_MAX_IMAGE_COUNT_FAILED:
        return "No physical device found with required maximal image count!";
    case VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_USAGE_FAILED:
        return "No physical device found with required image usage flags!";
    case VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_FORMATS_FAILED:
        return "No physical device found with required color formats!";
    case VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_SPACE_FAILED:
        return "No physical device found with required color spaces!";
    case VKA_PYHSICAL_DEVICE_ERROR_PRESENT_MODE_FAILD:
        return "No physical device found with required presentation modes!";
    case VKA_PYHSICAL_DEVICE_ERROR_QUEUE_FLAGS_FAILED:
        return "No physical device found with required queue flags!";
    case VKA_PYHSICAL_DEVICE_ERROR_DEVICE_TYPE_FAILED:
        return "No physical device found with required device type!";
    case VKA_PYHSICAL_DEVICE_ERROR_INVALID_PARAMETER:
        return "Invalid parameter!";
    default:
        return "Unknown error!";
    }
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

bool vka::device::are_layers_supported(VkPhysicalDevice device, std::vector<const char*> layer_names, std::vector<VkLayerProperties>* _properties)
{
    uint32_t n_layers;
    VkResult res = vkEnumerateDeviceLayerProperties(device, &n_layers, nullptr);    // get number of layers
    if (res != VK_SUCCESS) return false;

    VkLayerProperties properties[n_layers];
    res = vkEnumerateDeviceLayerProperties(device, &n_layers, properties);  // get layer properties
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < layer_names.size(); i++)
    {
        bool contains = false;
        for (uint32_t j = 0; j < n_layers; j++)
        {
            if (strcmp(layer_names[i], properties[j].layerName) == 0)   // check if current layer is supported
            {
                contains = true;
                if (_properties != nullptr)
                    _properties->push_back(properties[j]);  // add, if layer is contained
            }
        }

        if (!contains)
        {
            if (_properties != nullptr)
                _properties->clear();   // if one layer is not contained, clear property vector
            return false;
        }
    }

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

bool vka::device::are_extensions_supported(VkPhysicalDevice device, std::vector<const char*> extension_names, std::vector<VkExtensionProperties>* _properties)
{
    uint32_t n_extensions;
    VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, nullptr);   // get number of layers
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties properties[n_extensions];
    res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, properties); // get layer properties
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < extension_names.size(); i++)
    {
        bool contains = false;
        for (uint32_t j = 0; j < n_extensions; j++)
        {
            if (strcmp(extension_names[i], properties[j].extensionName) == 0)   // check if current layer is supported
            {
                contains = true;
                if (_properties != nullptr)
                    _properties->push_back(properties[j]);  // add, if layer is contained
            }
        }

        if (!contains)
        {
            if (_properties != nullptr)
                _properties->clear();   // if one layer is not contained, clear property vector
            return false;
        }
    }

    return true;
}
