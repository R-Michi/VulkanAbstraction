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

namespace vka
{
    namespace surface
    {
        /*
        * Returns the surface formats of a given physical device and surface.
        * The physical device is specified by 'device' and the surface is specified by 'surface'.
        * The returned surface formats are stored in 'formats'.
        */
        void formats(const VkPhysicalDevice device, const VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR>& formats);

        /*
        * Returns the surface's presentation modes of a given physical device.
        * The physical device is specified by 'device' and the surface is specified by 'surface'.
        * The returned presentation modes are stored in 'modes'.
        */
        void presentation_modes(const VkPhysicalDevice device, const VkSurfaceKHR surface, std::vector<VkPresentModeKHR>& modes);

        /*
        * Checks, if a surface supports a given format. More formally checks, if 'req_format' is
        * contained within 'formats'.
        * The format to search for is specified by 'req_format' and 'formats' specifies all
        * supported formats from a surface.
        */
        bool supports_format(const std::vector<VkSurfaceFormatKHR>& formats, VkSurfaceFormatKHR req_format);

        /*
        * Checks, if a surface supports multiple formats. More formally checks, if all elements of 'req_formats'
        * are contained within 'formats'.
        * The formats to search for are specified by 'req_formats' and 'formats' specifies all
        * supported formats from a surface.
        * If all formats are supported, then vka::NPOS is returned.
        * If any format is not supported, then the index within the 'formats'-vector of
        * the first not supported format is returned.
        */
        size_t supports_formats(const std::vector<VkSurfaceFormatKHR>& formats, const std::vector<VkSurfaceFormatKHR>& req_formats);

        /*
        * Checks, if a surface supports any format. More formally checks, if any element of 'candidates'
        * is contained within 'formats'.
        * The format chadidates are specified by 'candidates' and 'formats' specifies all
        * supported formats from a surface.
        * If any format is supported, then the index within the 'formats'-vector of 
        * the first supported format is returned.
        * If no format is supported, then vka::NPOS is returned.
        */
        size_t supports_any_format(const std::vector<VkSurfaceFormatKHR>& formats, const std::vector<VkSurfaceFormatKHR>& candidates);

        /*
        * Checks, if a surface supports a given presentation mode. More formally checks, if 'req_mode'
        * is contained within 'modes'.
        * The presentation mode to search for is specified by 'req_mode' and 'modes' specifies all
        * supported presentation modes from a surface.
        */
        bool supports_presentmode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR req_mode);

        /*
        * Checks, if a surface supports multiple presentation modes. More formally checks, if all elements of 'req_modes'
        * are contained within 'modes'.
        * The presentation modes to search for are specified by 'req_modes' and 'modes' specifies all
        * supported presentation modes from a surface.
        * If all presentation modes are supported, then vka::NPOS is returned.
        * If any presentation mode is not supported, then the index within the 'modes'-vector of
        * the first not supported presentation mode is returned.
        */
        size_t supports_presentmodes(const std::vector<VkPresentModeKHR>& modes, const std::vector<VkPresentModeKHR>& req_modes);

        /*
        * Checks, if a surface supports any presentation mode. More formally checks, if any element of 'candidates'
        * is contained within 'modes'.
        * The presentation mode candidates are specified by 'candidates' and 'modes' specifies all
        * supported presentation modes from a surface.
        * If any presentation mode is supported, then the index within the 'modes'-vector of 
        * the first supported presentation mode is returned.
        * If no presentation mode is supported, then vka::NPOS is returned.
        */
        size_t supports_any_presentmode(const std::vector<VkPresentModeKHR>& modes, const std::vector<VkPresentModeKHR>& candidates);
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_surface_impl.inl"
#endif