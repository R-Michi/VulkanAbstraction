#pragma once

#include "swapchain.h"

inline VkResult vka::swapchain::create_framebuffer(
    VkDevice device,
    const SwapchainFramebufferCreateInfo* create_info,
    const VkAllocationCallbacks* allocator,
    SwapchainFramebuffer* fbo
) noexcept
{
    const VkImageViewCreateInfo view_ci = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = create_info->swapchain_image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = create_info->view_format,
        .components = create_info->components,
        .subresourceRange = create_info->subresource_range
    };

    VkImageView view;
    VkResult result = vkCreateImageView(device, &view_ci, allocator, &view);
    if (static_cast<int32_t>(result) < 0) [[unlikely]]
        return result;

    const VkFramebufferCreateInfo fbo_ci = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .renderPass = create_info->render_pass,
        .attachmentCount = 1,
        .pAttachments = &view,
        .width = create_info->extent.width,
        .height = create_info->extent.height,
        .layers = 1
    };

    VkFramebuffer framebuffer;
    result = vkCreateFramebuffer(device, &fbo_ci, allocator, &framebuffer);
    if (static_cast<int32_t>(result) < 0) [[unlikely]]
    {
        vkDestroyImageView(device, view, allocator);
        return result;
    }

    fbo->view = view;
    fbo->framebuffer = framebuffer;
    return VK_SUCCESS;
}

inline void vka::swapchain::destroy_framebuffer(VkDevice device, SwapchainFramebuffer fbo, const VkAllocationCallbacks* allocator) noexcept
{
    vkDestroyFramebuffer(device, fbo.framebuffer, allocator);
    vkDestroyImageView(device, fbo.view, allocator);
}