#include "../include/vulkan_absraction.h"
#include <stdexcept>
#include <string>
#include <string.h>
#include <inttypes.h>
#include <sstream>

uint32_t vka::utility::find_memory_type_index(VkPhysicalDevice physical_device, uint32_t bits, VkMemoryPropertyFlags req_flags)
{
	VkPhysicalDeviceMemoryProperties properties = {};
	vkGetPhysicalDeviceMemoryProperties(physical_device, &properties);

	for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
	{
		if ((bits & (0b1 << i)) && (properties.memoryTypes[i].propertyFlags & req_flags) == req_flags)
			return i;
	}

	char str[64];
	sprintf(str, "%" PRIu32, static_cast<uint32_t>(req_flags));
	throw std::invalid_argument("Physical device does not support requiered memory property flags: " + std::string(str));
	
	return ~(0U);
}

bool vka::utility::image_format_supported(VkPhysicalDevice physical_device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature)
{
	// get the format properties from the format
	VkFormatProperties properties = {};
	vkGetPhysicalDeviceFormatProperties(physical_device, format, &properties);

	// check if the format properties of the given format supports the given format features
	if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & format_feature) == format_feature) return true;
	if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & format_feature) == format_feature) return true;
	return false;
}


void vka::utility::get_color_formats(std::vector<VkFormat>& formats)
{
	formats.clear();
	// color formats are VkFormat enums from 1 - 123
	for (int i = 1; i <= 123; i++)
		formats.push_back(static_cast<VkFormat>(i));
}

void vka::utility::get_depth_formats(std::vector<VkFormat>& formats)
{
	// all aviable depth formats
	formats = {
		VK_FORMAT_D16_UNORM, VK_FORMAT_X8_D24_UNORM_PACK32, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT
	};
}

void vka::utility::get_stencil_formats(std::vector<VkFormat>& formats)
{	
	// all aviable stencil formats
	formats = {
		VK_FORMAT_S8_UINT, VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT
	};
}

void vka::utility::get_depth_stencil_formats(std::vector<VkFormat>& formats)
{
	// all aviable combined depth and stencil formats
	formats = {
		VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT, VK_FORMAT_D32_SFLOAT_S8_UINT
	};
}


void vka::utility::get_supported_color_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats)
{
	std::vector<VkFormat> color_formats;
	get_color_formats(color_formats);

	formats.clear();
	for (VkFormat format : color_formats)
	{
		if (image_format_supported(physical_device, format, tiling, format_feature))
			formats.push_back(format);
	}
}

void vka::utility::get_supported_depth_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats)
{
	std::vector<VkFormat> depth_formats;
	get_depth_formats(depth_formats);

	formats.clear();
	for (VkFormat format : depth_formats)
	{
		if (image_format_supported(physical_device, format, tiling, format_feature))
			formats.push_back(format);
	}
}

void vka::utility::get_supported_stencil_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats)
{
	std::vector<VkFormat> stencil_formats;
	get_stencil_formats(stencil_formats);

	formats.clear();
	for (VkFormat format : stencil_formats)
	{
		if (image_format_supported(physical_device, format, tiling, format_feature))
			formats.push_back(format);
	}
}

void vka::utility::get_supported_depth_stencil_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats)
{
	std::vector<VkFormat> depth_stencil_formats;
	get_depth_stencil_formats(depth_stencil_formats);

	formats.clear();
	for (VkFormat format : depth_stencil_formats)
	{
		if (image_format_supported(physical_device, format, tiling, format_feature))
			formats.push_back(format);
	}
}


VkImageUsageFlagBits format_feature_to_image_usage_bit(VkFormatFeatureFlagBits format_feature)
{
	switch (format_feature)
	{
	case VK_FORMAT_FEATURE_TRANSFER_SRC_BIT:
		return VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	case VK_FORMAT_FEATURE_TRANSFER_DST_BIT:
		return VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	case VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT:
		return VK_IMAGE_USAGE_SAMPLED_BIT;
	case VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT:
		return VK_IMAGE_USAGE_STORAGE_BIT;
	case VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT:
		return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	case VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT:
		return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	case VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR:
		return VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV;
	case VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT:
		return VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT;
	case VK_FORMAT_FEATURE_FLAG_BITS_MAX_ENUM:
		return VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM;
	default:
		return static_cast<VkImageUsageFlagBits>(0);
	};
	return static_cast<VkImageUsageFlagBits>(0);
}

VkFormatFeatureFlagBits image_usage_to_format_feature_bit(VkImageUsageFlagBits image_usage)
{
	switch (image_usage)
	{
	case VK_IMAGE_USAGE_TRANSFER_SRC_BIT:
		return VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
	case VK_IMAGE_USAGE_TRANSFER_DST_BIT:
		return VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
	case VK_IMAGE_USAGE_SAMPLED_BIT:
		return VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
	case VK_IMAGE_USAGE_STORAGE_BIT:
		return VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
	case VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT:
		return VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
	case VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT:
		return VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
	case VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV:
		return VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
	case VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT:
		return VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT;
	case VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM:
		return VK_FORMAT_FEATURE_FLAG_BITS_MAX_ENUM;
	default:
		return static_cast<VkFormatFeatureFlagBits>(0);

	};
	return static_cast<VkFormatFeatureFlagBits>(0);
}

VkImageUsageFlags vka::utility::format_feature_to_image_usage(VkFormatFeatureFlags format_feature)
{
	static constexpr size_t n_bits = 8 * sizeof(VkFlags);

	VkImageUsageFlags flags = 0;
	for (size_t i = 0; i < n_bits; i++)
		flags |= format_feature_to_image_usage_bit(static_cast<VkFormatFeatureFlagBits>(format_feature & (1 << i)));
	return flags;
}

VkFormatFeatureFlags vka::utility::image_usage_to_format_feature(VkImageUsageFlags image_usage)
{
	static constexpr size_t n_bits = 8 * sizeof(VkFlags);

	VkFormatFeatureFlags flags = 0;
	for (size_t i = 0; i < n_bits; i++)
		flags |= image_usage_to_format_feature_bit(static_cast<VkImageUsageFlagBits>(image_usage & (1 << i)));
	return flags;
}