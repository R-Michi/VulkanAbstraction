#pragma once

#ifdef VULKAN_ABSTRACTION_DEBUG
#ifdef _MSVC_VER
#define VULKAN_ASSERT(result)\
if(result != VK_SUCCESS)\
	__debugbreak();
#else
#define VULKAN_ASSERT(result)\
if(result != VK_SUCCESS) \
	asm("int $3")
#endif
#else
#define VULKAN_ASSERT(result)
#endif

#ifdef __clang__  
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
	
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

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
	*	@param reqQueueFamilyFlags Only searches devices which support at least the given queue flags
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
	*	@param reqQueueCount Only searches queues families with at least the given number of queues.
	*/
	struct QueueFamilyFilter
	{
		VkQueueFlags	reqQueueFlags;
		uint32_t		reqQueueCount;
	};

	struct QueueInfo
	{
		uint32_t queueFamilyIndex;
		uint32_t usedQueueCount;
		uint32_t queueBaseIndex;
	};

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

	enum QueueFamilyPriority
	{
		VKA_QUEUE_FAMILY_PRIORITY_FIRST = 0x0,
		VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL = 0x1,
		VKA_QUEUE_FAMILY_PRIORITY_MAX_ENUM = 0x7FFFFFFF
	};

	enum QueueFamilyError : uint32_t
	{
		VKA_QUEUE_FAMILY_ERROR_NONE = 0x00000000,
		VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED = 0x00000001,
		VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED = 0x00000002,
		VKA_QUEUE_FAMILY_ERROR_INVALID_PARAMETER = 0x00000004,
		VKA_QUEUE_FAMILY_ERROR_MAX_ENUM = 0x7FFFFFFF
	};

	class Shader
	{
	private:
		VkDevice				_device;
		char*					_entry_point;
		VkShaderStageFlagBits	_stage;
		VkShaderModule			_module;
		size_t					_size;
		bool					_loaded;

	public:
		Shader(void) noexcept;
		explicit Shader(VkDevice device, const std::string& entry_point = "main") noexcept;

		Shader(const Shader&) = delete;
		Shader& operator= (const Shader&) = delete;

		Shader(Shader&& shader);
		Shader& operator= (Shader&& shader);

		virtual ~Shader(void) noexcept;

		VkResult load(const std::string& path, VkShaderStageFlagBits stage);
		void	 clear(void);

		void set_device(VkDevice device) noexcept;
		void set_entry_point(const std::string& entry_point) noexcept;

		VkShaderModule			get_module(void) const noexcept;
		VkShaderStageFlagBits	get_stage(void) const noexcept;
		const char*				get_entry_point(void) const noexcept;
		size_t					size(void) const noexcept;
	};

	class ShaderProgram
	{
	private:
		std::vector<VkPipelineShaderStageCreateInfo> _shader_stages;
		std::vector<Shader> _shaders;

	public:
		ShaderProgram(void) noexcept;
		explicit ShaderProgram(std::vector<Shader>& shaders);

		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator= (const ShaderProgram&) = delete;

		ShaderProgram(ShaderProgram&&) = delete;
		ShaderProgram& operator= (ShaderProgram&&) = delete;

		virtual ~ShaderProgram(void) noexcept;

		void attach(Shader& shader);
		void clear(void) noexcept;

		uint32_t count(void) const noexcept;
		const VkPipelineShaderStageCreateInfo* get_stages(void) const noexcept;
	};

	class Buffer
	{
	private:
		VkDevice _device;
		VkPhysicalDevice _physical_device;
		VkCommandPool _cmd_pool;
		VkQueue _cmd_queue;
		
		VkBufferCreateInfo _create_info;
		VkBuffer _buffer;
		VkMemoryPropertyFlags _properties;
		VkDeviceMemory _memory;

	public:
		Buffer(void);
		explicit Buffer(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool = VK_NULL_HANDLE, VkQueue queue = VK_NULL_HANDLE);

		Buffer(const Buffer& src);
		Buffer& operator= (const Buffer& src);

		Buffer(Buffer&& src);
		Buffer& operator= (Buffer&& src);

		virtual ~Buffer(void);

		// buffer create info setter
		void set_create_flags(VkBufferCreateFlags flags);
		void set_create_size(VkDeviceSize size);
		void set_create_usage(VkBufferUsageFlags usage);
		void set_create_sharing_mode(VkSharingMode sharing_mode);
		void set_create_queue_families(const uint32_t* indices, uint32_t count);
		void set_create_info(const VkBufferCreateInfo& create_info);

		// memory properties setter
		void set_memory_properties(VkMemoryPropertyFlags properties);

		// set handles for internal usage
		void set_physical_device(VkPhysicalDevice physical_device);
		void set_device(VkDevice device);
		void set_queue(VkQueue queue);
		void set_command_pool(VkCommandPool cmd_pool);

		VkResult	create(void);
		void*		map(VkDeviceSize size, VkDeviceSize offset);
		void		unmap(void);
		size_t		size(void);
		VkBuffer	handle(void);

		Buffer& copy(const Buffer& src);
		Buffer& move(Buffer&& src);
		void	clear(void);
	};

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

		size_t _size;
		size_t _px_count;

		void _default_img_create_info(void);
		void _default_view_create_info(void);
		void _default_sampler_create_info(void);

		VkResult buffer_copy(VkBuffer buffer, VkImage image, uint32_t mip_levels, uint32_t array_layers, VkOffset3D offset, VkExtent3D extent);
		VkResult transform_image_layout(VkImageLayout _old, VkImageLayout _new);

	public:
		Texture(void);
		Texture(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool, VkQueue queue);

		Texture(const Texture&) = delete;
		Texture& operator= (const Texture&) = delete;

		Texture(Texture&&) = delete;
		Texture& operator= (Texture&&) = delete;

		virtual ~Texture(void);

		// image create info setter
		void set_image_flags(VkImageCreateFlags flags);
		void set_image_type(VkImageType type);
		void set_image_format(VkFormat format);
		void set_image_extent(VkExtent3D extent);
		void set_image_mip_levels(uint32_t levels);
		void set_image_array_layers(uint32_t layers);
		void set_image_queue_families(uint32_t queue_family_index);
		void set_image_create_info(const VkImageCreateInfo& create_info);

		// view create info setter
		void set_view_type(VkImageViewType type);
		void set_view_format(VkFormat format);
		void set_view_components(VkComponentMapping component_mapping);
		void set_view_subresource_range(VkImageSubresourceRange subressource_range);
		void set_view_create_info(const VkImageViewCreateInfo& create_info);

		// sampler create info setter
		void set_sampler_mag_filter(VkFilter mag_filter);
		void set_sampler_min_filter(VkFilter min_filter);
		void set_sampler_mipmap_mode(VkSamplerMipmapMode mode);
		void set_sampler_address_mode(VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w);
		void set_sampler_mip_lod_bias(float bias);
		void set_sampler_anisotropy_enable(bool enable);
		void set_sampler_max_anisotropy(float max);
		void set_sampler_compare_enable(bool enable);
		void set_sampler_compare_op(VkCompareOp op);
		void set_sampler_lod(float min_lod, float max_lod);
		void set_sampler_border_color(VkBorderColor border_color);
		void set_sampler_unnormalized_coordinates(bool unnormalized);
		void set_sampler_create_info(const VkSamplerCreateInfo& create_info);

		// set handles for internal usage
		void set_pyhsical_device(VkPhysicalDevice physical_device);
		void set_device(VkDevice device);
		void set_command_pool(VkCommandPool command_pool);
		void set_queue(VkQueue queue);

		VkResult	create(const uint8_t* pdata, size_t pixel_stride);
		void		clear(void);

		VkImageView view(void);
		VkSampler	sampler(void);

		size_t size(void);
		size_t count(void);
	};

	class AttachmentImage
	{
	protected:
		VkPhysicalDevice _physical_device;
		VkDevice _device;

		VkImageCreateInfo _image_create_info;
		VkImageViewCreateInfo _view_create_info;

		VkImage _image;
		VkDeviceMemory _memory;
		VkImageView _view;

		void _default_img_create_info(void);
		void _default_view_create_info(void);

		bool is_image_format_supported(void);
		void get_color_formats(std::vector<VkFormat>& formats);
		void get_depth_formats(std::vector<VkFormat>& formats);
		void get_stencil_formats(std::vector<VkFormat>& formats);
		void get_depth_stencil_formats(std::vector<VkFormat>& formats);
		void throw_unsupported_format(const std::vector<VkFormat>& formats, size_t block_size);

	public:
		AttachmentImage(void);
		AttachmentImage(VkPhysicalDevice physical_device, VkDevice device);
		
		AttachmentImage(const AttachmentImage&) = delete;
		AttachmentImage& operator= (const AttachmentImage&) = delete;

		AttachmentImage(AttachmentImage&&) = delete;
		AttachmentImage& operator= (AttachmentImage&&) = delete;

		virtual ~AttachmentImage(void);

		// image create info setter
		void set_image_format(VkFormat format);
		void set_image_extent(VkExtent2D extent);
		void set_image_samples(VkSampleCountFlagBits samples);
		void set_image_usage(VkImageUsageFlags usage);
		void set_image_queue_family_index(uint32_t index);

		// view create info setter
		void set_view_format(VkFormat format);
		void set_view_components(VkComponentMapping component_mapping);
		void set_view_aspect_mask(VkImageAspectFlags aspect_mask);

		// set internal requiered handles
		void set_physical_device(VkPhysicalDevice physical_device);
		void set_device(VkDevice device);

		VkResult create(void);
		void	 clear(void);

		VkImageView view(void);
	};

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
	PhysicalDeviceError find_suited_physical_device(const std::vector<VkPhysicalDevice>& devices, size_t begin, const PhysicalDeviceFilter& filter, size_t& index);

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
	*	@return @param queue_families A vector of queue family properties.
	*/
	void get_queue_family_properties(const VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queue_families);

	/**
	*	@brief Finds the best suited physical device.
	*	@param queue_families Queue family vector of aviable queue families.
	*	@param begin Index to begin searching for queue families.
	*	@param filter Filter of the queue families.
	*	@param priority Priority for the searching algorithm.
	*	@reurn @param index Index of the queue familiy in the @param queue_families vector. 
	*/
	QueueFamilyError find_suited_queue_family(const std::vector<VkQueueFamilyProperties>& queue_families, size_t begin, const QueueFamilyFilter& filter, QueueFamilyPriority priority, size_t& index);
		
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
};
