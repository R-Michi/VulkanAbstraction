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

    struct CreateFboReturnInfo
    {
        vka::unique_handle<vka::SwapchainFramebuffer[]> fbos;
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

static CreateFboReturnInfo create_framebuffers(VkDevice device, VkRenderPass pass, VkFormat format, VkExtent2D extent, uint32_t count, const VkImage* images)
{
    vka::SwapchainFramebufferCreateInfo create_info = {
        .swapchain_image = VK_NULL_HANDLE, // set in loop
        .view_format = format,
        .extent = extent,
        .components = component_mapping(),
        .subresource_range = subresource_range(),
        .render_pass = pass
    };

    CreateFboReturnInfo ri = {
        .fbos = vka::unique_handle<vka::SwapchainFramebuffer[]>(device, new vka::SwapchainFramebuffer[count]{}, count)
    };

    // create fbos
    for (uint32_t i = 0; i < count; i++)
    {
        create_info.swapchain_image = images[i];
        ri.result = vka::swapchain::create_framebuffer(device, &create_info, nullptr, ri.fbos.get() + i);
        if (vka::is_error(ri.result)) [[unlikely]] return ri;
    }
    return ri;
}

// The swapchain and image views must be guarded because of the specified guarantees of the function.
// It says: The output handles are only updated if and only if no error occurred while creating those handles.
// This implies that all handles created earlier must be destroyed to prevent leaking handles and to maintain
// correctness. This is achieved by a guard to a swapchain and a guard to an array of image-views which destroy the
// handles in their destructor unless released.
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

VkResult vka::swapchain::create(VkDevice device, VkRenderPass pass, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, unique_handle<SwapchainFramebuffer[]>& fbos)
{
    CreateSwapchainReturnInfo sri = create_swapchain(device, create_info);
    if (sri.result != VK_SUCCESS) [[unlikely]] return sri.result;

    CreateFboReturnInfo fri = create_framebuffers(device, pass, create_info.imageFormat, create_info.imageExtent, sri.image_count, sri.images.get());
    if (fri.result != VK_SUCCESS) [[unlikely]] return fri.result;

    swapchain = sri.swapchain.release();
    fbos = std::move(fri.fbos);
    return VK_SUCCESS;
}
