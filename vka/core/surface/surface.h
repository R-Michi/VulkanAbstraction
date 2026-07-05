/**
 * @brief Helper functions for creating surfaces.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::surface
{
    /**
     * Queries the capabilities of a surface.
     * @param device Physical device.
     * @param surface Surface for which to query the capabilities.
     * @return Returns the capabilities of the specified surface.
     */
    VkSurfaceCapabilitiesKHR capabilities(VkPhysicalDevice device, VkSurfaceKHR surface) noexcept;

    /**
     * Queries all available formats of a surface.
     * @param device SPhysical device.
     * @param surface Surface for which to query the formats.
     * @return Returns all available formats of the specified surface.
     */
    std::vector<VkSurfaceFormatKHR> formats(VkPhysicalDevice device, VkSurfaceKHR surface);

    /**
     * Queries all available presentation modes of a surface.
     * @param device Physical device.
     * @param surface Surface for which to query the presentation modes.
     * @return Returns all available presentation modes of the specified surface.
     */
    std::vector<VkPresentModeKHR> presentation_modes(VkPhysicalDevice device, VkSurfaceKHR surface);

    /**
     * Checks if a surface supports a specified format.
     * @param formats All available formats of the surface.
     * @param req_format Format to check if it is supported by the surface.
     * @return Returns whether the specified format is supported.
     */
    bool supports_format(const std::vector<VkSurfaceFormatKHR>& formats, VkSurfaceFormatKHR req_format) noexcept;

    /**
     * Checks if a surface supports multiple formats.
     * @param formats All available formats of the surface.
     * @param req_formats Array of formats to check if they are supported by the surface.
     * @param n Number of formats to check.
     * @return Returns <c>vka::NPOS</c> if all formats are supported. If any format is not supported, the index of the
     * first not supported format is returned.
     */
    uint32_t supports_formats(const std::vector<VkSurfaceFormatKHR>& formats, const VkSurfaceFormatKHR* req_formats, uint32_t n) noexcept;

    /**
     * Checks if a surface supports any format.
     * @param formats All available formats of the surface.
     * @param candidates Array of formats to check if the surface supports any of them.
     * @param n Number of formats to check.
     * @return Returns the index of the first supported format. If no format is supported, <c>vka::NPOS</c> is returned.
     */
    uint32_t supports_any_format(const std::vector<VkSurfaceFormatKHR>& formats, const VkSurfaceFormatKHR* candidates, uint32_t n) noexcept;

    /**
     * Checks if a surface supports a specified presentation mode.
     * @param modes All available presentation modes of the surface.
     * @param req_mode Presentation mode to check if it is supported by the surface.
     * @return Returns whether the specified presentation mode is supported.
     */
    bool supports_presentmode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR req_mode) noexcept;

    /**
     * Checks if a surface supports multiple presentation modes.
     * @param modes All available presentation modes of the surface.
     * @param req_modes Array of presentation modes to check if they are supported by the surface.
     * @param n Number of presentation modes to check.
     * @return Returns <c>vka::NPOS</c> if all presentation modes are supported. If any presentation mode is not
     * supported, the index of the first not supported presentation mode is returned.
     */
    uint32_t supports_presentmodes(const std::vector<VkPresentModeKHR>& modes, const VkPresentModeKHR* req_modes, uint32_t n) noexcept;

    /**
     * Checks if a surface supports any presentation mode.
     * @param modes All available presentation modes of the surface.
     * @param candidates Array of presentation modes to check if the surface supports any of them.
     * @param n Number of presentation modes to check.
     * @return Returns the index of the first supported presentation mode. If no presentation mode is supported,
     * <c>vka::NPOS</c> is returned.
     */
    uint32_t supports_any_presentmode(const std::vector<VkPresentModeKHR>& modes, const VkPresentModeKHR* candidates, uint32_t n) noexcept;
}
