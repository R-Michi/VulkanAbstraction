/**
* @file     vka_surface_impl.h
* @brief    Implemenation of surface functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

void vka::surface::formats(const VkPhysicalDevice device, const VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats)
{
    uint32_t n;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &n, nullptr);
    formats.resize(n);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &n, formats.data());
}

void vka::surface::presentation_modes(const VkPhysicalDevice device, const VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& modes)
{
    uint32_t n;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &n, nullptr);
    modes.resize(n);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &n, modes.data());
}

bool vka::surface::supports_format(const std::vector<VkSurfaceFormatKHR>& formats, VkSurfaceFormatKHR req_format)
{
    for (VkSurfaceFormatKHR format : formats)
    {
        if (format.format == req_format.format && format.colorSpace == req_format.colorSpace)
            return true;
    }
    return false;
}

size_t vka::surface::supports_formats(const std::vector<VkSurfaceFormatKHR>& formats, const std::vector<VkSurfaceFormatKHR>& req_formats)
{
    for (size_t i = 0; i < req_formats.size(); i++)
    {
        if (!supports_format(formats, req_formats[i]))
            return i;
    }
    return vka::NPOS;
}

size_t vka::surface::supports_any_format(const std::vector<VkSurfaceFormatKHR>& formats, const std::vector<VkSurfaceFormatKHR>& candidates)
{
    for (size_t i = 0; i < candidates.size(); i++)
    {
        if (supports_format(formats, candidates[i]))
            return i;
    }
    return vka::NPOS;
}

bool vka::surface::supports_presentmode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR req_mode)
{
    for (VkPresentModeKHR mode : modes)
    {
        if (mode == req_mode)
            return true;
    }
    return false;
}

size_t vka::surface::supports_presentmodes(const std::vector<VkPresentModeKHR>& modes, const std::vector<VkPresentModeKHR>& req_modes)
{
    for (size_t i = 0; i < req_modes.size(); i++)
    {
        if (!supports_presentmode(modes, req_modes[i]))
            return i;
    }
    return vka::NPOS;
}

size_t vka::surface::supports_any_presentmode(const std::vector<VkPresentModeKHR>& modes, const std::vector<VkPresentModeKHR>& candidates)
{
    for (size_t i = 0; i < candidates.size(); i++)
    {
        if (supports_presentmode(modes, candidates[i]))
            return i;
    }
    return vka::NPOS;
}
