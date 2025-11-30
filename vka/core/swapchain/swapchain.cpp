/**
 * @brief Implementation for the swapchain functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include <vka/vka.h>

// The swapchain and image views must be guarded because of the specified guarantees of the function.
// It says: The output handles are only updated if and only if no error occurred while creating those handles.
// This implies that all handles created earlier must be destroyed to prevent leaking handles and to maintain
// correctness. This is achieved by a guard to a swapchain and a guard to an array of image-views which destroy the
// handles in their destructor unless released.
VkResult vka::swapchain::create(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<VkImageView>& image_views)
{
    VkSwapchainKHR tmp_swapchain = VK_NULL_HANDLE;
    detail::SwapchainGuard swapchain_guard(device, &tmp_swapchain);
    VkResult result = vkCreateSwapchainKHR(device, &create_info, nullptr, &tmp_swapchain);
    if (is_error(result)) [[unlikely]] return result;

    // get images from swapchain (only temporary)
    uint32_t image_count;
    result = vkGetSwapchainImagesKHR(device, tmp_swapchain, &image_count, nullptr);
    if (is_error(result)) [[unlikely]] return result;

    VkImage* images = (VkImage*)alloca(image_count * sizeof(VkImage));
    result = vkGetSwapchainImagesKHR(device, tmp_swapchain, &image_count, images);
    if (is_error(result)) [[unlikely]] return result;

    // create image views from the images
    constexpr VkComponentMapping component_mapping = {
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY
    };
    constexpr VkImageSubresourceRange subresource_range = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
    };
    VkImageViewCreateInfo view_create_info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = VK_NULL_HANDLE, // set in loop
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = create_info.imageFormat,
        .components = component_mapping,
        .subresourceRange = subresource_range
    };

    // The image views must be initialized to guarantee correctness with the guard.
    VkImageView* views = (VkImageView*)alloca(image_count * sizeof(VkImageView));
    for (uint32_t i = 0; i < image_count; i++)
        views[i] = VK_NULL_HANDLE;

    detail::ImageViewGuard view_guard(device, views, image_count);
    for (uint32_t i = 0; i < image_count; i++)
    {
        view_create_info.image = images[i];
        result = vkCreateImageView(device, &view_create_info, nullptr, views + i);
        if (is_error(result)) [[unlikely]] return result;
    }

    swapchain_guard.release();
    view_guard.release();

    swapchain = tmp_swapchain;
    image_views = std::vector(views, views + image_count);
    return VK_SUCCESS;
}
