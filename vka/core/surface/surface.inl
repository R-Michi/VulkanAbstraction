/**
 * @brief Implementation for the surface functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

std::vector<VkSurfaceFormatKHR> vka::surface::formats(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    std::vector<VkSurfaceFormatKHR> formats;
    uint32_t n;

    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &n, nullptr);
    formats.resize(n);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &n, formats.data());

    return formats;
}

std::vector<VkPresentModeKHR> vka::surface::presentation_modes(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    std::vector<VkPresentModeKHR> modes;
    uint32_t n;

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &n, nullptr);
    modes.resize(n);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &n, modes.data());

    return modes;
}

bool vka::surface::supports_format(const std::vector<VkSurfaceFormatKHR>& formats, VkSurfaceFormatKHR req_format) noexcept
{
    for (const VkSurfaceFormatKHR format : formats)
    {
        if (format.format == req_format.format && format.colorSpace == req_format.colorSpace)
            return true;
    }
    return false;
}

size_t vka::surface::supports_formats(const std::vector<VkSurfaceFormatKHR>& formats, const VkSurfaceFormatKHR* req_formats, size_t n) noexcept
{
    for (size_t i = 0; i < n; i++)
    {
        if (!supports_format(formats, req_formats[i]))
            return i;
    }
    return NPOS;
}

size_t vka::surface::supports_any_format(const std::vector<VkSurfaceFormatKHR>& formats, const VkSurfaceFormatKHR* candidates, size_t n) noexcept
{
    for (size_t i = 0; i < n; i++)
    {
        if (supports_format(formats, candidates[i]))
            return i;
    }
    return NPOS;
}

bool vka::surface::supports_presentmode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR req_mode) noexcept
{
    for (const VkPresentModeKHR mode : modes)
    {
        if (mode == req_mode)
            return true;
    }
    return false;
}

size_t vka::surface::supports_presentmodes(const std::vector<VkPresentModeKHR>& modes, const VkPresentModeKHR* req_modes, size_t n) noexcept
{
    for (size_t i = 0; i < n; i++)
    {
        if (!supports_presentmode(modes, req_modes[i]))
            return i;
    }
    return NPOS;
}

size_t vka::surface::supports_any_presentmode(const std::vector<VkPresentModeKHR>& modes, const VkPresentModeKHR* candidates, size_t n) noexcept
{
    for (size_t i = 0; i < n; i++)
    {
        if (supports_presentmode(modes, candidates[i]))
            return i;
    }
    return NPOS;
}
