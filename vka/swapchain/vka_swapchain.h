/**
* @file     vka_swapchain.h
* @brief    Useful swapchain related functions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../vka_global.h"

namespace vka
{
    namespace swapchain
    {
        /**
        * @brief Creates the swapchain and the corresponding image views.
        * @param[in] device: logical device
        * @param[in] create_info: swapchain create info
        * @param[out] swapchain: created swapchain
        * @param[out] image_views: created image views
        * @return vulkan result
        */
        VkResult setup(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<VkImageView>& image_views);
    }
}