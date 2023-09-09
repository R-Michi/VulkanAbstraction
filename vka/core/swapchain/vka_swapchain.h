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

namespace vka
{
    namespace swapchain
    {
        /*
        * Returns the image count for the swapchain.
        * If the requiered image count given by 'req_count' is 0, which it is by default, the
        * minimum image count for the swapchain is returned.
        * 
        * If the requiered image count given by 'req_count' is 0xFFFFFFFF (unsigend 32-bit integer limit),
        * the maximum image count for the swapchain is returned.
        * 
        * If 'req_count' is any other value, then the requiered image count is returned, 
        * if it is not greather than the maximum and not less than the
        * minimum capable image count. 
        *
        * If the given value is greather than the maximum or less than the minimum image count
        * capable for the surface, the maximum image count is returned, or the minimum respectively.
        * 
        * More formally, if 'req_count' > 0, 'max_count' is the maximum image count and
        * 'min_count' is the minium image count, then:
        * min(max(image_count, min_count), max_count) is returned.
        */
        uint32_t image_count(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t req_count = 0);


        /*
        * Returns the image extent for the swapchain.
        * If the reqiered image extent given by 'req_extent' has any component that is 0,
        * the minimum extent is returned.
        * 
        * If the requiered image extent given by 'req_extent' has any component that is 0xFFFFFFFF,
        * the maximum extent is returned.
        * 
        * If 'req_extent' is any other value, then the requiered image extent is returned,
        * if it is not greather than the maximum and not less than the minimum capable image
        * extent.
        * 
        * If the given value is greather than the maximum or less than the minimum image extent
        * capable for the surface, the maximum image extent is returned, or the minimum respectively.
        * This applies to every component of the VkExtent2D separately.
        * 
        * More formally if 'req_extent.width' > 0, 'req_extent.height' > 0, 'max_extent' is the
        * maximum image extent and 'min_extent' is the minimum image extent, then:
        * return_extent.width = min(max(req_extent.width, min_extent.width), max_extent.width)
        * return_extent.height = min(max(req_extent.height, min_extent.height), max_extent.height)
        * is returned.
        */
        VkExtent2D image_extent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D req_extent = { 0, 0 });

        /**
        * @brief                    Creates the swapchain and the corresponding image views.
        * @param[in]  device        logical device
        * @param[in]  create_info   swapchain create info
        * @param[out] swapchain     created swapchain
        * @param[out] image_views   created image views
        * @return                   vulkan result
        */
        VkResult setup(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<VkImageView>& image_views);
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_swapchain_impl.inl"
#endif
