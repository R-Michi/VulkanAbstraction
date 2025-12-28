/**
 * @brief Implementation for the swapchain functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <memory>
#include <vulkan/vulkan.h>
#include <vka/vka.h>

namespace
{
    struct CreateSwapchainReturnInfo
    {
        vka::unique_handle<VkSwapchainKHR> swapchain;
        std::unique_ptr<VkImage[]> images;
        uint32_t image_count;
        VkResult result;
    };

    struct CreateImageViewReturnInfo
    {
        vka::unique_handle<VkImageView[]> views;
        VkResult result;
    };
}

static consteval VkComponentMapping component_mapping() noexcept
{
    return {
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY,
        VK_COMPONENT_SWIZZLE_IDENTITY
    };
}

static consteval VkImageSubresourceRange subresource_range() noexcept
{
    return {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1
    };
}

static CreateSwapchainReturnInfo create_swapchain(VkDevice device, const VkSwapchainCreateInfoKHR& create_info)
{
    CreateSwapchainReturnInfo ri = {}; // zero the memory

    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    ri.result = vkCreateSwapchainKHR(device, &create_info, nullptr, &swapchain);
    ri.swapchain = vka::unique_handle(device, swapchain);
    if (vka::is_error(ri.result)) [[unlikely]] return ri;

    // get image count
    ri.result = vkGetSwapchainImagesKHR(device, ri.swapchain.get(), &ri.image_count, nullptr);
    if (vka::is_error(ri.result)) [[unlikely]] return ri;

    // get images
    ri.images = std::unique_ptr<VkImage[]>(new VkImage[ri.image_count]{ VK_NULL_HANDLE });
    ri.result = vkGetSwapchainImagesKHR(device, ri.swapchain.get(), &ri.image_count, ri.images.get());
    return ri;
}

static CreateImageViewReturnInfo create_image_views(VkDevice device, VkFormat format, uint32_t count, const VkImage* images)
{
    VkImageViewCreateInfo view_create_info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = VK_NULL_HANDLE, // set in loop
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
        .components = component_mapping(),
        .subresourceRange = subresource_range()
    };

    CreateImageViewReturnInfo ri = {
        .views = vka::unique_handle<VkImageView[]>(device, new VkImageView[count]{ VK_NULL_HANDLE }, count)
    };

    // create views
    for (uint32_t i = 0; i < count; i++)
    {
        view_create_info.image = images[i];
        ri.result = vkCreateImageView(device, &view_create_info, nullptr, ri.views.get() + i);
        if (vka::is_error(ri.result)) [[unlikely]] return ri;
    }
    return ri;
}

VkResult vka::swapchain::create(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, unique_handle<VkImageView[]>& image_views)
{
    CreateSwapchainReturnInfo sri = create_swapchain(device, create_info);
    if (sri.result != VK_SUCCESS) [[unlikely]] return sri.result;

    CreateImageViewReturnInfo vri = create_image_views(device, create_info.imageFormat, sri.image_count, sri.images.get());
    if (vri.result != VK_SUCCESS) [[unlikely]] return vri.result;

    swapchain = sri.swapchain.release();
    image_views = std::move(vri.views);
    return VK_SUCCESS;
}
