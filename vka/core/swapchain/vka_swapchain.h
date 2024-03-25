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
    /**
     * @brief Clamps a given image count to the minimum and maximum capable image count.
     * @param capabilities Specifies the capabilities of a surface.
     * @param req_count Specifies the required image count.
     * @return Returns the clamped image count for the swapchain.
     * @details If the required image count is no special value, it gets limited to the minimum and maximum capable
     * image count. More formally: Lets assume 'c' is the required, 'a' the minimum and 'b' the maximum image count.
     * This function then computes the resulting image count 'r' as follows: r = min(max(c, a), b).\n
     * Furthermore, the required image count can have two special values:\n
     *  - If the value is 0, the minimum capable image count is returned.\n
     *  - If the value is 0xFFFFFFFF (maximum value of uint32_t), the maximum capable image count is returned.
     */
    uint32_t image_count(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t req_count = 0) noexcept;


    /**
     * @brief Clamps a given image extent to the minimum and maximum capable image extent.
     * @param capabilities Specifies the capabilities of a surface.
     * @param req_extent Specifies the required image extent.
     * @return Returns the clamped image extent for the swapchain.
     * @details If the required image extent is no special value, it gets limited to the minimum and maximum capable
     * image extent. More formally: Lets assume 'e' is the required, 'a' the minimum and 'b' the maximum image extent.
     * This function then computes the resulting image extent 'r' as follows: r = min(max(e, a), b).\n
     * Furthermore, the required image extent can have two special values:\n
     *  - If any of the required extent's values is 0, the minimum extent is returned.
     *  - If any of the required extent's values is 0xFFFFFFFF (maximum value of uint32_t), the maximum capable image
     *  extent is returned.
     */
    VkExtent2D image_extent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D req_extent = { 0, 0 }) noexcept;

    /**
     * @brief Creates the swapchain and the corresponding image views.
     * @param device Specifies a valid device.
     * @param create_info Specifies a swapchain create info.
     * @param swapchain Returns the created swapchain.
     * @param image_views Specifies an array in which to store all created image views. The length of the array must
     * correspond to the number of images specified in the swapchain create info. More formally the array length must
     * be >='VkSwapchainCreateInfoKHR::minImageCount'.
     * @throw std::runtime_error Is thrown, if creating the swapchain or the image views failed.
     * @note It is recommended to initialize the input swapchain handle and the image view handles with VK_NULL_HANDLE
     * to be able to delete the right handles again in case of an error.
     */
    void setup(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, VkImageView* image_views);
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_swapchain_impl.inl"
#endif
