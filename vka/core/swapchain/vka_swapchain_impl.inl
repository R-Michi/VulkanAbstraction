/**
* @file     vka_swapchain_impl.h
* @brief    Implemenation of swapchain functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

VkResult vka::swapchain::setup(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<VkImageView>& image_views)
{
    VkResult result = vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain);
    if (result != VK_SUCCESS)
        return result;
    
    image_views.clear();
    
    uint32_t n_images;
    vkGetSwapchainImagesKHR(device, swapchain, &n_images, nullptr);
    VkImage images[n_images];
    vkGetSwapchainImagesKHR(device, swapchain, &n_images, images);
    
    for (uint32_t i = 0; i < n_images; i++)
    {
        VkImageViewCreateInfo image_view_create_info = {};
        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.pNext = nullptr;
        image_view_create_info.flags = 0;
        image_view_create_info.image = images[i];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = create_info.imageFormat;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;			// final image should not have mipmapping
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;
        
        VkImageView image_view;
        VkResult result = vkCreateImageView(device, &image_view_create_info, nullptr, &image_view);
        if (result != VK_SUCCESS)
            return result;
        
        image_views.push_back(image_view);
    }
    return VK_SUCCESS;
}
