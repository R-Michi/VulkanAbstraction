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
     * @param image_views Returns all image views created from the swapchain. The size of the vector indicates the
     * actual number of images created by the swapchain. Note that the actual image count might be different from the
     * count specified in VkSwapchainCreateInfoKHR::minImageCount. The vector only contains successfully created image
     * views.
     * @return Returns VK_SUCCESS if and only if everything was created successfully.
     */
    VkResult create(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<VkImageView>& image_views);

    /**
     * Creates the swapchain, its associated image views, as well as framebuffers that reference those image views. If
     * creating any of the handles fails, nothing is returned by reference and the output parameters are not updated.
     * @param device Specifies the device to create the swapchain and its corresponding image views.
     * @param pass Specifies the render-pass from which the framebuffers are created.
     * @param create_info Specifies the create-info of the swapchain.
     * @param swapchain Returns the created swapchain handle.
     * @param fbos Returns all image views created from the swapchain and their associated framebuffers. The size of the
     * vector indicates the actual number of images created by the swapchain. Note that the actual image count might be
     * different from the count specified in VkSwapchainCreateInfoKHR::minImageCount. The vector only contains
     * successfully created image views.
     * @return Returns VK_SUCCESS if and only if everything was created successfully.
     */
    VkResult create(VkDevice device, VkRenderPass pass, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<SwapchainFramebuffer>& fbos);
}
