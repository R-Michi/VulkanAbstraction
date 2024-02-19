/**
* @file     vka_surface.h
* @brief    Useful surface functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka::surface
{
    /**
     * @brief Returns all available surface formats of a physical device.
     * @param device Specifies a valid physical device.
     * @param surface Specifies a valid surface from which to query the formats.
     * @param formats Specifies a vector in which to store all supported surface formats.
     */
    void formats(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats);

    /**
     * @brief Returns all available surface presentation modes of a physical device.
     * @param device Specifies a valid physical device.
     * @param surface Specifies a valid surface from which to query the presentation modes.
     * @param modes Specifies a vector in which to store all supported presentation modes.
     */
    void presentation_modes(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& modes);

    /**
     * @brief Checks, if a surface supports a given format.
     * @param formats Specifies all available formats of a surface.
     * @param req_format Specifies the surface to check for.
     * @return Returns 'true', if the format is supported and false otherwise.
     */
    bool supports_format(const std::vector<VkSurfaceFormatKHR>& formats, VkSurfaceFormatKHR req_format) noexcept;

    /**
     * @brief Checks, if a surface supports multiple formats.
     * @param formats Specifies all available formats of a surface.
     * @param req_formats Specifies the formats to check, if all of them are supported.
     * @param n Specifies the number of formats to check.
     * @return Returns vka::NPOS, if all formats are supported. If any format is not supported, then the index of the
     * first not supported format is returned.
     */
    size_t supports_formats(const std::vector<VkSurfaceFormatKHR>& formats, VkSurfaceFormatKHR* req_formats, size_t n) noexcept;

    /**
     * @brief Checks, if a surface supports any format.
     * @param formats Specifies all available formats of a surface.
     * @param candidates Specifies the formats to check, if any of them is supported.
     * @param n Specifies the number of formats to check.
     * @return Returns the index of the first supported format. If no format is supported, vka::NPOS is returned.
     */
    size_t supports_any_format(const std::vector<VkSurfaceFormatKHR>& formats, VkSurfaceFormatKHR* candidates, size_t n) noexcept;

    /**
     * @brief Checks, if a surface supports a given presentation mode.
     * @param modes Specifies all available presentation modes of a surface.
     * @param req_mode Specifies the presentation mode to check for.
     * @return Returns 'true', if the presentation mode is supported and 'false' otherwise.
     */
    bool supports_presentmode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR req_mode) noexcept;

    /**
     * @brief Checks, if a surface supports multiple presentation modes.
     * @param modes Specifies all available presentation modes of a surface.
     * @param req_modes Specifies the presentation modes to check, if all of them are supported.
     * @param n Specifies the number of presentation modes to check.
     * @return Returns vka::NPOS, if all presentation modes are supported. If any presentation mode is not supported,
     * then the index of the first not supported presentation mode is returned.
     */
    size_t supports_presentmodes(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR* req_modes, size_t n) noexcept;

    /**
     * @brief Checks, if a surface supports any presentation mode.
     * @param modes Specifies all available presentation modes of a surface.
     * @param candidates Specifies the presentation modes to check, if any of them is supported.
     * @param n Specifies the number of presentation modes to check.
     * @return Returns the index of the first supported presentation mode. If no presentation mode is supported,
     * vka::NPOS is returned.
     */
    size_t supports_any_presentmode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR* candidates, size_t n) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_surface_impl.inl"
#endif