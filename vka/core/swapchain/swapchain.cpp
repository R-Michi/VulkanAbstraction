/**
 * @brief Implementation for the swapchain functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include "../../vka.h"

uint32_t vka::swapchain::image_count(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t req_count) noexcept
{
    if (req_count == 0)
        return capabilities.minImageCount;
    if (req_count == 0xFFFFFFFF)
        return capabilities.maxImageCount;
    return std::min(std::max(req_count, capabilities.minImageCount), capabilities.maxImageCount);
}

VkExtent2D vka::swapchain::image_extent(const VkSurfaceCapabilitiesKHR& capabilities, VkExtent2D req_extent) noexcept
{
    if (req_extent.width == 0 || req_extent.height == 0)
        return capabilities.minImageExtent;
    if (req_extent.width == 0xFFFFFFFF || req_extent.height == 0xFFFFFFFF)
        return capabilities.maxImageExtent;
    return {
        std::min(std::max(req_extent.width, capabilities.minImageExtent.width), capabilities.maxImageExtent.width),
        std::min(std::max(req_extent.height, capabilities.minImageExtent.height), capabilities.maxImageExtent.height)
    };
}

VkSwapchainKHR vka::swapchain::setup(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkImageView* image_views)
{
    constexpr char SWAPCHAIN_CREATE_FAILED[] = "[vka::swapchain::setup]: Failed to create swapchain.";
    constexpr char IMAGE_VIEW_CREATE_FAILED[] = "[vka::swapchain::setup]: Failed to create image view";

    VkSwapchainKHR swapchain;
    check_result(vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain), SWAPCHAIN_CREATE_FAILED);

    uint32_t image_count;
    vkGetSwapchainImagesKHR(device, swapchain, &image_count, nullptr);
    VkImage* const images = (VkImage*)alloca(image_count * sizeof(VkImage));
    vkGetSwapchainImagesKHR(device, swapchain, &image_count, images);

    VkImageViewCreateInfo iv_ci = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = VK_NULL_HANDLE,        // set in loop
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = VK_FORMAT_MAX_ENUM,   // set in loop
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,            // the final image should not have mipmapping
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };

    for (uint32_t i = 0; i < image_count; i++)
    {
        iv_ci.image = images[i];
        iv_ci.format = create_info.imageFormat;
        check_result(vkCreateImageView(device, &iv_ci, nullptr, image_views + i), IMAGE_VIEW_CREATE_FAILED);
    }
    return swapchain;
}
