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

constexpr void vka::utility::get_color_formats(VkFormat* formats) noexcept
{
    for (uint32_t i = 1; i <= 123; i++)
        formats[i] = static_cast<VkFormat>(i);
}

constexpr void vka::utility::get_depth_formats(VkFormat* formats) noexcept
{
    formats[0] = VK_FORMAT_D16_UNORM;
    formats[1] = VK_FORMAT_X8_D24_UNORM_PACK32;
    formats[2] = VK_FORMAT_D32_SFLOAT;
    formats[3] = VK_FORMAT_D16_UNORM_S8_UINT;
    formats[4] = VK_FORMAT_D24_UNORM_S8_UINT;
    formats[5] = VK_FORMAT_D32_SFLOAT_S8_UINT;
}

constexpr void vka::utility::get_stencil_formats(VkFormat* formats) noexcept
{
    formats[0] = VK_FORMAT_S8_UINT;
    formats[1] = VK_FORMAT_D16_UNORM_S8_UINT;
    formats[2] = VK_FORMAT_D24_UNORM_S8_UINT;
    formats[3] = VK_FORMAT_D32_SFLOAT_S8_UINT;
}

constexpr void vka::utility::get_depth_stencil_formats(VkFormat* formats) noexcept
{
    formats[0] = VK_FORMAT_D16_UNORM_S8_UINT;
    formats[1] = VK_FORMAT_D24_UNORM_S8_UINT;
    formats[2] = VK_FORMAT_D32_SFLOAT_S8_UINT;
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

constexpr VkImageUsageFlags vka::utility::cvt_ff2iu(VkFormatFeatureFlags format_feature) noexcept
{
    constexpr size_t n_bits = 8 * sizeof(VkFlags);
    
    VkImageUsageFlags flags = 0;
    for (size_t i = 0; i < n_bits; i++)
        flags |= detail::utility::ff2iu_bit(static_cast<VkFormatFeatureFlagBits>(format_feature & (1 << i)));
    return flags;
}

constexpr VkFormatFeatureFlags vka::utility::cvt_iu2ff(VkImageUsageFlags image_usage) noexcept
{
    constexpr size_t n_bits = 8 * sizeof(VkFlags);
    
    VkFormatFeatureFlags flags = 0;
    for (size_t i = 0; i < n_bits; i++)
        flags |= detail::utility::iu2ff_bit(static_cast<VkImageUsageFlagBits>(image_usage & (1 << i)));
    return flags;
}

void vka::utility::cvt_stdstr2ccpv(const std::vector<std::string>& std_in, const char** ccp_out) noexcept
{
    for (size_t i = 0; i < std_in.size(); i++)
        ccp_out[i] = std_in[i].c_str();
}

VkResult vka::utility::execute_scb(VkDevice device, VkCommandPool pool, VkQueue queue, uint32_t count, const VkCommandBuffer* scbs, VkFence fence, uint64_t timeout) noexcept
{
    if (device == VK_NULL_HANDLE || pool == VK_NULL_HANDLE || queue == VK_NULL_HANDLE)
        return VK_RESULT_MAX_ENUM;

    const VkCommandBufferAllocateInfo ai = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };

    detail::utility::CommandBuffer cbo(device, ai);
    VkResult res = cbo.result();
    if(res != VK_SUCCESS) return res;

    const VkCommandBufferBeginInfo bi = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = nullptr
    };

    res = vkBeginCommandBuffer(cbo, &bi);
    if (res != VK_SUCCESS) return res;

    vkCmdExecuteCommands(cbo, count, scbs);

    res = vkEndCommandBuffer(cbo);
    if (res != VK_SUCCESS) return res;

    const VkSubmitInfo si = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = nullptr,
        .commandBufferCount = 1,
        .pCommandBuffers = &cbo,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = nullptr
    };

    res = vkQueueSubmit(queue, 1, &si, fence);
    if (res != VK_SUCCESS) return res;

    if (fence == VK_NULL_HANDLE)    res = vkQueueWaitIdle(queue);
    else                            res = vkWaitForFences(device, 1, &fence, VK_TRUE, timeout);
    return res;
}

/*
* Initialize the map only once at the first
* call to that function.
* Every subsequent call just returns the size of
* the given format.
*/
size_t vka::utility::format_sizeof(VkFormat format)
{
    static std::unordered_map<VkFormat, size_t> f2s;    // creates an empty map
    if (f2s.empty())
        detail::utility::init_format_sizeof(f2s);
    return f2s.at(format);
}
