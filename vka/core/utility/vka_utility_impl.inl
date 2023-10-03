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

uint32_t vka::utility::find_memory_type_index(const VkPhysicalDeviceMemoryProperties& properties, uint32_t bits, VkMemoryPropertyFlags req_flags) noexcept
{
    // If a memory type is supported, the corresponding bit is set in the memoryTypeBit bit mask.
    // This bit mask is specified by 'bits'. This loop iterates over all set bits in the mask and
    // checks, if the requiered flags 'req_flags' are contained in the memoryType's propertyFlags.
    // If this is the case, the index of the memory type is returned.
    for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
    {
        if ((bits & (0b1 << i)) && (properties.memoryTypes[i].propertyFlags & req_flags) == req_flags)
            return i;
    }
    return vka::NPOS32;
}

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

VkResult vka::utility::begin_cbo(VkDevice device, VkCommandPool pool, VkCommandBuffer& cbo) noexcept
{
    const VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    VkCommandBuffer cbo2 = VK_NULL_HANDLE;  // prevent using reference argument, should I ever remove the inline property of this function
    const VkResult res = vkAllocateCommandBuffers(device, &alloc_info, &cbo2);
    cbo = cbo2;
    if (res != VK_SUCCESS) return res;

    constexpr VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = nullptr
    };
    return vkBeginCommandBuffer(cbo2, &begin_info);
}

VkResult vka::utility::end_cbo(VkQueue queue, VkCommandBuffer cbo, VkFence fence) noexcept
{
    return detail::utility::end_cbo_and_submit(queue, cbo, fence);
}

VkResult vka::utility::end_wait_cbo(VkQueue queue, VkCommandBuffer cbo, VkDevice device, VkFence fence, uint64_t timeout) noexcept
{
    const VkResult res = detail::utility::end_cbo_and_submit(queue, cbo, fence);
    if (res != VK_SUCCESS) return res;
    return (fence == VK_NULL_HANDLE) ? vkQueueWaitIdle(queue) : vkWaitForFences(device, 1, &fence, VK_TRUE, timeout);
}
