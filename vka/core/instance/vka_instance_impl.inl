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

bool vka::instance::supports_layer(const std::string& layer_name, VkLayerProperties* properties)
{
    uint32_t layer_count;
    VkResult res = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties layer_properties[layer_count];
    res = vkEnumerateInstanceLayerProperties(&layer_count, layer_properties);
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

size_t vka::instance::supports_layers(const std::vector<std::string>& layer_names, std::vector<VkLayerProperties>* properties)
{
    uint32_t layer_count;
    VkResult res = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties layer_properties[layer_count];
    res = vkEnumerateInstanceLayerProperties(&layer_count, layer_properties);
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

bool vka::instance::supports_extension(const std::string& extension_name, VkExtensionProperties* properties)
{
    uint32_t extension_count;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties extension_properties[extension_count];
    res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties);
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

size_t vka::instance::supports_extensions(const std::vector<std::string>& extension_names, std::vector<VkExtensionProperties>* properties)
{
    uint32_t extension_count;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties extension_properties[extension_count];
    res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties);;
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

#ifdef VKA_GLFW_ENABLE
void vka::instance::get_glfw_extensions(std::vector<std::string>& glfw_extensions)
{
    if (!glfwVulkanSupported())
        throw std::runtime_error("[vka::instance::get_glfw_extensions]: Vulkan is not supported.");
    
    uint32_t n_extensions;
    const char** extensions = glfwGetRequiredInstanceExtensions(&n_extensions);
    for (uint32_t i = 0; i < n_extensions; i++)
        glfw_extensions.push_back(extensions[i]);
}
#endif
