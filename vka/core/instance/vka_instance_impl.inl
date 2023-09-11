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

bool vka::instance::supports_layer(std::string_view layer_name, VkLayerProperties* properties) noexcept
{
    uint32_t layer_count;
    VkResult res = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (res != VK_SUCCESS && res != VK_INCOMPLETE) return false;

    VkLayerProperties layer_properties[layer_count];
    res = vkEnumerateInstanceLayerProperties(&layer_count, layer_properties);
    if (res != VK_SUCCESS && res != VK_INCOMPLETE) return false;

    size_t idx = detail::instance::supports_layer(layer_properties, layer_count, layer_name);
    bool b = (idx != vka::NPOS);
    if (b && properties != nullptr)
        *properties = layer_properties[idx];
    return b;
}

size_t vka::instance::supports_layers(const std::vector<std::string>& layer_names, VkLayerProperties* properties) noexcept
{
    uint32_t layer_count;
    VkResult res = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties layer_properties[layer_count];
    res = vkEnumerateInstanceLayerProperties(&layer_count, layer_properties);
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < layer_names.size(); i++)
    {
        size_t j;
        if ((j = detail::instance::supports_layer(layer_properties, layer_count, layer_names[i])) == vka::NPOS)   // layer 'layer_names[i]' is not supported
            return i;
        if (properties != nullptr)
            properties[i] = layer_properties[j];
    }
    return vka::NPOS;
}

bool vka::instance::supports_extension(std::string_view extension_name, VkExtensionProperties* properties) noexcept
{
    uint32_t extension_count;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties extension_properties[extension_count];
    res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties);
    if (res != VK_SUCCESS) return false;

    size_t idx = detail::instance::supports_extension(extension_properties, extension_count, extension_name);
    bool b = (idx != vka::NPOS);
    if (b && properties != nullptr)
        *properties = extension_properties[idx];
    return b;
}

size_t vka::instance::supports_extensions(const std::vector<std::string>& extension_names, VkExtensionProperties* properties) noexcept
{
    uint32_t extension_count;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties extension_properties[extension_count];
    res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties);;
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < extension_names.size(); i++)
    {
        size_t j;
        if ((j = detail::instance::supports_extension(extension_properties, extension_count, extension_names[i])) == vka::NPOS) // extension 'extension_names[i]' is not supported
            return i;
        if (properties != nullptr)
            properties[i] = extension_properties[j];
    }
    return vka::NPOS;
}

#ifdef VKA_GLFW_ENABLE
void vka::instance::get_glfw_extensions(std::vector<std::string>& extensions)
{
    uint32_t count;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&count);
    for (uint32_t i = 0; i < count; i++)
        extensions.push_back(glfw_extensions[i]);
}

void vka::instance::get_glfw_extensions(const char** extensions) noexcept
{
    uint32_t count;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&count);
    for (uint32_t i = 0; i < count; i++)
        extensions[i] = glfw_extensions[i];
}
#endif
