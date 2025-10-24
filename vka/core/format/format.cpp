/**
 * @brief Implementation for the format functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include <vka/vka.h>

bool vka::format::supports_feature2(VkPhysicalDevice device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature) noexcept
{
    VkFormatProperties properties;
    vkGetPhysicalDeviceFormatProperties(device, format, &properties);
    return supports_feature(properties, tiling, format_feature);
}

std::vector<VkFormat> vka::format::get_supported(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature)
{
    std::vector<VkFormat> formats;
    formats.reserve(324);

    // VK_VERSION_1_0 (non-extension) formats, (range [0, 184]):
    for (uint32_t i = 0; i <= 184; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }

    // VK_VERSION_1_1 (non-extension) formats, (range [1000156000, 1000156033]):
    for (uint32_t i = 1000156000; i <= 1000156033; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }

    // VK_VERSION_1_3 (non-extension) formats, (range [1000330000, 1000330003]):
    for (uint32_t i = 1000330000; i <= 1000330003; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }

    // VK_VERSION_1_3 (non-extension) formats, (range [1000340000, 1000340001]):
    for (uint32_t i = 1000340000; i <= 1000340001; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }

    // VK_VERSION_1_3 (non-extension) formats, (range [1000066000, 1000066013]):
    for (uint32_t i = 1000066000; i <= 1000066013; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }

    // VK_VERSION_1_4 (non-extension) formats, (range [1000470000, 1000470001]):
    for (uint32_t i = 1000470000; i <= 1000470001; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }

    // VK_IMG_format_pvrtc (extension) formats, (range [1000054000, 1000054007]):
    for (uint32_t i = 1000054000; i <= 1000054007; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }

    // VK_ARM_tensors (extension) formats, (range [1000460000, 1000460000]):
    if (supports_feature2(physical_device, VK_FORMAT_R8_BOOL_ARM, tiling, format_feature))
        formats.push_back(VK_FORMAT_R8_BOOL_ARM);

    // VK_NV_optical_flow (extension) formats, (range [1000464000, 1000464000]):
    if (supports_feature2(physical_device, VK_FORMAT_R16G16_SFIXED5_NV, tiling, format_feature))
        formats.push_back(VK_FORMAT_R16G16_SFIXED5_NV);

    // VK_ARM_format_pack (extension) formats, (range [1000609000, 1000609013]):
    for (uint32_t i = 1000609000; i <= 1000609013; i++)
    {
        if (supports_feature2(physical_device, static_cast<VkFormat>(i), tiling, format_feature))
            formats.push_back(static_cast<VkFormat>(i));
    }

    return formats;
}
