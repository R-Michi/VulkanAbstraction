/**
* @file     vka_instance_impl.inl
* @brief    Implemenation of the instance related functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

bool vka::instance::is_layer_supported(const char* layer_name, VkLayerProperties* _property)
{
    uint32_t n_layers;
    VkResult res = vkEnumerateInstanceLayerProperties(&n_layers, nullptr);  // get number of layers
    if (res != VK_SUCCESS) return false;
    
    VkLayerProperties properties[n_layers];
    res = vkEnumerateInstanceLayerProperties(&n_layers, properties);        // get layer properties
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

bool vka::instance::are_layers_supported(const std::vector<const char*>& layer_names, size_t& idx, std::vector<VkLayerProperties>* _properties)
{
    uint32_t n_layers;
    VkResult res = vkEnumerateInstanceLayerProperties(&n_layers, nullptr);  // get number of layers
    if (res != VK_SUCCESS) return false;
    
    VkLayerProperties properties[n_layers];
    res = vkEnumerateInstanceLayerProperties(&n_layers, properties);        // get layer properties
    if (res != VK_SUCCESS) return false;
    
    // For all given layers...
    for (size_t i = 0; i < layer_names.size(); i++)
    {
        // ...check if the current one is contained within all instance layers.
        bool contains = false;
        for (uint32_t j = 0; j < n_layers && !contains; j++)
        {
            // If layer is supported, the loop ends and its properties are stored.
            if (strcmp(layer_names[i], properties[j].layerName) == 0)   
            {
                contains = true;
                if (_properties != nullptr)
                    _properties->push_back(properties[j]);
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

bool vka::instance::is_extension_supported(const char* extension_name, VkExtensionProperties* _property)
{
    uint32_t n_extensions;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, nullptr); // get number of extensions
    if (res != VK_SUCCESS) return false;
    
    VkExtensionProperties properties[n_extensions];
    res = vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, properties);   // get extension properties
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

bool vka::instance::are_extensions_supported(const std::vector<const char*>& extension_names, size_t& idx, std::vector<VkExtensionProperties>* _properties)
{
    uint32_t n_extensions;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, nullptr); // get number of extensions
    if (res != VK_SUCCESS) return false;
    
    VkExtensionProperties properties[n_extensions];
    res = vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, properties);   // get extension properties
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
                    _properties->push_back(properties[j]);  // add, if layer is contained
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

#ifndef VKA_GLFW_DISABLE
void vka::instance::get_glfw_extensions(std::vector<const char*>& glfw_extensions)
{
    if (!glfwVulkanSupported())
        throw std::runtime_error("[vka::instance::get_glfw_extensions]: Vulkan is not supported.");
    
    uint32_t n_extensions;
    const char** extensions = glfwGetRequiredInstanceExtensions(&n_extensions);
    for (uint32_t i = 0; i < n_extensions; i++)
        glfw_extensions.push_back(extensions[i]);
}
#endif
