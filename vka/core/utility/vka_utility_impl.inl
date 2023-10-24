/**
* @file     vka_utility_impl.h
* @brief    Implemenation of utility functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

bool vka::utility::supports_format_feature(const VkFormatProperties& properties, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept
{   
    // check if the format properties of the given format supports the given format features
    if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & format_feature) == format_feature) return true;
    if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & format_feature) == format_feature) return true;
    return false;
}

bool vka::utility::supports_format_feature2(VkPhysicalDevice device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept
{
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(device, format, &properties);
    return supports_format_feature(properties, tiling, format_feature);
}

void vka::utility::get_supported_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats)
{
    formats.clear();
    for (uint32_t i = 0; i <= 184; i++)
    {
        if (supports_format_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000156000; i <= 1000156033; i++)
    {
        if (supports_format_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000330000; i <= 1000330003; i++)
    {
        if (supports_format_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000340000; i <= 1000340001; i++)
    {
        if (supports_format_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000054000; i <= 1000054007; i++)
    {
        if (supports_format_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000066000; i <= 1000066013; i++)
    {
        if (supports_format_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    // format 1000464000
    if (supports_format_feature2(physical_device, VK_FORMAT_R16G16_S10_5_NV, tiling, format_feature))
        formats.push_back(VK_FORMAT_R16G16_S10_5_NV);
}

void vka::utility::cvt_stdstr2ccpv(const std::vector<std::string>& std_in, const char** ccp_out) noexcept
{
    for (size_t i = 0; i < std_in.size(); i++)
        ccp_out[i] = std_in[i].c_str();
}

VkCommandBuffer vka::utility::begin_cbo(VkDevice device, VkCommandPool pool) noexcept
{
    constexpr const char CBO_ALLOC_FAILED[] = "[vka::utility::begin_cbo]: Failed to allocate command buffer.";
    constexpr const char CBO_BEGIN_FAILED[] = "[vka::utility::begin_cbo]: Failed to begin command buffer recording";

    const VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    VkCommandBuffer cbo;
    detail::error::check_result(vkAllocateCommandBuffers(device, &alloc_info, &cbo), CBO_ALLOC_FAILED);

    constexpr VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = nullptr
    };
    if (detail::error::is_error(vkBeginCommandBuffer(cbo, &begin_info))) [[unlikely]]
    {
        vkFreeCommandBuffers(device, pool, 1, &cbo);
        detail::error::throw_runtime_error(CBO_BEGIN_FAILED);
    }
    return cbo;
}

void vka::utility::end_cbo(VkQueue queue, VkCommandBuffer cbo, VkFence fence) noexcept
{
    detail::utility::end_cbo_and_submit(queue, cbo, fence);
}

VkResult vka::utility::end_wait_cbo(VkQueue queue, VkCommandBuffer cbo, VkDevice device, VkFence fence, uint64_t timeout) noexcept
{
    const VkResult res = detail::utility::end_cbo_and_submit(queue, cbo, fence);
    if (res != VK_SUCCESS) return res;
    return (fence == VK_NULL_HANDLE) ? vkQueueWaitIdle(queue) : vkWaitForFences(device, 1, &fence, VK_TRUE, timeout);
}
