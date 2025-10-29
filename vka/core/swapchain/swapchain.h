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
     * @brief Creates the swapchain and its associated image views.
     * @param device Specifies the device to create the swapchain and its corresponding image views.
     * @param create_info Specifies the create-info of the swapchain.
     * @param swapchain Returns the created swapchain handle. If the creation of the >> swapchain << failed, the handle
     * is not altered.
     * @param image_views Returns all image views created from the swapchain. The size of the vector indicates the
     * actual number of images created by the swapchain. Note that the actual image count might be different from the
     * count specified in VkSwapchainCreateInfoKHR::minImageCount. The vector only contains successfully created image
     * views.
     * @return Returns VK_SUCCESS iff everything was created successfully.
     * @note This function does not clear the vector, and image views are only appended to the vector. Keep in mind that
     * this function may still fail after the swapchain handle has already been created!
     * @details A swapchain can be created for the first time, or it can be created from a so-called "old" swapchain
     * handle. The first case is straightforward and proceeds in the same way as creating any other vulkan handle. For
     * the second case follow these steps in order:
     * 1. Initialize the new create-info with the current swapchain handle.
     * 2. Destroy all current image views.
     * 3. Invalidate the image view handles (clearing the vector that contains them).
     * 4. Create a new temporary VkSwapchainKHR handle and initialize it with VK_NULL_HANDLE.
     * 5. Call this function.
     * 6. Destroy the current swapchain handle.
     * 7. Update the current swapchain with the new obtained swapchain handle.
     * 8. Check the result.
     * @details Some of those steps could be exchanged. However, an exception must not be thrown between steps 2 and 3,
     * and between steps 5 and 7 to avoid leaks or accessing dangling handles.
     */
    VkResult create(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<VkImageView>& image_views);
}
