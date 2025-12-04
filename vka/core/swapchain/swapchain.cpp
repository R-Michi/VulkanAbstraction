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
        VkSwapchainKHR swapchain;
        std::unique_ptr<VkImage[]> images;
        uint32_t image_count;
        VkResult result;
    };

    struct CreateImageViewReturnInfo
    {
        std::unique_ptr<VkImageView[]> views;
        VkResult result;
    };

    struct CreateFboReturnInfo
    {
        std::unique_ptr<VkFramebuffer[]> fbos;
        VkResult result;
    };
}

static CreateSwapchainReturnInfo create_swapchain(VkDevice device, const VkSwapchainCreateInfoKHR& create_info)
{
    CreateSwapchainReturnInfo ri = {}; // zero the memory

    ri.result = vkCreateSwapchainKHR(device, &create_info, nullptr, &ri.swapchain);
    if (vka::is_error(ri.result)) [[unlikely]] return ri;

    // get image count
    ri.result = vkGetSwapchainImagesKHR(device, ri.swapchain, &ri.image_count, nullptr);
    if (vka::is_error(ri.result)) [[unlikely]] return ri;

    // get images
    ri.images = std::unique_ptr<VkImage[]>(new VkImage[ri.image_count]{ VK_NULL_HANDLE });
    ri.result = vkGetSwapchainImagesKHR(device, ri.swapchain, &ri.image_count, ri.images.get());
    return ri;
}

static CreateImageViewReturnInfo create_image_views(VkDevice device, VkFormat format, uint32_t count, const VkImage* images)
{
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
        .format = format,
        .components = component_mapping,
        .subresourceRange = subresource_range
    };

    CreateImageViewReturnInfo ri = {
        .views = std::unique_ptr<VkImageView[]>(new VkImageView[count]{ VK_NULL_HANDLE })
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

static CreateFboReturnInfo create_framebuffers(VkDevice device, VkRenderPass pass, VkExtent2D extent, uint32_t count, const VkImageView* views)
{
    // create framebuffers from the views
    VkFramebufferCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .renderPass = pass,
        .attachmentCount = 1,
        .pAttachments = nullptr, // set in loop
        .width = extent.width,
        .height = extent.height,
        .layers = 1
    };

    CreateFboReturnInfo ri = {
        .fbos = std::unique_ptr<VkFramebuffer[]>(new VkFramebuffer[count]{ VK_NULL_HANDLE })
    };

    // create fbos
    for (uint32_t i = 0; i < count; i++)
    {
        create_info.pAttachments = views + i;
        ri.result = vkCreateFramebuffer(device, &create_info, nullptr, ri.fbos.get() + i);
        if (vka::is_error(ri.result)) [[unlikely]] return ri;
    }
    return ri;
}

// The swapchain and image views must be guarded because of the specified guarantees of the function.
// It says: The output handles are only updated if and only if no error occurred while creating those handles.
// This implies that all handles created earlier must be destroyed to prevent leaking handles and to maintain
// correctness. This is achieved by a guard to a swapchain and a guard to an array of image-views which destroy the
// handles in their destructor unless released.
VkResult vka::swapchain::create(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<VkImageView>& image_views)
{
    const CreateSwapchainReturnInfo sri = create_swapchain(device, create_info);
    handle_guard swapchain_guard(device, &sri.swapchain);
    if (sri.result != VK_SUCCESS) [[unlikely]] return sri.result;

    const CreateImageViewReturnInfo vri = create_image_views(device, create_info.imageFormat, sri.image_count, sri.images.get());
    handle_guard view_guard(device, vri.views.get(), sri.image_count);
    if (vri.result != VK_SUCCESS) [[unlikely]] return vri.result;

    swapchain = sri.swapchain;
    image_views = std::vector(vri.views.get(), vri.views.get() + sri.image_count);

    swapchain_guard.release();
    view_guard.release();
    return VK_SUCCESS;
}

VkResult vka::swapchain::create(VkDevice device, VkRenderPass pass, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<SwapchainFramebuffer>& fbos)
{
    const CreateSwapchainReturnInfo sri = create_swapchain(device, create_info);
    handle_guard swapchain_guard(device, &sri.swapchain);
    if (sri.result != VK_SUCCESS) [[unlikely]] return sri.result;

    const CreateImageViewReturnInfo vri = create_image_views(device, create_info.imageFormat, sri.image_count, sri.images.get());
    handle_guard view_guard(device, vri.views.get(), sri.image_count);
    if (vri.result != VK_SUCCESS) [[unlikely]] return vri.result;

    const CreateFboReturnInfo fri = create_framebuffers(device, pass, create_info.imageExtent, sri.image_count, vri.views.get());
    handle_guard fbo_guard(device, fri.fbos.get(), sri.image_count);
    if (fri.result != VK_SUCCESS) [[unlikely]] return fri.result;

    std::vector<SwapchainFramebuffer> fbos_vec(sri.image_count);
    for (uint32_t i = 0; i < sri.image_count; i++)
        fbos_vec[i] = { vri.views[i], fri.fbos[i] };
    swapchain = sri.swapchain;
    fbos = std::move(fbos_vec);

    swapchain_guard.release();
    view_guard.release();
    fbo_guard.release();
    return VK_SUCCESS;
}
