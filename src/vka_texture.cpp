#include "../include/vulkan_absraction.h"
#include <stdexcept>
#include <sstream>
#include <initializer_list>
#include <algorithm>

vka::Texture::Texture(void) noexcept
{
	this->_image_create_info = {};
	this->_default_img_create_info();
	this->_view_create_info = {};
	this->_default_view_create_info();
	this->_sampler_create_info = {};
	this->_default_sampler_create_info();

	this->_physical_device	= VK_NULL_HANDLE;
	this->_device			= VK_NULL_HANDLE;
	this->_command_pool		= VK_NULL_HANDLE;
	this->_command_queue	= VK_NULL_HANDLE;

	this->_image			= VK_NULL_HANDLE;
	this->_memory			= VK_NULL_HANDLE;
	this->_view				= VK_NULL_HANDLE;
	this->_sampler			= VK_NULL_HANDLE;
	this->_mipmap			= false;
	this->_mipfilter		= VK_FILTER_NEAREST;

	this->_size				= 0;
	this->_px_count			= 0;

	this->_view_create_info.image = this->_image;
}

vka::Texture::Texture(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool, VkQueue queue) noexcept : Texture()
{
	this->_physical_device = physical_device;
	this->_device = device;
	this->_command_pool = cmd_pool;
	this->_command_queue = queue;
}

vka::Texture::~Texture(void)
{
	this->clear();
}

void vka::Texture::_default_img_create_info(void) noexcept
{
	this->_image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	this->_image_create_info.pNext = nullptr;
	this->_image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	this->_image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
	this->_image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
	this->_image_create_info.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	this->_image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	this->_image_create_info.mipLevels = 1;
}

void vka::Texture::_default_view_create_info(void) noexcept
{
	this->_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	this->_view_create_info.pNext = nullptr;
}

void vka::Texture::_default_sampler_create_info(void) noexcept
{
	this->_sampler_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	this->_sampler_create_info.pNext = nullptr;
}

// image create info
void vka::Texture::set_image_flags(VkImageCreateFlags flags) noexcept
{
	this->_image_create_info.flags = flags;
}

void vka::Texture::set_image_type(VkImageType type) noexcept
{
	this->_image_create_info.imageType = type;
}

void vka::Texture::set_image_format(VkFormat format) noexcept
{
	this->_image_create_info.format = format;
}
 
void vka::Texture::set_image_extent(VkExtent3D extent) noexcept
{
	this->_image_create_info.extent = extent;
}

void vka::Texture::set_image_array_layers(uint32_t layers) noexcept
{
	this->_image_create_info.arrayLayers = layers;
}

void vka::Texture::set_image_queue_families(uint32_t queue_family_index) noexcept
{
	this->_image_create_info.queueFamilyIndexCount = 1;
	this->_image_create_info.pQueueFamilyIndices = &queue_family_index;
}

void vka::Texture::set_image_create_info(const VkImageCreateInfo& create_info) noexcept
{
	this->_image_create_info = create_info;
	this->_default_img_create_info();
}

// image view create info
void vka::Texture::set_view_type(VkImageViewType type) noexcept
{
	this->_view_create_info.viewType = type;
}

void vka::Texture::set_view_format(VkFormat format) noexcept
{
	this->_view_create_info.format = format;
}

void vka::Texture::set_view_components(VkComponentMapping component_mapping) noexcept
{
	this->_view_create_info.components = component_mapping;
}

void vka::Texture::set_view_subresource_range(VkImageSubresourceRange subressource_range) noexcept
{
	this->_view_create_info.subresourceRange = subressource_range;
}

void vka::Texture::set_view_create_info(const VkImageViewCreateInfo& create_info) noexcept
{
	this->_view_create_info = create_info;
	this->_view_create_info.image = this->_image;
	this->_default_view_create_info();
}

// sampler create info
void vka::Texture::set_sampler_mag_filter(VkFilter mag_filter) noexcept
{
	this->_sampler_create_info.magFilter = mag_filter;
}

void vka::Texture::set_sampler_min_filter(VkFilter min_filter) noexcept
{
	this->_sampler_create_info.minFilter = min_filter;
}

void vka::Texture::set_sampler_mipmap_mode(VkSamplerMipmapMode mode) noexcept
{
	this->_sampler_create_info.mipmapMode = mode;
}

void vka::Texture::set_sampler_address_mode(VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w) noexcept
{
	this->_sampler_create_info.addressModeU = u;
	this->_sampler_create_info.addressModeV = v;
	this->_sampler_create_info.addressModeW = w;
}

void vka::Texture::set_sampler_mip_lod_bias(float bias) noexcept
{
	this->_sampler_create_info.mipLodBias = bias;
}

void vka::Texture::set_sampler_anisotropy_enable(bool enable) noexcept
{
	this->_sampler_create_info.anisotropyEnable = (enable) ? VK_TRUE : VK_FALSE;
}

void vka::Texture::set_sampler_max_anisotropy(float max) noexcept
{
	this->_sampler_create_info.maxAnisotropy = max;
}

void vka::Texture::set_sampler_compare_enable(bool enable) noexcept
{
	this->_sampler_create_info.compareEnable = (enable) ? VK_TRUE : VK_FALSE;
}

void vka::Texture::set_sampler_compare_op(VkCompareOp op) noexcept
{
	this->_sampler_create_info.compareOp = op;
}

void vka::Texture::set_sampler_lod(float min_lod, float max_lod) noexcept
{ 
	this->_sampler_create_info.minLod = min_lod;
	this->_sampler_create_info.maxLod = max_lod;
}

void vka::Texture::set_sampler_border_color(VkBorderColor border_color) noexcept
{
	this->_sampler_create_info.borderColor = border_color;
}

void vka::Texture::set_sampler_unnormalized_coordinates(bool unnormalized) noexcept
{
	this->_sampler_create_info.unnormalizedCoordinates = (unnormalized) ? VK_TRUE : VK_FALSE;
}

void vka::Texture::set_sampler_create_info(const VkSamplerCreateInfo& create_info) noexcept
{
	this->_sampler_create_info = create_info;
	this->_default_sampler_create_info();
}

// set handles for internal usage
void vka::Texture::set_pyhsical_device(VkPhysicalDevice physical_device) noexcept
{
	this->_physical_device = physical_device;
}

void vka::Texture::set_device(VkDevice device) noexcept
{
	this->_device = device;
}

void vka::Texture::set_command_pool(VkCommandPool command_pool) noexcept
{
	this->_command_pool = command_pool;
}

void vka::Texture::set_queue(VkQueue queue) noexcept
{
	this->_command_queue = queue;
}


void vka::Texture::mipmap_generate(bool generate) noexcept
{
	this->_mipmap = generate;
}

void vka::Texture::mipmap_filter(VkFilter filter) noexcept
{
	this->_mipfilter = filter;
}


VkResult vka::Texture::create(const void* pdata, size_t pixel_stride)
{
	if (this->_image != VK_NULL_HANDLE)
		throw std::runtime_error("Can not create texture withot deleting the old one!");
	if (this->_physical_device == VK_NULL_HANDLE)
		throw std::invalid_argument("Physical device of texture is a VK_NULL_HANDLE! Requiered from texture create.");
	if (this->_device == VK_NULL_HANDLE)
		throw std::invalid_argument("Device of texture is a VK_NULL_HANDLE! Requiered from texture create.");
	if (this->_command_pool == VK_NULL_HANDLE)
		throw std::invalid_argument("Command pool of texture is a VK_NULL_HANDLE! Requiered from texture create.");
	if (this->_command_queue == VK_NULL_HANDLE)
		throw std::invalid_argument("Queue of texture is a VK_NULL_HANDLE! Requiered from texture create.");

	const VkExtent3D& extent = this->_image_create_info.extent;
	this->_size = extent.width * extent.height * extent.depth * pixel_stride;
	this->_px_count = extent.width * extent.height * extent.depth;

	// calculate mip levels
	if (this->_mipmap)
	{
		std::initializer_list<uint32_t> list = { extent.width, extent.height, extent.depth };
		this->_image_create_info.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(list)))) + 1;
	}
	

	// create staging buffer for image
	Buffer staging_buffer(this->_physical_device, this->_device);
	staging_buffer.set_create_flags(0);
	staging_buffer.set_create_size(this->_size);
	staging_buffer.set_create_usage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	staging_buffer.set_create_sharing_mode(VK_SHARING_MODE_EXCLUSIVE);
	staging_buffer.set_create_queue_families(this->_image_create_info.pQueueFamilyIndices, this->_image_create_info.queueFamilyIndexCount);
	staging_buffer.set_memory_properties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	
	VkResult result = staging_buffer.create();
	if (result != VK_SUCCESS) return result;

	// copy image data into staging buffer
	void* _map = staging_buffer.map(this->_size, 0);
	memcpy(_map, pdata, this->_size);
	staging_buffer.unmap();

	// create image handle
	result = vkCreateImage(this->_device, &this->_image_create_info, nullptr, &this->_image);
	if (result != VK_SUCCESS) return result;

	// get image memory requierements, used for the actual memory of the image 
	VkMemoryRequirements mem_req = {};
	vkGetImageMemoryRequirements(this->_device, this->_image, &mem_req);

	// allocate memory for image
	VkMemoryAllocateInfo memory_alloc_info = {};
	memory_alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memory_alloc_info.pNext = nullptr;
	memory_alloc_info.allocationSize = mem_req.size;
	memory_alloc_info.memoryTypeIndex = utility::find_memory_type_index(this->_physical_device, mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	result = vkAllocateMemory(this->_device, &memory_alloc_info, nullptr, &this->_memory);
	if (result != VK_SUCCESS) return result;
	
	// link memory with image
	result = vkBindImageMemory(this->_device, this->_image, this->_memory, 0);
	if (result != VK_SUCCESS) return result;

	// begin recording of commands
	VkCommandBuffer command_buffer;
	result = this->record_commands(command_buffer);
	if (result != VK_SUCCESS) return result;

	// change layout to be able to copy data into the image
	this->transform_image_layout(command_buffer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	// copy staging buffer to image
	this->buffer_copy(command_buffer, staging_buffer.handle(), this->_image, this->_image_create_info.arrayLayers, extent);
	if (this->_mipmap)
	{
		// generate mipmaps and the image gets automatically transisioned to the VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL layout.
		this->generate_mipmaps(command_buffer);
	}
	else
	{
		// change layout again that the shader can read the texture most efficient
		// transision the whole image if no mipmaps should be generated
		this->transform_image_layout(command_buffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	// execute recorded commands
	result = this->execute_commands(command_buffer);
	if (result != VK_SUCCESS) return result;

	// create view out of image
	this->_view_create_info.image = this->_image;
	this->_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	result = vkCreateImageView(this->_device, &this->_view_create_info, nullptr, &this->_view);
	if (result != VK_SUCCESS) return result;

	// create sampler
	result = vkCreateSampler(this->_device, &this->_sampler_create_info, nullptr, &this->_sampler);
	if (result != VK_SUCCESS) return result;

	return VK_SUCCESS;
}

void vka::Texture::clear(void)
{
	if (this->_device == VK_NULL_HANDLE)
		throw std::invalid_argument("Device of texture is a VK_NULL_HANDLE! Requiered from texture clear.");

	if (this->_sampler != VK_NULL_HANDLE)
	{
		vkDestroySampler(this->_device, this->_sampler, nullptr);
		this->_sampler = VK_NULL_HANDLE;
	}

	if (this->_view != VK_NULL_HANDLE)
	{
		vkDestroyImageView(this->_device, this->_view, nullptr);
		this->_view = VK_NULL_HANDLE;
	}

	if (this->_image != VK_NULL_HANDLE)
	{
		vkDestroyImage(this->_device, this->_image, nullptr);
		this->_image = VK_NULL_HANDLE;
	}

	if (this->_memory != VK_NULL_HANDLE)
	{
		vkFreeMemory(this->_device, this->_memory, nullptr);
		this->_memory = VK_NULL_HANDLE;
	}
}

VkImageView vka::Texture::view(void) const noexcept
{
	return this->_view;
}

VkSampler vka::Texture::sampler(void) const noexcept
{
	return this->_sampler;
}

size_t vka::Texture::size(void) const noexcept
{
	return this->_size;
}

size_t vka::Texture::count(void) const noexcept
{
	return this->_px_count;
}

uint32_t vka::Texture::mip_levels(void) const noexcept
{
	const VkExtent3D& extent = this->_image_create_info.extent;
	std::initializer_list<uint32_t> list = { extent.width, extent.height, extent.depth };

	return (this->_mipmap) ? static_cast<uint32_t>(std::floor(std::log2(std::max(list)))) + 1 : 1;
}

uint32_t vka::Texture::array_layers(void) const noexcept
{
	return this->_image_create_info.arrayLayers;
}


VkResult vka::Texture::record_commands(VkCommandBuffer& command_buffer)
{
	VkCommandBufferAllocateInfo command_buffer_alloc_info = {};
	command_buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_alloc_info.pNext = nullptr;
	command_buffer_alloc_info.commandPool = this->_command_pool;
	command_buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_alloc_info.commandBufferCount = 1;

	VkResult result = vkAllocateCommandBuffers(this->_device, &command_buffer_alloc_info, &command_buffer);
	if (result != VK_SUCCESS) return result;

	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.pNext = nullptr;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	begin_info.pInheritanceInfo = nullptr;

	result = vkBeginCommandBuffer(command_buffer, &begin_info);
	if (result != VK_SUCCESS) return result;

	return VK_SUCCESS;
}

VkResult vka::Texture::execute_commands(VkCommandBuffer command_buffer)
{
	VkResult result = vkEndCommandBuffer(command_buffer);
	if (result != VK_SUCCESS) return result;

	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pNext = nullptr;
	submit_info.waitSemaphoreCount = 0;
	submit_info.pWaitSemaphores = nullptr;
	submit_info.pWaitDstStageMask = nullptr;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &command_buffer;
	submit_info.signalSemaphoreCount = 0;
	submit_info.pSignalSemaphores = nullptr;

	result = vkQueueSubmit(this->_command_queue, 1, &submit_info, VK_NULL_HANDLE);
	if (result != VK_SUCCESS) return result;

	result = vkQueueWaitIdle(this->_command_queue);
	if (result != VK_SUCCESS) return result;

	vkFreeCommandBuffers(this->_device, this->_command_pool, 1, &command_buffer);
	return VK_SUCCESS;
}


void vka::Texture::buffer_copy(VkCommandBuffer command_buffer, VkBuffer buffer, VkImage image, uint32_t array_layers, VkExtent3D extent)
{
	// copy information
	VkBufferImageCopy buffer_image_copy = {};
	buffer_image_copy.bufferOffset = 0;
	buffer_image_copy.bufferRowLength = 0;
	buffer_image_copy.bufferImageHeight = 0;
	buffer_image_copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	buffer_image_copy.imageSubresource.mipLevel = 0;	// the mip levels are generated by the GPU via commands, there is only one in the staging buffer
	buffer_image_copy.imageSubresource.baseArrayLayer = 0;
	buffer_image_copy.imageSubresource.layerCount = array_layers;
	buffer_image_copy.imageOffset = { 0,0,0 };
	buffer_image_copy.imageExtent = extent;

	// copy...
	vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &buffer_image_copy);
}

void vka::Texture::transform_image_layout(VkCommandBuffer command_buffer, VkImageLayout _old, VkImageLayout _new)
{
	VkPipelineStageFlags src_flags, dst_flags;
	VkImageMemoryBarrier image_memory_barrier = {};
	image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	image_memory_barrier.pNext = nullptr;

	if ((_old == VK_IMAGE_LAYOUT_PREINITIALIZED || _old == VK_IMAGE_LAYOUT_UNDEFINED) && _new == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		image_memory_barrier.srcAccessMask = VK_ACCESS_NONE_KHR;
		image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		src_flags = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dst_flags = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (_old == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _new == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		image_memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		image_memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		src_flags = VK_PIPELINE_STAGE_TRANSFER_BIT;
		dst_flags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		std::stringstream ss;
		ss << "Transfer to non supported layout! (old: " << _old << ", new_ " << _new << ")";
		throw std::invalid_argument(ss.str());
	}

	image_memory_barrier.oldLayout = _old;
	image_memory_barrier.newLayout = _new;
	image_memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	image_memory_barrier.image = this->_image;
	image_memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	image_memory_barrier.subresourceRange.baseMipLevel = 0;
	image_memory_barrier.subresourceRange.levelCount = this->_image_create_info.mipLevels;
	image_memory_barrier.subresourceRange.baseArrayLayer = 0;
	image_memory_barrier.subresourceRange.layerCount = this->_image_create_info.arrayLayers;

	vkCmdPipelineBarrier(command_buffer, src_flags, dst_flags, 0, 0, nullptr, 0, nullptr, 1, &image_memory_barrier);
}

void vka::Texture::generate_mipmaps(VkCommandBuffer command_buffer)
{
	// check if mipmap filter operations are supported
	VkFormatProperties format_properties = {};
	vkGetPhysicalDeviceFormatProperties(this->_physical_device, this->_image_create_info.format, &format_properties);

	if (this->_mipfilter == VK_FILTER_LINEAR && !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
	{
		std::stringstream ss;
		ss << "The current image format (" << this->_image_create_info.format << ") does not support linear filtering on mipmaps.";
		throw std::invalid_argument(ss.str());
	}
	else if (this->_mipfilter == VK_FILTER_CUBIC_IMG && !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG))
	{
		std::stringstream ss;
		ss << "The current image format (" << this->_image_create_info.format << ") does not support cubic filtering on mipmaps.";
		throw std::invalid_argument(ss.str());
	}
	else
	{
		this->_mipfilter = VK_FILTER_NEAREST;
	}

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.pNext = nullptr;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = this->_image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = this->_image_create_info.arrayLayers;

	VkExtent3D mip_extent = this->_image_create_info.extent;
	for (uint32_t i = 1; i < this->_image_create_info.mipLevels; i++)
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		VkImageBlit blit = {};
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = this->_image_create_info.arrayLayers;
		blit.srcOffsets[0] = { 0,0,0 };
		blit.srcOffsets[1] = { static_cast<int32_t>(mip_extent.width), static_cast<int32_t>(mip_extent.height), static_cast<int32_t>(mip_extent.depth) };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = this->_image_create_info.arrayLayers;
		blit.dstOffsets[0] = { 0,0,0 };
		blit.dstOffsets[1] = { static_cast<int32_t>((mip_extent.width  > 1) ? mip_extent.width  / 2 : 1), 
							   static_cast<int32_t>((mip_extent.height > 1) ? mip_extent.height / 2 : 1),
							   static_cast<int32_t>((mip_extent.depth  > 1) ? mip_extent.depth  / 2 : 1) };

		if(mip_extent.width  > 1) mip_extent.width	/= 2;
		if(mip_extent.height > 1) mip_extent.height	/= 2;
		if(mip_extent.depth  > 1) mip_extent.depth	/= 2;

		vkCmdBlitImage(command_buffer, this->_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, this->_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, this->_mipfilter);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}

	// make a layout transition for the last mipmap level
	barrier.subresourceRange.baseMipLevel = this->_image_create_info.mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(command_buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}
