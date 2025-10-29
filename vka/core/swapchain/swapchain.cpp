/**
 * @brief Implementation for the swapchain functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include <vka/vka.h>

VkResult vka::swapchain::create(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<VkImageView>& image_views)
{
    VkResult result = vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain);
    if (is_error(result)) return result;

    // get images from swapchain (only temporary)
    uint32_t image_count;
    result = vkGetSwapchainImagesKHR(device, swapchain, &image_count, nullptr);

    VkImage* images = (VkImage*)alloca(image_count * sizeof(VkImage));
    if (is_error(result)) return result;
    result = vkGetSwapchainImagesKHR(device, swapchain, &image_count, images);
    if (is_error(result)) return result;

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
    for (uint32_t i = 0; i < image_count; i++)
    {
        VkImageView view;
        view_create_info.image = images[i];
        result = vkCreateImageView(device, &view_create_info, nullptr, &view);
        if (is_error(result)) return result;
        image_views.push_back(view);
    }
    return VK_SUCCESS;
}
