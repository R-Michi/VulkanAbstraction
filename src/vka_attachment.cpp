#include "../include/vulkan_absraction.h"
#include <stdexcept>
#include <sstream>

vka::AttachmentImage::AttachmentImage(void) noexcept
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

vka::AttachmentImage::AttachmentImage(VkPhysicalDevice physical_device, VkDevice device) noexcept : AttachmentImage()
{
	this->_physical_device = physical_device;
	this->_device = device;
}

vka::AttachmentImage::~AttachmentImage(void)
{
	this->clear();
}

void vka::AttachmentImage::_default_img_create_info(void) noexcept
{
	this->_image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	this->_image_create_info.pNext = nullptr;
	this->_image_create_info.flags = 0;
	this->_image_create_info.imageType = VK_IMAGE_TYPE_2D;
	this->_image_create_info.extent.depth = 1;
	this->_image_create_info.mipLevels = 1;
	this->_image_create_info.arrayLayers = 1;
	this->_image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	this->_image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	this->_image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
}

void vka::AttachmentImage::_default_view_create_info(void) noexcept
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
void vka::AttachmentImage::set_image_format(VkFormat format) noexcept
{
	this->_image_create_info.format = format;
}

void vka::AttachmentImage::set_image_extent(VkExtent2D extent) noexcept
{
	this->_image_create_info.extent.width = extent.width;
	this->_image_create_info.extent.height = extent.height;
}

void vka::AttachmentImage::set_image_samples(VkSampleCountFlagBits samples) noexcept
{
	this->_image_create_info.samples = samples;
}

void vka::AttachmentImage::set_image_usage(VkImageUsageFlags usage) noexcept
{
	this->_image_create_info.usage = usage;
}

void vka::AttachmentImage::set_image_queue_family_index(uint32_t index) noexcept
{
	this->_image_create_info.queueFamilyIndexCount = 1;
	this->_image_create_info.pQueueFamilyIndices = &index;
}

// view create info setter
void vka::AttachmentImage::set_view_format(VkFormat format) noexcept
{
	this->_view_create_info.format = format;
}

void vka::AttachmentImage::set_view_components(VkComponentMapping component_mapping) noexcept
{
	this->_view_create_info.components = component_mapping;
}

void vka::AttachmentImage::set_view_aspect_mask(VkImageAspectFlags aspect_mask) noexcept
{
	this->_view_create_info.subresourceRange.aspectMask = aspect_mask;
}

// set internal requiered handles
void vka::AttachmentImage::set_physical_device(VkPhysicalDevice physical_device) noexcept
{
	this->_physical_device = physical_device;
}

void vka::AttachmentImage::set_device(VkDevice device) noexcept
{
	this->_device = device;
}


VkResult vka::AttachmentImage::create(void)
{
	if (this->_image != VK_NULL_HANDLE)
		throw std::runtime_error("Can not create attachment image without deleting the old one!");
	if (this->_physical_device == VK_NULL_HANDLE)
		throw std::invalid_argument("Physical device of attachment image is a VK_NULL_HANDLE! Requiered from attachment image create.");
	if(this->_device == VK_NULL_HANDLE)
		throw std::invalid_argument("Device of attachment image is a VK_NULL_HANDLE! Requiered from attachment image create.");

	// check if image format is supported
	if (!this->is_image_format_supported())
	{
		std::vector<VkFormat> formats;
		utility::get_supported_formats(this->_physical_device, this->_image_create_info.tiling, utility::image_usage_to_format_feature(this->_image_create_info.usage), formats);
		this->throw_unsupported_format(formats, 16);
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

VkImageView vka::AttachmentImage::view(void) const noexcept
{
	return this->_view;
}

bool vka::AttachmentImage::is_image_format_supported(void) const noexcept
{
	return utility::image_format_supported(
		this->_physical_device,
		this->_image_create_info.format,
		this->_image_create_info.tiling,
		utility::image_usage_to_format_feature(this->_image_create_info.usage)	// is ok because there should only be one bit
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