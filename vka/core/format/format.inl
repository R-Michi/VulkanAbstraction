/**
 * @brief Implementation for the format functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

bool vka::format::supports_feature2(VkPhysicalDevice device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept
{
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(device, format, &properties);
    return supports_feature(properties, tiling, format_feature);
}

std::vector<VkFormat> vka::format::get_supported(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature)
{
    std::vector<VkFormat> formats;
    for (uint32_t i = 0; i <= 184; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000156000; i <= 1000156033; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000330000; i <= 1000330003; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000340000; i <= 1000340001; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000054000; i <= 1000054007; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    for (uint32_t i = 1000066000; i <= 1000066013; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }
    // format 1000464000
    if (supports_feature2(physical_device, VK_FORMAT_R16G16_S10_5_NV, tiling, format_feature))
        formats.push_back(VK_FORMAT_R16G16_S10_5_NV);

    return formats;
}
