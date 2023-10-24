/**
* @file     vka_instance_impl.inl
* @brief    Detailed implementation of inline instance functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline size_t vka::detail::instance::supports_layer(const VkLayerProperties* properties, uint32_t count, std::string_view layer_name) noexcept
{
    for (size_t i = 0; i < (size_t)count; i++)  // count must be uint32_t requiered by vulkan
    {
        if (layer_name == properties[i].layerName)
            return i;
    }
    return vka::NPOS;
}

inline size_t vka::detail::instance::supports_extension(const VkExtensionProperties* properties, uint32_t count, std::string_view extension_name) noexcept
{
    for (size_t i = 0; i < (size_t)count; i++)  // count must be uint32_t requiered by vulkan
    {
        if (extension_name == properties[i].extensionName)
            return i;
    }
    return vka::NPOS;
}