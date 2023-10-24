/**
* @file     vka_swapchain.h
* @brief    Useful swapchain functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka::swapchain
{
    /*
    * Returns the image count for the swapchain. If the required image count given by
    * 'req_count' is 0, which it is by default, the minimum image count for the swapchain is
    * returned.
    *
    * If the required image count given by 'req_count' is 0xFFFFFFFF (unsigned 32-bit integer
    * limit), the maximum image count for the swapchain is returned.
    *
    * If 'req_count' is any other value, then the required image count is returned, if it is
    * not greater than the maximum and not less than the minimum capable image count.
    *
    * If the given value is greater than the maximum or less than the minimum image count
    * capable for the surface, the maximum image count is returned or the minimum respectively.
    * More formally, if 'req_count' > 0, 'max_count' is the maximum image count and 'min_count'
    * is the minimum image count, then: min(max(image_count, min_count), max_count) is returned.
    */
    uint32_t image_count(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t req_count = 0) noexcept;


    /*
    * Returns the image extent for the swapchain. If the required image extent given by
    * 'req_extent' has any component that is 0, the minimum extent is returned.
    *
    * If the required image extent given by 'req_extent' has any component that is 0xFFFFFFFF,
    * the maximum extent is returned.
    *
    * If 'req_extent' is any other value, then the required image extent is returned,
    * if it is not greater than the maximum and not less than the minimum capable image extent.
    *
    * If the given value is greater than the maximum or less than the minimum image extent
    * capable for the surface, the maximum image extent is returned, or the minimum
    * respectively. This applies to every component of the VkExtent2D separately.
    * More formally,  if 'req_extent.width' > 0, 'req_extent.height' > 0, 'max_extent' is the
    * maximum image extent and 'min_extent' is the minimum image extent, then:
    * return_extent.width = min(max(req_extent.width, min_extent.width), max_extent.width)
    * return_extent.height = min(max(req_extent.height, min_extent.height), max_extent.height)
    * is returned.
    */
    VkExtent2D image_extent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D req_extent = { 0, 0 }) noexcept;

    /*
    * Creates the swapchain and the corresponding image views. A device is required for this
    * operation and is specified by 'device'. The create-info for the swapchain is specified by
    * 'create_info' and the swapchain is stored in 'swapchain'. The image views of the
    * swapchain are stored in the 'image_views' array. The array must have a length of at least
    * the amount of specified images in the swapchain. If an error occured while setting up, a
    * std::runtime_error exception is thrown with an appropriate message about the error.
    * NOTE: If an error occured it was either caused by creating the swapchain or one of the
    * image views. Therefore, it is recommended to initialize the input swapchain and the input
    * image views with VK_NULL_HANDLE in order to delete the handles that have been created in
    * the case of an error.
    */
    void setup(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, VkImageView* image_views) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_swapchain_impl.inl"
#endif
