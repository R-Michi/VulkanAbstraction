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

uint32_t vka::utility::find_memory_type_index(VkPhysicalDevice device, uint32_t bits, VkMemoryPropertyFlags req_flags)
{
    VkPhysicalDeviceMemoryProperties properties = {};
    vkGetPhysicalDeviceMemoryProperties(device, &properties);
    
    for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
    {
        if ((bits & (0b1 << i)) && (properties.memoryTypes[i].propertyFlags & req_flags) == req_flags)
            return i;
    }
    
    char str[64];
    sprintf(str, "%" PRIu32, static_cast<uint32_t>(req_flags));
    throw std::invalid_argument("[vka::utility::find_memory_type_index]: Physical device does not support requiered memory property flags: " + std::string(str));
    
    return vka::NPOS;
}

bool vka::utility::supports_format_feature(VkPhysicalDevice physical_device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature)
{
    // get the format properties from the format
    VkFormatProperties properties = {};
    vkGetPhysicalDeviceFormatProperties(physical_device, format, &properties);
    
    // check if the format properties of the given format supports the given format features
    if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & format_feature) == format_feature) return true;
    if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & format_feature) == format_feature) return true;
    return false;
}

constexpr void vka::utility::get_color_formats(std::vector<VkFormat>& formats)
{
    formats.clear();
    for (int i = 1; i <= 123; i++)
        formats.push_back(static_cast<VkFormat>(i));
}

constexpr void vka::utility::get_depth_formats(std::vector<VkFormat>& formats)
{
    formats = {
        VK_FORMAT_D16_UNORM, VK_FORMAT_X8_D24_UNORM_PACK32, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT
    };
}

constexpr void vka::utility::get_stencil_formats(std::vector<VkFormat>& formats)
{
    formats = {
        VK_FORMAT_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT
    };
}

constexpr void vka::utility::get_depth_stencil_formats(std::vector<VkFormat>& formats)
{
    formats = {
        VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT
    };
}

void vka::utility::get_supported_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats)
{
    formats.clear();
    for (int i = 0; i <= 184; i++)
    {
        if (supports_format_feature(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (int i = 1000156000; i <= 1000156033; i++)
    {
        if (supports_format_feature(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (int i = 1000330000; i <= 1000330003; i++)
    {
        if (supports_format_feature(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (int i = 1000340000; i <= 1000340001; i++)
    {
        if (supports_format_feature(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (int i = 1000054000; i <= 1000054007; i++)
    {
        if (supports_format_feature(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (int i = 1000066000; i <= 1000066013; i++)
    {
        if (supports_format_feature(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    // format 1000464000
    if (supports_format_feature(physical_device, VK_FORMAT_R16G16_S10_5_NV, tiling, format_feature))
        formats.push_back(VK_FORMAT_R16G16_S10_5_NV);
}

constexpr VkImageUsageFlags vka::utility::cvt_ff2iu(VkFormatFeatureFlags format_feature)
{
    constexpr size_t n_bits = 8 * sizeof(VkFlags);
    
    VkImageUsageFlags flags = 0;
    for (size_t i = 0; i < n_bits; i++)
        flags |= detail::utility::ff2iu_bit(static_cast<VkFormatFeatureFlagBits>(format_feature & (1 << i)));
    return flags;
}

constexpr VkFormatFeatureFlags vka::utility::cvt_iu2ff(VkImageUsageFlags image_usage)
{
    constexpr size_t n_bits = 8 * sizeof(VkFlags);
    
    VkFormatFeatureFlags flags = 0;
    for (size_t i = 0; i < n_bits; i++)
        flags |= detail::utility::iu2ff_bit(static_cast<VkImageUsageFlagBits>(image_usage & (1 << i)));
    return flags;
}

void vka::utility::cvt_stdstr2ccpv(const std::vector<std::string>& std_in, std::vector<const char*>& ccp_out)
{
    ccp_out.clear();
    for (const std::string& str : std_in)
        ccp_out.push_back(str.c_str());
}

VkResult vka::utility::execute_scb(VkDevice device, VkCommandPool pool, VkQueue queue, uint32_t count, const VkCommandBuffer* scbs)
{
    if (device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::utility::execute_scb]: Device is a VK_NULL_HANDLE!");
    if (pool == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::utility::execute_scb]: Command pool VK_NULL_HANDLE!");
    if (queue == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::utility::execute_scb]: Command queue VK_NULL_HANDLE!");

    VkCommandBufferAllocateInfo ai = {};
    ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.pNext = nullptr;
    ai.commandPool = pool;
    ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = 1;

    detail::utility::CommandBufferWrapper cbow(device, ai);
    VkResult res = cbow.result();
    if(res != VK_SUCCESS) return res;
    const VkCommandBuffer cbo = cbow.get().at(0);

    VkCommandBufferBeginInfo bi = {};
    bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bi.pNext = nullptr;
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    bi.pInheritanceInfo = nullptr;

    res = vkBeginCommandBuffer(cbo, &bi);
    if (res != VK_SUCCESS) return res;

    vkCmdExecuteCommands(cbo, count, scbs);

    res = vkEndCommandBuffer(cbo);
    if (res != VK_SUCCESS) return res;

    VkSubmitInfo si = {};
    si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    si.pNext = nullptr;
    si.waitSemaphoreCount = 0;
    si.pWaitSemaphores = nullptr;
    si.pWaitDstStageMask = nullptr;
    si.commandBufferCount = 1;
    si.pCommandBuffers = &cbo;
    si.signalSemaphoreCount = 0;
    si.pSignalSemaphores = nullptr;

    res = vkQueueSubmit(queue, 1, &si, VK_NULL_HANDLE);
    if (res != VK_SUCCESS) return res;

    res = vkQueueWaitIdle(queue);
    return res;
}

/*
* Initialize the map only once at the first
* call to that function.
* Every subsequent call just returns the size of
* the given format.
*/
size_t vka::utility::format_sizeof(VkFormat format) noexcept
{
    static std::unordered_map<VkFormat, size_t> f2s;
    if (f2s.empty())
        detail::utility::init_format_sizeof(f2s);
    return f2s[format];
}
