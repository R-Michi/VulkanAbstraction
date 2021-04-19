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

namespace vk
{
	namespace abstraction
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
	};
};
