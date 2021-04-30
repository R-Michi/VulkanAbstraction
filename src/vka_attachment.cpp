#include "../include/vulkan_absraction.h"
#include <stdexcept>
#include <sstream>

vka::AttachmentImage::AttachmentImage(void)
{
	this->_physical_device = VK_NULL_HANDLE;
	this->_device = VK_NULL_HANDLE;
	
	this->_image_create_info = {};
	this->_default_img_create_info();
	this->_view_create_info = {};
	this->_default_view_create_info();

	this->_image = VK_NULL_HANDLE;
	this->_memory = VK_NULL_HANDLE;
	this->_view = VK_NULL_HANDLE;

	this->_view_create_info.image = VK_NULL_HANDLE;
}

vka::AttachmentImage::AttachmentImage(VkPhysicalDevice physical_device, VkDevice device) : AttachmentImage()
{
	this->_physical_device = physical_device;
	this->_device = device;
}

vka::AttachmentImage::~AttachmentImage(void)
{
	this->clear();
}

void vka::AttachmentImage::_default_img_create_info(void)
{
	this->_image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	this->_image_create_info.pNext = nullptr;
	this->_image_create_info.flags = 0;
	this->_image_create_info.imageType = VK_IMAGE_TYPE_2D;
	this->_image_create_info.mipLevels = 1;
	this->_image_create_info.arrayLayers = 1;
	this->_image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	this->_image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	this->_image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
}

void vka::AttachmentImage::_default_view_create_info(void)
{
	this->_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	this->_view_create_info.pNext = nullptr;
	this->_view_create_info.flags = 0;
	this->_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
	this->_view_create_info.subresourceRange.baseArrayLayer = 0;
	this->_view_create_info.subresourceRange.layerCount = 1;
	this->_view_create_info.subresourceRange.baseMipLevel = 0;
	this->_view_create_info.subresourceRange.levelCount = 1;
}

// image create info setter
void vka::AttachmentImage::set_image_format(VkFormat format)
{
	this->_image_create_info.format = format;
}

void vka::AttachmentImage::set_image_extent(VkExtent2D extent)
{
	this->_image_create_info.extent = {extent.width, extent.height, 1};
}

void vka::AttachmentImage::set_image_samples(VkSampleCountFlagBits samples)
{
	this->_image_create_info.samples = samples;
}

void vka::AttachmentImage::set_image_usage(VkImageUsageFlags usage)
{
	this->_image_create_info.usage = usage;
}

void vka::AttachmentImage::set_image_queue_family_index(uint32_t index)
{
	this->_image_create_info.queueFamilyIndexCount = 1;
	this->_image_create_info.pQueueFamilyIndices = &index;
}

// view create info setter
void vka::AttachmentImage::set_view_format(VkFormat format)
{
	this->_view_create_info.format = format;
}

void vka::AttachmentImage::set_view_components(VkComponentMapping component_mapping)
{
	this->_view_create_info.components = component_mapping;
}

void vka::AttachmentImage::set_view_aspect_mask(VkImageAspectFlags aspect_mask)
{
	this->_view_create_info.subresourceRange.aspectMask = aspect_mask;
}

// set internal requiered handles
void vka::AttachmentImage::set_physical_device(VkPhysicalDevice physical_device)
{
	this->_physical_device = physical_device;
}

void vka::AttachmentImage::set_device(VkDevice device)
{
	this->_device = device;
}


VkResult vka::AttachmentImage::create(void)
{
	if (this->_image != VK_NULL_HANDLE)
		throw std::runtime_error("Can not create attachment image withot deleting the old one!");
	if (this->_physical_device == VK_NULL_HANDLE)
		throw std::invalid_argument("Physical device of attachment image is a VK_NULL_HANDLE! Requiered from attachment image create.");
	if(this->_device == VK_NULL_HANDLE)
		throw std::invalid_argument("Device of attachment image is a VK_NULL_HANDLE! Requiered from attachment image create.");

	// check if image usage is valid
	if (this->_image_create_info.usage != VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT &&
		this->_image_create_info.usage != VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT &&
		this->_image_create_info.usage != VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT &&
		this->_image_create_info.usage != VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)
	{
		std::stringstream ss;
		ss  << "AttachmentImage does not support image usage (" << this->_image_create_info.usage << ") given by VkImageCreateInfo.\n"
			<< "Supported image usages:\n\t"
			<< VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT << ", "
			<< VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT << ", "
			<< VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT << ", "
			<< VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

		throw std::invalid_argument(ss.str());
	}

	// compare if the image views aspect mask matches the image create infos image usage
	// only for color, depth, stencil, depth/stencil for other attachments there is no check yet
	// if image usage matches the aspect mask
	if (this->_image_create_info.usage == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT && 
	   (this->_view_create_info.subresourceRange.aspectMask != VK_IMAGE_ASPECT_COLOR_BIT))
	{
		throw std::invalid_argument("Image usage given by VkImageCreateInfo of AttachmentImage is a VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT and does not match the "
									"requiered aspect mask VK_IMAGE_ASPECT_COLOR_BIT of VkImageViewCreateInfo::VkImageSubresourceRange.");
	}
	else if (this->_image_create_info.usage == VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT && 
		    !(this->_view_create_info.subresourceRange.aspectMask == VK_IMAGE_ASPECT_DEPTH_BIT || 
			  this->_view_create_info.subresourceRange.aspectMask == VK_IMAGE_ASPECT_STENCIL_BIT ||
			  this->_view_create_info.subresourceRange.aspectMask == (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT)))
	{
		throw std::invalid_argument("Image usage given by VkImageCreateInfo of AttachmentImage is a VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT and does not match the "
									"requiered aspect mask VK_IMAGE_ASPECT_DEPTH_BIT or VK_IMAGE_ASPECT_STENCIL_BIT or VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT " 
									"of VkImageViewCreateInfo::VkImageSubresourceRange.");
	}

	// pre check if format is valid only for color, depth, stencil, depth/stencil
	// for other attachments there is no check yet if the given format is supported
	if (this->_image_create_info.usage == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
	{
		// color case
		if (!this->is_image_format_supported())
		{
			std::vector<VkFormat> color_formats;
			this->get_color_formats(color_formats);
			this->throw_unsupported_format(color_formats, 16);
		}
	}
	else if (this->_image_create_info.usage == VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
	{
		if (this->_view_create_info.subresourceRange.aspectMask == VK_IMAGE_ASPECT_DEPTH_BIT)
		{
			// depth only case
			if (!this->is_image_format_supported())
			{
				std::vector<VkFormat> depth_formats;
				this->get_depth_formats(depth_formats);
				this->throw_unsupported_format(depth_formats, 16);
			}
		}
		else if (this->_view_create_info.subresourceRange.aspectMask == VK_IMAGE_ASPECT_STENCIL_BIT)
		{
			// stencil only case
			if (!this->is_image_format_supported())
			{
				std::vector<VkFormat> stencil_formats;
				this->get_stencil_formats(stencil_formats);
				this->throw_unsupported_format(stencil_formats, 16);
			}
		}
		else if (this->_view_create_info.subresourceRange.aspectMask == (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT))
		{
			// depth and stencil case
			if (!this->is_image_format_supported())
			{
				std::vector<VkFormat> depth_stencil_formats;
				this->get_depth_stencil_formats(depth_stencil_formats);
				this->throw_unsupported_format(depth_stencil_formats, 16);
			}
		}
	}

	// create image out of create info
	VkResult result = vkCreateImage(this->_device, &this->_image_create_info, nullptr, &this->_image);
	if (result != VK_SUCCESS) return result;

	// get memory requierements for image
	VkMemoryRequirements mem_req = {};
	vkGetImageMemoryRequirements(this->_device, this->_image, &mem_req);

	// allocate memory for attachment image
	VkMemoryAllocateInfo mem_alloc_info = {};
	mem_alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mem_alloc_info.pNext = nullptr;
	mem_alloc_info.allocationSize = mem_req.size;
	mem_alloc_info.memoryTypeIndex = utility::find_memory_type_index(this->_physical_device, mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	result = vkAllocateMemory(this->_device, &mem_alloc_info, nullptr, &this->_memory);
	if (result != VK_SUCCESS) return result;

	// link image with memory
	result = vkBindImageMemory(this->_device, this->_image, this->_memory, 0);
	if (result != VK_SUCCESS) return result;

	// create image view out of image and view create info
	this->_view_create_info.image = this->_image;
	result = vkCreateImageView(this->_device, &this->_view_create_info, nullptr, &this->_view);
	if (result != VK_SUCCESS) return result;

	return VK_SUCCESS;
}

void vka::AttachmentImage::clear(void)
{
	if(this->_device == VK_NULL_HANDLE)
		throw std::invalid_argument("Device of attachment image is a VK_NULL_HANDLE! Requiered from attachment image clear.");

	if (this->_view != VK_NULL_HANDLE)
	{
		vkDestroyImageView(this->_device, this->_view, nullptr);
		this->_view = VK_NULL_HANDLE;
	}

	if (this->_image != VK_NULL_HANDLE)
	{
		vkDestroyImage(this->_device, this->_image, nullptr);
		this->_image = VK_NULL_HANDLE;
		this->_view_create_info.image = this->_image;
	}

	if (this->_memory != VK_NULL_HANDLE)
	{
		vkFreeMemory(this->_device, this->_memory, nullptr);
		this->_memory = VK_NULL_HANDLE;
	}
}

VkImageView vka::AttachmentImage::view(void)
{
	return this->_view;
}

bool vka::AttachmentImage::is_image_format_supported(void)
{
	return utility::image_format_supported(
		this->_physical_device,
		this->_image_create_info.format,
		this->_image_create_info.tiling,
		utility::image_usage_to_format_feature(this->_image_create_info.usage)	// is ok because there should only be one bit
	);
}

void vka::AttachmentImage::get_color_formats(std::vector<VkFormat>& formats)
{
	utility::get_supported_color_formats(
		this->_physical_device,
		this->_image_create_info.tiling,
		utility::image_usage_to_format_feature(this->_image_create_info.usage),
		formats
	);
}

void vka::AttachmentImage::get_depth_formats(std::vector<VkFormat>& formats)
{
	utility::get_supported_depth_formats(
		this->_physical_device,
		this->_image_create_info.tiling,
		utility::image_usage_to_format_feature(this->_image_create_info.usage),
		formats
	);
}

void vka::AttachmentImage::get_stencil_formats(std::vector<VkFormat>& formats)
{
	utility::get_supported_stencil_formats(
		this->_physical_device,
		this->_image_create_info.tiling,
		utility::image_usage_to_format_feature(this->_image_create_info.usage),
		formats
	);
}

void vka::AttachmentImage::get_depth_stencil_formats(std::vector<VkFormat>& formats)
{
	utility::get_supported_depth_stencil_formats(
		this->_physical_device,
		this->_image_create_info.tiling,
		utility::image_usage_to_format_feature(this->_image_create_info.usage),
		formats
	);
}

void vka::AttachmentImage::throw_unsupported_format(const std::vector<VkFormat>& formats, size_t block_size)
{
	// alternative formats
	std::stringstream ss;
	ss << "Image format (" << this->_image_create_info.format << ") given by VkImageCreateInfo is not supported.\nAlternative image formats: ";

	size_t n = 0, rn = formats.size();
	while (n < formats.size())
	{
		ss << "\n\t";
		for (size_t i = 0; i < ((rn < block_size) ? rn : block_size); i++)
		{
			if (n == formats.size() - 1)
				ss << formats.at(n);
			else
				ss << formats.at(n) << ", ";
			++n;
		}
		rn -= block_size;
	}
	throw std::invalid_argument(ss.str());
}