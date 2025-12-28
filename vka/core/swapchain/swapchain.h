/**
 * @brief Helper function for creating swapchains.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::swapchain
{
    /**
     * Creates the swapchain and its associated image views. If creating any of the handles fails, nothing is returned
     * by reference and the output parameters are not updated.
     * @param device Specifies the device to create the swapchain and its corresponding image views.
     * @param create_info Specifies the create-info of the swapchain.
     * @param swapchain Returns the created swapchain handle.
     * @param image_views Returns all image views created from the swapchain. Note that the actual image count might be
     * different from the count specified in VkSwapchainCreateInfoKHR::minImageCount.
     * @return Returns VK_SUCCESS if and only if everything was created successfully.
     */
    VkResult create(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, unique_handle<VkImageView[]>& image_views);
}
