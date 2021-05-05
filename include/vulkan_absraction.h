/*********************************************************************************************************************
* 
* Title:    vulkan abstraction
* Author:   R-Michi (github name)
* Date:     05.05.2021
* 
* Libraries:
*   - C++ std library
*   - vulkan SDK https://www.lunarg.com/vulkan-sdk/
*   - glfw3 https://github.com/glfw/glfw
*   - glm https://github.com/g-truc/glm
*   - stb (stb_image.h) https://github.com/nothings/stb
*   - tiny obj loader (tiny_obj_loader.h) https://github.com/tinyobjloader/tinyobjloader/blob/master/tiny_obj_loader.h
* 
* Description:
*   Abstraction library for vulkan.
*
* @version 1.0.0
* @copyright (C) Michael Reim, this software is free to use.
*
* If there are any bugs, or suggestions for improvement write to:
* https://github.com/R-Michi/VulkanAbstraction/issues
* 
*********************************************************************************************************************/

#pragma once

#ifdef VULKAN_ABSTRACTION_DEBUG
	#ifdef _MSVC_VER
		#define VULKAN_ASSERT(result)\
		if(result != VK_SUCCESS)\
			__debugbreak();
	#else
		#define VULKAN_ASSERT(result)\
		if(result != VK_SUCCESS) \
			asm("int $3")	// causes a debug break (sort of)
		#endif
#else
	#define VULKAN_ASSERT(result)
#endif

#ifdef __clang__	// suppress waring "empty body" for clang for include file stb_image.h
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wempty-body"
		#include <stb/stb_image.h>
	#pragma clang diagnostic pop
#else
	#include <stb/stb_image.h>
#endif

#ifndef GLFW_INCLUDE_VULKAN
	#define GLFW_INCLUDE_VULKAN
#endif
	
#include <vector>
#include <string>
#include <unordered_map>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <tiny_obj_loader.h>

namespace vka	
{
	/**
	*	@brief Used to filter out physical devices.
	*	@param sequence Only searches devices which have the given sequence in the name.
	*	@param reqMemoryPropertyFlags Only searches devices with those memory property flags.
	*	@param reqDeviceTypeHirachy Searches devices with the given device type hirachy. The first element has the highest priority.
	*	@param pSurface The surface is requiered to filter our the physical devices.
	*	@param reqMinImageCount Only searches devices which have a swapchain image count equal or smaller than the given value.
	*	@param reqMaxImageCount Only searches devices which have a swapchain image count equal or bigger than the given value.
	*	@param reqSurfaceImageUsageFlags Only searches devices which support those image usage flags on the surface.
	*	@param reqSurfaceColorFormats Only searches devices which support those color formats on the surface.
	*	@param reqSurfaceColorSpaces Only searches devices which support those color spaces on the surface.
	*	@param reqPresentModes Only searches devices which support those presentation modes.
	*	@param reqQueueFamilyFlags Only searches devices which support at least the requiered queue flags.
	*/
	struct PhysicalDeviceFilter
	{
		const char*							sequence;
		std::vector<VkMemoryPropertyFlags>	reqMemoryPropertyFlags;
		std::vector<VkPhysicalDeviceType>	reqDeviceTypeHirachy;
		const VkSurfaceKHR*					pSurface;
		uint32_t							reqMinImageCount;
		uint32_t							reqMaxImageCount;
		VkImageUsageFlags					reqSurfaceImageUsageFlags;
		std::vector<VkFormat>				reqSurfaceColorFormats;
		std::vector<VkColorSpaceKHR>		reqSurfaceColorSpaces;
		std::vector<VkPresentModeKHR>		reqPresentModes;
		std::vector<VkQueueFlags>			reqQueueFamilyFlags;
	};

	/**
	*	@brief Used to filter out queue families.
	*	@param reqQueueFlags Only searches queues families with the requiered queue flags.
	*	@param reqQueueCount Only searches queues families with at least the requiered number of queues.
	*/
	struct QueueFamilyFilter
	{
		VkQueueFlags	reqQueueFlags;
		uint32_t		reqQueueCount;
	};

	/**
	*	@brief Stores information about the queues you are using. This struct must be initialized by yourself.
	*		   Its only purpose is to increase the clarity of the used queues. Additionally its also used by the queue validation.
	*	@param queueFamilyIndex Stores the queue family index of your queues.
	*	@param usedQueueCount Stores how many queues you are using from that one queue family.
	*	@param queueBaseIndex Stores the base index of the used queues. Is useful if you have multiple QueueInfo structs,
	*						  so that the queue indices do not overlap.
	*/
	struct QueueInfo
	{
		uint32_t queueFamilyIndex;
		uint32_t usedQueueCount;
		uint32_t queueBaseIndex;
	};

	/**
	*	@brief All possible errors returned by find_matching_physical_device(...) function.
	*/
	enum PhysicalDeviceError : uint32_t
	{
		VKA_PYHSICAL_DEVICE_ERROR_NONE = 0x00000000,
		VKA_PYHSICAL_DEVICE_ERROR_SEQUENCE_FAILED = 0x00000001,
		VKA_PYHSICAL_DEVICE_ERROR_MEMORY_PROPERTIES_FAILED = 0x00000002,
		VKA_PYHSICAL_DEVICE_ERROR_SURFACE_FAILED = 0x00000004,
		VKA_PYHSICAL_DEVICE_ERROR_MIN_IMAGE_COUNT_FAILED = 0x00000008,
		VKA_PYHSICAL_DEVICE_ERROR_MAX_IMAGE_COUNT_FAILED = 0x00000010,
		VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_USAGE_FAILED = 0x00000020,
		VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_FORMATS_FAILED = 0x00000040,
		VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_SPACE_FAILED = 0x00000080,
		VKA_PYHSICAL_DEVICE_ERROR_PRESENT_MODE_FAILD = 0x00000100,
		VKA_PYHSICAL_DEVICE_ERROR_QUEUE_FLAGS_FAILED = 0x00000200,
		VKA_PYHSICAL_DEVICE_ERROR_DEVICE_TYPE_FAILED = 0x00000400,
		VKA_PYHSICAL_DEVICE_ERROR_INVALID_PARAMETER = 0x0000800,
		VKA_PYHSICAL_DEVICE_ERROR_MAX_ENUM = 0x7FFFFFFF
	};

	/**
	*	@brief Priority enum to search the best matching queue family in the function find_matching_queue_family(...).
	*	VKA_QUEUE_FAMILY_PRIORITY_FIRST searches the first queue family that has the requiered properties given by QueueFamilyFilter struct.
	*	VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL searches the most optimal queue family that has the requiered properties given by QueueFamilyFilter struct.
	*/
	enum QueueFamilyPriority
	{
		VKA_QUEUE_FAMILY_PRIORITY_FIRST = 0x1,
		VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL = 0x2,
		VKA_QUEUE_FAMILY_PRIORITY_MAX_ENUM = 0x7FFFFFFF
	};

	/**
	*	@brief All possible errors returned by find_matching_queue_family(...) function.
	*/
	enum QueueFamilyError : uint32_t
	{
		VKA_QUEUE_FAMILY_ERROR_NONE = 0x00000000,
		VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED = 0x00000001,
		VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED = 0x00000002,
		VKA_QUEUE_FAMILY_ERROR_INVALID_PARAMETER = 0x00000004,
		VKA_QUEUE_FAMILY_ERROR_MAX_ENUM = 0x7FFFFFFF
	};

	/**
	*	@brief This class represents a single shader stage in the rendering pipeline
	*/
	class Shader
	{
	private:
		VkDevice _device;
		char* _entry_point;
		VkShaderStageFlagBits _stage;
		VkShaderModule _module;
		size_t _size;
		bool _loaded;

	public:
		Shader(void) noexcept;

		/**
		*	@param device The logical device that is used for the shader.
		*	@param entry_point Entry point of the shader, main by default.
		*/
		explicit Shader(VkDevice device, const std::string& entry_point = "main") noexcept;

		Shader(const Shader&) = delete;
		Shader& operator= (const Shader&) = delete;

		Shader(Shader&& shader);
		Shader& operator= (Shader&& shader);

		virtual ~Shader(void) noexcept;

		/**
		*	@brief Loads the pre-compiled shader file (file extension: .spv).
		*	@param path Path to the shader file.
		*	@param stage The stage of the shader (vertex, geometry, fragment,...).
		*	@return Vulkan result.
		*/
		VkResult load(const std::string& path, VkShaderStageFlagBits stage);

		/** @brief Clears the shader. The shader can be reloaded again. */
		void clear(void);

		/** @param device The logical device that is used for the shader. */
		void set_device(VkDevice device) noexcept;

		/** @param entry_point Entry point of the shader, main by default. */
		void set_entry_point(const std::string& entry_point) noexcept;

		/** @return The module created module of the sahder. */
		VkShaderModule get_module(void) const noexcept;

		/** @return The stage of that shader. */
		VkShaderStageFlagBits get_stage(void) const noexcept;

		/** @return The entry point of the shader code. */
		const char* get_entry_point(void) const noexcept;

		/** @return The size in bytes of the shader code. */
		size_t size(void) const noexcept;
	};

	/**
	*	@brief All shader stages are linked to a shader program that contains all shader
	*		   stages the rendering pipeline will use.
	*/
	class ShaderProgram
	{
	private:
		std::vector<VkPipelineShaderStageCreateInfo> _shader_stages;
		std::vector<Shader> _shaders;

	public:
		ShaderProgram(void) noexcept;

		/**
		*	@brief Construct or to attach multiple shaders.
		*	@param shaders A std::vector of shaders to attach.
		*/
		explicit ShaderProgram(std::vector<Shader>& shaders);

		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator= (const ShaderProgram&) = delete;

		ShaderProgram(ShaderProgram&&) = delete;
		ShaderProgram& operator= (ShaderProgram&&) = delete;

		virtual ~ShaderProgram(void) noexcept;

		/**
		*	@brief Attaches a shader to the shader program.
		*	@param shader The shader to attach.
		*/
		void attach(Shader& shader);

		/** @brief Cleares the shader program. No shaders are attached anymore after clear. */
		void clear(void) noexcept;

		/** @return The amount of shaders attached to the shader program. */
		uint32_t count(void) const noexcept;

		/** @return An array of VkPipelineShaderStageCreateInfo structs for the rendering pipeline. */
		const VkPipelineShaderStageCreateInfo* get_stages(void) const noexcept;
	};

	/**
	*	@brief Class to create any buffer object to store vertices, indices, uniforms, etc.
	*		   Buffer creation is more easily and straight forward.
	*/
	class Buffer
	{
	private:
		VkDevice _device;
		VkPhysicalDevice _physical_device;
		VkCommandPool _cmd_pool;
		VkQueue	_cmd_queue;
		
		VkBufferCreateInfo _create_info;
		VkBuffer _buffer;
		VkMemoryPropertyFlags _properties;
		VkDeviceMemory _memory;

	public:
		Buffer(void) noexcept;

		/**
		*	@param physical_device The physical device that is used by the buffer.
		*	@param device The logical device that is used by the buffer.
		*	@param cmd_pool The command pool that is used for buffer copy operations.
		*	@param queue The command queue that is used for buffer copy operations.
		*/
		explicit Buffer(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool = VK_NULL_HANDLE, VkQueue queue = VK_NULL_HANDLE) noexcept;

		/** @brief Copy constructor and copy operator. */
		Buffer(const Buffer& src);
		Buffer& operator= (const Buffer& src);

		/** @brief Move constructor and move operator. */
		Buffer(Buffer&& src);
		Buffer& operator= (Buffer&& src);

		virtual ~Buffer(void);

		/** @brief setter for the buffer create info. */
		void set_create_flags(VkBufferCreateFlags flags) noexcept;
		void set_create_size(VkDeviceSize size) noexcept;
		void set_create_usage(VkBufferUsageFlags usage) noexcept;
		void set_create_sharing_mode(VkSharingMode sharing_mode) noexcept;
		void set_create_queue_families(const uint32_t* indices, uint32_t count) noexcept;
		void set_create_info(const VkBufferCreateInfo& create_info) noexcept;

		/** @brief Sets the memory properties of the buffer. */
		void set_memory_properties(VkMemoryPropertyFlags properties) noexcept;

		/** @param physical_device The physical device that is used by the buffer. */
		void set_physical_device(VkPhysicalDevice physical_device) noexcept;

		/** @param device The logical device that is used by the buffer. */
		void set_device(VkDevice device) noexcept;

		/** @param cmd_pool The command pool that is used for buffer copy operations. */
		void set_queue(VkQueue queue) noexcept;

		/** @param queue The command queue that is used for buffer copy operations. */
		void set_command_pool(VkCommandPool cmd_pool) noexcept;

		/**
		*	@brief Creates the buffer.
		*	@return Vulkan result.
		*/
		VkResult create(void);

		/**
		*	@brief Maps the buffer for direct buffer access.
		*	@param size The size of the buffer to map.
		*	@param offset Offset in the buffer memory.
		*	@return A pointer to the buffer memory
		*/
		void* map(VkDeviceSize size, VkDeviceSize offset);

		/** @brief Unampps the buffer again. */
		void unmap(void);

		/** @return The size in bytes of the buffer. */
		size_t size(void) const noexcept;

		/** @return The VkBuffer handle. */
		VkBuffer handle(void) const noexcept;

		/**
		*	@brief Copies a buffer. Buffer copy operation requieres command pool and command queue to be set.
		*		   The buffer create info or the memory properties of the destination buffer can change how the buffer is copied.
		*		   If the create info and the memory properties of the destination buffer contain their default values then the copy operation is a 1:1 copy.
		*		   If the create info or the memory properties of the destination buffer have been modified then the buffer can also copy
		*		   memory into another memory target (e.g. host memory -> device memory or cached memory -> host memory).
		*		   This description counts for the copy operator. The copy constructor is always a 1:1 copy.
		*	@param src The source buffer of the copy operation. The destination buffer is always 'this'.
		*	@return The destination buffer of the copy operatrion ('this').
		*/
		Buffer& copy(const Buffer& src);

		/**
		*	@brief Move is a copy operation followed by a clear of the source buffer.
		*		   After a move the source buffer is not aviable anymore.
		*		   The same description counts for the move operator and the move constructor.
		*	@param src The source buffer of the move operatrion. The destination is always 'this'.
		*	@return The destination buffer of the move operatrion ('this').
		*/
		Buffer& move(Buffer&& src);

		/** @brief Cleares the buffer. The buffer is no longer aviable after a clear. */
		void clear(void);
	};

	/**
	*	@brief Class to create textures more easily and straight forward.
	*/
	class Texture
	{
	private:
		VkPhysicalDevice _physical_device;
		VkDevice _device;
		VkCommandPool _command_pool;
		VkQueue _command_queue;

		VkImageCreateInfo _image_create_info;
		VkImageViewCreateInfo _view_create_info;
		VkSamplerCreateInfo _sampler_create_info;

		VkImage _image;
		VkDeviceMemory _memory;
		VkImageView _view;
		VkSampler _sampler;
		bool _mipmap;
		VkFilter _mipfilter;

		size_t _size;
		size_t _px_count;

		void _default_img_create_info(void) noexcept;
		void _default_view_create_info(void) noexcept;
		void _default_sampler_create_info(void) noexcept;

		/** @brief Begins command buffer recording. */
		VkResult record_commands(VkCommandBuffer& command_buffer);

		/** @brief Executes command buffer.  */
		VkResult execute_commands(VkCommandBuffer command_buffer);

		/** @brief Copies a buffer into an image. */
		void buffer_copy(VkCommandBuffer command_buffer, VkBuffer buffer, VkImage image, uint32_t array_layers, VkExtent3D extent);

		/** @brief Performs an image layout transision. */
		void transform_image_layout(VkCommandBuffer command_buffer, VkImageLayout _old, VkImageLayout _new);

		/** @brief Generates the mipmaps. */
		void generate_mipmaps(VkCommandBuffer command_buffer);

	public:
		Texture(void) noexcept;

		/**
		*	@param physical_device The physical device that is used by the texture.
		*	@param device The logical device that is used by the texture.
		*	@param cmd_pool The command pool that is used by the texture.
		*	@param queue The command queue that is used by the texture.
		*/
		Texture(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool, VkQueue queue) noexcept;

		Texture(const Texture&) = delete;
		Texture& operator= (const Texture&) = delete;

		Texture(Texture&&) = delete;
		Texture& operator= (Texture&&) = delete;

		virtual ~Texture(void);

		/**
		*	@brief Setter for the parameter of the VkImageCreateInfo.
		*/
		void set_image_flags(VkImageCreateFlags flags) noexcept;
		void set_image_type(VkImageType type) noexcept;
		void set_image_format(VkFormat format) noexcept;
		void set_image_extent(VkExtent3D extent) noexcept;
		void set_image_array_layers(uint32_t layers) noexcept;
		void set_image_queue_families(uint32_t queue_family_index) noexcept;
		void set_image_create_info(const VkImageCreateInfo& create_info) noexcept;

		/**
		*	@brief Setter for the parameter of the VkImageViewCreateInfo.
		*/
		void set_view_type(VkImageViewType type) noexcept;
		void set_view_format(VkFormat format) noexcept;
		void set_view_components(VkComponentMapping component_mapping) noexcept;
		void set_view_subresource_range(VkImageSubresourceRange subressource_range) noexcept;
		void set_view_create_info(const VkImageViewCreateInfo& create_info) noexcept;

		/**
		*	@brief Setter for the parameter of the VkSamplerCreateInfo.
		*/
		void set_sampler_mag_filter(VkFilter mag_filter) noexcept;
		void set_sampler_min_filter(VkFilter min_filter) noexcept;
		void set_sampler_mipmap_mode(VkSamplerMipmapMode mode) noexcept;
		void set_sampler_address_mode(VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w) noexcept;
		void set_sampler_mip_lod_bias(float bias) noexcept;
		void set_sampler_anisotropy_enable(bool enable) noexcept;
		void set_sampler_max_anisotropy(float max) noexcept;
		void set_sampler_compare_enable(bool enable) noexcept;
		void set_sampler_compare_op(VkCompareOp op) noexcept;
		void set_sampler_lod(float min_lod, float max_lod) noexcept;
		void set_sampler_border_color(VkBorderColor border_color) noexcept;
		void set_sampler_unnormalized_coordinates(bool unnormalized) noexcept;
		void set_sampler_create_info(const VkSamplerCreateInfo& create_info) noexcept;

		/** @brief Sets wether mipmaps should be generated or not. */
		void mipmap_generate(bool generate) noexcept;

		/** @brief Sets the mipmap filter. */
		void mipmap_filter(VkFilter filter) noexcept;

		/** @param physical_device The physical device that is used by the texture. */
		void set_pyhsical_device(VkPhysicalDevice physical_device) noexcept;

		/** @param device The logical device that is used by the texture. */
		void set_device(VkDevice device) noexcept;

		/** @param cmd_pool The command pool that is used by the texture. */
		void set_command_pool(VkCommandPool command_pool) noexcept;

		/** @param queue The command queue that is used by the texture. */
		void set_queue(VkQueue queue) noexcept;

		/**
		*	@brief Creates the texture object.
		*	@param pdata Pointer to the pixel data.
		*	@param pixel_stride Stride (size) of one pixel.
		*	@return Vulkan result.
		*/
		VkResult create(const void* pdata, size_t pixel_stride);

		/** @brief Cleares the texture. The texture is no longer aviable after a clear. */
		void clear(void);

		/** @return The view handle. */
		VkImageView view(void) const noexcept;

		/** @return The sampler handle. */
		VkSampler sampler(void) const noexcept;

		/** @return Size of the texture in bytes. */
		size_t size(void) const noexcept;

		/** @return Number of pixel within the texture image. */
		size_t count(void) const noexcept;

		/** @return Number of mipmap levels. */
		uint32_t mip_levels(void) const noexcept;

		/** @return Number of array layers. */
		uint32_t array_layers(void) const noexcept;
	};

	/**
	*	@brief Class to create any attachment image more easily and straigt forward.
	*/
	class AttachmentImage
	{
	private:
		VkPhysicalDevice _physical_device;
		VkDevice _device;

		VkImageCreateInfo _image_create_info;
		VkImageViewCreateInfo _view_create_info;

		VkImage _image;
		VkDeviceMemory _memory;
		VkImageView _view;

		void _default_img_create_info(void) noexcept;
		void _default_view_create_info(void) noexcept;

		bool is_image_format_supported(void) const noexcept;
		void get_color_formats(std::vector<VkFormat>& formats) const noexcept;
		void get_depth_formats(std::vector<VkFormat>& formats) const noexcept;
		void get_stencil_formats(std::vector<VkFormat>& formats) const noexcept;
		void get_depth_stencil_formats(std::vector<VkFormat>& formats) const noexcept;
		void throw_unsupported_format(const std::vector<VkFormat>& formats, size_t block_size);

	public:
		AttachmentImage(void) noexcept;

		/**
		*	@param physical_device The physical device that is used by the attachment image.
		*	@param device The logical device that is used by the attachment image.
		*/
		explicit AttachmentImage(VkPhysicalDevice physical_device, VkDevice device) noexcept;
		
		AttachmentImage(const AttachmentImage&) = delete;
		AttachmentImage& operator= (const AttachmentImage&) = delete;

		AttachmentImage(AttachmentImage&&) = delete;
		AttachmentImage& operator= (AttachmentImage&&) = delete;

		virtual ~AttachmentImage(void);

		/**
		*	@brief Setter for the parameter of the VkImageViewCreateInfo.
		*/
		void set_image_format(VkFormat format) noexcept;
		void set_image_extent(VkExtent2D extent) noexcept;
		void set_image_samples(VkSampleCountFlagBits samples) noexcept;
		void set_image_usage(VkImageUsageFlags usage) noexcept;
		void set_image_queue_family_index(uint32_t index) noexcept;

		/**
		*	@brief Setter for the parameter of the VkImageViewCreateInfo.
		*/
		void set_view_format(VkFormat format) noexcept;
		void set_view_components(VkComponentMapping component_mapping) noexcept;
		void set_view_aspect_mask(VkImageAspectFlags aspect_mask) noexcept;

		/** @param physical_device The physical device that is used by the attachment image. */
		void set_physical_device(VkPhysicalDevice physical_device) noexcept;

		/** @param device The logical device that is used by the texture. */
		void set_device(VkDevice device) noexcept;

		/**
		*	@brief Creates the attachment image.
		*	@return Vulkan result.
		*/
		VkResult create(void);

		/** @brief Cleares the attachment image. The attachment image is no longer aviable after a clear. */
		void clear(void);

		/** @return The view handle. */
		VkImageView view(void) const noexcept;
	};

#ifdef VULKAN_ABSTRACTION_EXPERIMENTAL

	/**
	*	This is just an experimental implementation of model loading.
	*	It is not the final version, the implementation can not determine 
	*	at runtime how many components there are for: vertices, texture coordinates
	*	and normal vectors. Additionally the library is restricted to only be 
	*	capable of loading .obj files. The final version will support the
	*	most (common) model formats.
	* 
	*	Now, the tiny object loader library will be used in the background.
	*	Which is, like i mentioned before, only capable of loading
	*	.obj files. For future implementation I will use assimp in the
	*	background, which is a more powerful library.
	*/

	/**
	*	@brief Stores position, texture coordinate and normal vector of a vertex.
	*/
	template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
	struct vertex_t
	{
		static_assert(_vc <= 4U && _vc >= 1, "Vertex component count of vka::Mesh::vertex_t is greater than 4 or less than 1!");
		static_assert(_tc <= 4U && _tc >= 1, "Texture coordinate component count of vka::Mesh::vertex_t is greater than 4 or less than 1!");
		static_assert(_nc <= 4U && _nc >= 1, "Normal vector component count of vka::Mesh::vertex_t is greater than 4 or less than 1!");

		glm::vec<_vc, tinyobj::real_t, glm::qualifier::defaultp> position;
		glm::vec<_tc, tinyobj::real_t, glm::qualifier::defaultp> texcoord;
		glm::vec<_nc, tinyobj::real_t, glm::qualifier::defaultp> normal;

		vertex_t(const glm::vec<_vc, tinyobj::real_t, glm::qualifier::defaultp>& pos		= glm::vec<_vc, tinyobj::real_t, glm::qualifier::defaultp>(0.0f),
				 const glm::vec<_tc, tinyobj::real_t, glm::qualifier::defaultp>& texcoord	= glm::vec<_tc, tinyobj::real_t, glm::qualifier::defaultp>(0.0f),
				 const glm::vec<_nc, tinyobj::real_t, glm::qualifier::defaultp>& normal		= glm::vec<_nc, tinyobj::real_t, glm::qualifier::defaultp>(0.0f))
		{
			this->position	= pos;
			this->texcoord	= texcoord;
			this->normal	= normal;
		}

		vertex_t(const vertex_t& other) { *this = other; }
		vertex_t& operator= (const vertex_t& other)
		{
			this->position	= other.position;
			this->texcoord	= other.texcoord;
			this->normal	= other.normal;
			return *this;
		}

		vertex_t(vertex_t&& other)		{ *this = (vertex_t&&)other; }
		vertex_t& operator= (vertex_t&& other)
		{
			this->position	= other.position;
			this->texcoord	= other.texcoord;
			this->normal	= other.normal;

			other.position	= glm::vec<_vc, tinyobj::real_t, glm::qualifier::defaultp>(0.0f);
			other.texcoord	= glm::vec<_tc, tinyobj::real_t, glm::qualifier::defaultp>(0.0f);
			other.normal	= glm::vec<_nc, tinyobj::real_t, glm::qualifier::defaultp>(0.0f);
			return *this;
		}

		// used for std::unordered_map
		bool operator== (const vertex_t& other) const noexcept
		{
			//return (this->position == other.position && this->texcoord == other.texcoord && this->normal == other.normal);
			return (memcmp(this, &other, sizeof(*this)) == 0);
		}

		// used for std::unordered_map
		struct hasher
		{
			size_t operator()(const vertex_t& other) const noexcept
			{
				return ((std::hash<glm::vec<_vc, tinyobj::real_t, glm::qualifier::defaultp>>()(other.position) ^
						(std::hash<glm::vec<_tc, tinyobj::real_t, glm::qualifier::defaultp>>()(other.texcoord) << 1)) >> 1) ^
						(std::hash<glm::vec<_nc, tinyobj::real_t, glm::qualifier::defaultp>>()(other.normal)   << 1);
			}
		};
	};

	/**
	*	@brief Contains the data for rendering: vertices, indices and an ID
	*	to the corresponding materials.
	*/
	template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
	class Mesh
	{
	private:
		std::vector<vertex_t<_vc, _tc, _nc> > _vertices;	// vertices of mesh
		std::vector<uint32_t> _indices;						// indices of mesh
		std::vector<int> _materials;						// corresponding material ID's
		std::string _name;									// name of mesh

	public:
		Mesh(void) = default;	// not needed

		Mesh(const Mesh& other) { *this = other; }
		Mesh& operator= (const Mesh& other)
		{
			this->_vertices		= other._vertices;
			this->_indices		= other._indices;
			this->_materials	= other._materials;
			this->_name			= other._name;
			return *this;
		}

		Mesh(Mesh&& other) { *this = (Mesh&&)other; }
		Mesh& operator= (Mesh&& other)
		{
			this->_vertices		= other._vertices;
			this->_indices		= other._indices;
			this->_materials	= other._materials;
			this->_name			= other._name;

			this->clear();
			return *this;
		}

		virtual ~Mesh(void) {}

		/**
		*	@brief Creates and initializes the mesh.
		*	@param attribute Attribute type from tiny object loader of current model.
		*	@param shape Sha�e type from tiny object loader of current model.
		*/
		void create(const tinyobj::attrib_t& attribute, const tinyobj::shape_t& shape)
		{
			this->_name = shape.name;
			this->_materials = shape.mesh.material_ids;

			// Hash map to store only unique vertices and their corresponding index.
			// It's only purpose is to deduplicate vertices (to figure out if a vertex already exists or not).
			// Uses a hash map to have direct access to the vertex as this is faster than using a tree or list map.
			std::unordered_map<vertex_t<_vc, _tc, _nc>, uint32_t, typename vertex_t<_vc, _tc, _nc>::hasher> map;
			for (const tinyobj::index_t index : shape.mesh.indices)
			{
				vertex_t<_vc, _tc, _nc> vertex;
				tinyobj::real_t* pos		= (tinyobj::real_t*)&vertex.position;
				tinyobj::real_t* texcoord	= (tinyobj::real_t*)&vertex.texcoord;
				tinyobj::real_t* normal		= (tinyobj::real_t*)&vertex.normal;

				// read positions
				for (uint32_t i = 0; i < _vc; i++)
				{
					if (attribute.vertices.size() > 0)
						pos[i] = attribute.vertices[_vc * index.vertex_index + i];
				}

				// read texture coordinates
				for (uint32_t i = 0; i < _tc; i++)
				{
					if (attribute.texcoords.size() > 0)
						texcoord[i] = attribute.texcoords[_tc * index.texcoord_index + i];
				}

				// read normal vectors
				for (uint32_t i = 0; i < _nc; i++)
				{
					if (attribute.normals.size() > 0)
						normal[i] = attribute.normals[_nc * index.normal_index + i];
				}
				
				// only store vertex if it doesnt exist in the hash map
				if (map.count(vertex) == 0)
				{
					// the current size of the _vertices vector is the index of the current UNIQUE vertex
					map[vertex] = static_cast<uint32_t>(_vertices.size());
					// ... also store the UNIQUE vertex
					this->_vertices.push_back(vertex);
				}
				// store index of the current vertex
				this->_indices.push_back(map[vertex]);
			}	
		}

		/** @return a vector of vertices. */
		const std::vector< vertex_t<_vc, _tc, _nc> >& vertices(void) const noexcept { return this->_vertices; }

		/** @return a vector of indices to address the vertices. */
		const std::vector<uint32_t>& indices(void) const noexcept					{ return this->_indices; }

		/** @return a vector of the corresponding material ID's for the current mesh. */
		const std::vector<int>& material_ids(void) const noexcept					{ return this->_materials; }

		/** @return the name of the mesh.  */
		const std::string& name(void) const noexcept								{ return this->_name; }

		/** @return a pointer to the vertex data.  */
		const void* vertex_data(void) const noexcept								{ return this->_vertices.data(); }

		/** @return a pointer to the index data. */
		const void* index_data(void) const noexcept									{ return this->_indices.data(); }

		/** @return the size in bytes of all vertices.  */
		size_t vertex_size(void) const noexcept										{ return this->_vertices.size() * sizeof(vertex_t<_vc, _tc, _nc>); }

		/** @return the size in bytes of all indices. */
		size_t index_size(void) const noexcept										{ return this->_indices.size() * sizeof(uint32_t); }

		/** @brief Cleares the mesh / cleares all internal vectors. */
		void clear(void) noexcept
		{
			this->_vertices.clear();
			this->_indices.clear();
			this->_materials.clear();
			this->_name.clear();

			this->_vertices.shrink_to_fit();
			this->_indices.shrink_to_fit();
			this->_materials.shrink_to_fit();
		}
	};

	/**
	*	@brief Contains one or more meshes and materials.
	*/
	template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
	class Model
	{
	private:
		std::vector< Mesh<_vc, _tc, _nc> > _meshes;		// all meshes of the model
		std::vector<tinyobj::material_t> _materials;	// all materials of the model

	public:
		Model(void) = default;
		virtual ~Model(void) {}

		Model(const Model& other) { *this = other; }
		Model& operator= (const Model& other)
		{
			this->_meshes		= other._meshes;
			this->_materials	= other._materials;
		}

		Model(Model&& other) { *this = (Model&&)other; }
		Model& operator= (Model&& other)
		{
			this->_meshes		= other._meshes;
			this->_materials	= other._materials;
			this->clear();
		}

		/**
		*	@brief Loads the model into the memory.
		*	@param path Path to the model file.
		*/
		void load(const std::string& path)
		{
			tinyobj::ObjReaderConfig config = {};
			config.vertex_color = false;
			config.triangulate = true;	// always triangulate faces

			tinyobj::ObjReader obj_file;
			if (!obj_file.ParseFromFile(path, config))
				throw std::invalid_argument("Failed to load object file \"" + path + "\".");

			for (const tinyobj::shape_t& shape : obj_file.GetShapes())
			{
				Mesh<_vc, _tc, _nc> mesh;
				mesh.create(obj_file.GetAttrib(), shape);
				this->_meshes.push_back(mesh);
			}
			this->_materials = obj_file.GetMaterials();
		}

		/**
		*	@brief Combines vertices and indices of all meshes into a single vertex and index vector.
		*	NOTE: Materials for different meshes will become useless!
		*	@param[out] vertices Resulting vertices vector.
		*	@param[out] indices Resulting indices vector.
		*/
		void combine(std::vector< vertex_t<_vc, _tc, _nc> >& vertices, std::vector<uint32_t>& indices)
		{
			vertices.clear();
			indices.clear();

			uint32_t base_index = 0;
			for (const Mesh<_vc, _tc, _nc>& cur_mesh : this->_meshes)
			{
				for (const vertex_t<_vc, _tc, _nc>&v : cur_mesh.vertices())
					vertices.push_back(v);
				for (uint32_t i : cur_mesh.indices())
					indices.push_back(base_index + i);
				base_index += cur_mesh.vertices().size();
			}
		}

		/** @return a vector of all meshes. */
		const std::vector< Mesh<_vc, _tc, _nc> >& meshes(void) const noexcept { return this->_meshes; }

		/** @return a vector of all materials. */
		const std::vector<tinyobj::material_t> materials(void) const noexcept { return this->_materials; }

		/** @brief Cleares the model / cleares all internal vectors. */
		void clear(void) noexcept
		{
			this->_meshes.clear();
			this->_materials.clear();
			this->_meshes.shrink_to_fit();
			this->_materials.shrink_to_fit();
		}
	};

	// typical definitions
	typedef vertex_t<3, 2, 3>	vertex323_t;
	typedef vertex_t<4, 2, 3>	vertex423_t;
	typedef vertex_t<3, 3, 3>	vertex333_t;
	typedef vertex_t<4, 3, 3>	vertex433_t;

	typedef Mesh<3, 2, 3>		Mesh323;
	typedef Mesh<4, 2, 3>		Mesh423;
	typedef Mesh<3, 3, 3>		Mesh333;
	typedef Mesh<4, 3, 3>		Mesh433;

	typedef Model<3, 2, 3>		Model323;
	typedef Model<4, 2, 3>		Model423;
	typedef Model<3, 3, 3>		Model333;
	typedef Model<4, 3, 3>		Model433;
#endif

	/**
	*	@brief Checks if one layer at instance level is supported.
	*	@param layer_name The name of the layer.
	*	@param _property Returns the properties of the layer if supported (optional).
	*	@return 'true' if layer is supported, 'false' if layer is not supported.
	*/
	bool is_instance_layer_supported(const char* layer_name, VkLayerProperties* _property = nullptr);

	/**
	*	@brief Checks if multiple layers at instance level are supported.
	*	@param layer_names The layer's names.
	*	@param _properties Returns the properties of the layers if supported (optional).
	*	@return 'true' if all layers are supported, 'false' if one or more layers are not supported.
	*/
	bool are_instance_layers_supported(std::vector<const char*> layer_names, std::vector<VkLayerProperties>* _properties = nullptr);

	/**
	*	@brief Checks if one extension at instance level is supported.
	*	@param extension_name The name of the extension.
	*	@param _property Returns the properties of the extension if supported (optional).
	*	@return 'true' if extension is supported, 'false' if extension is not supported.
	*/
	bool is_instance_extension_supported(const char* extension_name, VkExtensionProperties* _property = nullptr);

	/**
	*	@brief Checks if multiple extensions at instance level are supported.
	*	@param extension_names The extension's names.
	*	@param _properties Returns the properties of the extensions if supported (optional).
	*	@return 'true' if all extensions are supported, 'false' if one or more extensions are not supported.
	*/
	bool are_instance_extensions_supported(std::vector<const char*> extension_names, std::vector<VkExtensionProperties>* _properties = nullptr);

	/**
	*	@return @param glfw_extensions All requiered extensions requiered for GLFW.
	*/
	void get_requiered_glfw_extensions(std::vector<const char*>& glfw_extensions);

	/**
	*	@brief Gives all physical devices of a given instance.
	*	@param instance Vulkan instance.
	*	@return @param devices All physical devices of the given instance.
	*/
	void get_physical_devices(const VkInstance& instance, std::vector<VkPhysicalDevice>& devices);

	/**
	*	@brief Gives the properties of multiple devices at once.
	*	@param device The physical devices to read the properties from.
	*	@return @param properties The properties of the given physical devices.
	*/
	void get_physical_device_properties(const std::vector<VkPhysicalDevice>& devices, std::vector<VkPhysicalDeviceProperties>& properties);

	/**
	*	@brief Finds the best suited physical device.
	*	@param devices Vector of aviable physical devices.
	*	@param begin Index to begin searching for devices.
	*	@param filter Filter for the physical device.
	*	@return @param index Index of the device in the @param devices vector.
	*	@return Error if no physical device was found.
	*/
	PhysicalDeviceError find_matching_physical_device(const std::vector<VkPhysicalDevice>& devices, size_t begin, const PhysicalDeviceFilter& filter, size_t& index);

	/**
	*	@brief Converts FindPhysicalDeviceError enum to string.
	*	@param error The error Code.
	*	@return The string of the error code.
	*/
	const char* physical_device_strerror(PhysicalDeviceError error);

	/**
	*	@brief Checks if one layer at instance level is supported.
	*	@param device Device that belongs to the layer.
	*	@param layer_name The name of the layer.
	*	@param _property Returns the properties of the layer if supported (optional).
	*	@return 'true' if layer is supported, 'false' if layer is not supported.
	*/
	bool is_device_layer_supported(VkPhysicalDevice device, const char* layer_name, VkLayerProperties* _property = nullptr);

	/**
	*	@brief Checks if multiple layers at instance level are supported.
	*	@param device Device that belongs to the layers.
	*	@param layer_names The layer's names.
	*	@param _properties Returns the properties of the layers if supported (optional).
	*	@return 'true' if all layers are supported, 'false' if one or more layers are not supported.
	*/
	bool are_device_layers_supported(VkPhysicalDevice device, std::vector<const char*> layer_names, std::vector<VkLayerProperties>* _properties = nullptr);

	/**
	*	@brief Checks if one extension at instance level is supported.
	*	@param device Device that belongs to the extension.
	*	@param extension_name The name of the extension.
	*	@param _property Returns the properties of the extension if supported (optional).
	*	@return 'true' if extension is supported, 'false' if extension is not supported.
	*/
	bool is_device_extension_supported(VkPhysicalDevice device, const char* extension_name, VkExtensionProperties* _property = nullptr);

	/**
	*	@brief Checks if multiple extensions at instance level are supported.
	*	@param device Device that belongs to the extensions.
	*	@param extension_names The extension's names.
	*	@param _properties Returns the properties of the extensions if supported (optional).
	*	@return 'true' if all extensions are supported, 'false' if one or more extensions are not supported.
	*/
	bool are_device_extensions_supported(VkPhysicalDevice device, std::vector<const char*> extension_names, std::vector<VkExtensionProperties>* _properties = nullptr);

	/**
	*	@brief Returns all queue families of a physical device.
	*	@param device The physical device to read the queue families from.
	*	@param queue_families A std::vector where the queue family properties will be stored.
	*/
	void get_queue_family_properties(const VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queue_families);

	/**
	*	@brief Finds the best suited physical device.
	*	@param queue_families Queue family vector of aviable queue families.
	*	@param begin Index to begin searching for queue families.
	*	@param filter Filter of the queue families.
	*	@param priority Priority for the searching algorithm.
	*	@param index Value where the found queue family index will be stored.
	*/
	QueueFamilyError find_matching_queue_family(const std::vector<VkQueueFamilyProperties>& queue_families, size_t begin, const QueueFamilyFilter& filter, QueueFamilyPriority priority, size_t& index);
		
	/**
	*	@brief Converts QueueFamiliyError enum to string.
	*	@param error The error Code.
	*	@return The string of the error code.
	*/
	const char* queue_family_strerror(QueueFamilyError error);

	/**
	*	@brief Checks if the used number of queues does not exceed the maximum queue count for that family.
	*	@param queue_families Aviable queue families, MUST contain all!
	*	@param queue_infos The queue infos that sould be checked.
	*	@return 'true' if the validation was successful, 'false' if validation was not successful.
	*/
	bool validate_queue_families(const std::vector<VkQueueFamilyProperties>& queue_families, const std::vector<QueueInfo>& queue_infos);

	/**
	*	@brief Creates the swapchain and the corresponding image views.
	*	@param device Device used for the swapchain.
	*	@param create_info The create info struct for the swapchain.
	*	@return @param swapchain The created swapchain.
	*	@return @param image_views The created image views of the swapchain.
	*	@return Vulkan result.
	*/
	VkResult setup_swapchain(VkDevice device, const VkSwapchainCreateInfoKHR& create_info, VkSwapchainKHR& swapchain, std::vector<VkImageView>& image_views);

	namespace utility
	{
		/**
		*	@brief Finds the memory type index of a given memoryTypeBits bit mask. Used for VkMemoryRequierements struct.
		*	@param physical_device The physical device where the operation should happen.
		*	@param bits The memoryTypeBits bit mask from VkMemoryRequierements struct.
		*	@param req_flags The memory property flags to search the index for.
		*	@return The memory type index of the @param req_flags.
		*	@return Maximum value of uint32_t (4294967296) if no index could be found.
		*/
		uint32_t find_memory_type_index(VkPhysicalDevice physical_device, uint32_t bits, VkMemoryPropertyFlags req_flags);

		/**
		*	@brief Checks if a format is supported for a given tiling and format feature.
		*	@param physical_device The physical device to read the format properties from.
		*	@param format The format to check if it is supported.
		*	@param tiling The tiling where the format should be checked if it is aviable.
		*	@param format_featute The format feature where the format should be checked if it is aviable.
		*	@return 'true' if it is supported and 'false' if it is not supported.
		*/
		bool image_format_supported(VkPhysicalDevice physical_device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags format_feature);

		/**
		*	@brief Returnes all supported formats for either color, depth, stencil or depth/stencil combined.
		*	@param formats A std::vector where the formats should be stored.
		*/
		void get_color_formats(std::vector<VkFormat>& formats);
		void get_depth_formats(std::vector<VkFormat>& formats);
		void get_stencil_formats(std::vector<VkFormat>& formats);
		void get_depth_stencil_formats(std::vector<VkFormat>& formats);

		/**
		*	@brief Returns all supported formats for a given tiling and format feature (color, depth, stencil or depth/stencil combined).
		*	@param physical_device The physical device to read the format properties from.
		*	@param tiling The tiling where the formats should be read.
		*	@param format_feature The format feature where the formats should be read.
		*	@param formats A std::vector where the formats should be stored.
		*/
		void get_supported_color_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);
		void get_supported_depth_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);
		void get_supported_stencil_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);
		void get_supported_depth_stencil_formats(VkPhysicalDevice physical_device, VkImageTiling tiling, VkFormatFeatureFlags format_feature, std::vector<VkFormat>& formats);

		/**
		*	Converts a format feature flag bit to a image usage flag bit.
		*	@param format_feature The format feature flag bit that should be converted.
		*	@return The image usage flag bit.
		*/
		VkImageUsageFlags format_feature_to_image_usage(VkFormatFeatureFlags format_feature);

		/**
		*	Converts a image usage flag bit to a format feature flag bit.
		*	@param image_usage The image usage flag bit that should be converted.
		*	@return The format feature flag bit.
		*/
		VkFormatFeatureFlags image_usage_to_format_feature(VkImageUsageFlags image_usage);
	};
}
