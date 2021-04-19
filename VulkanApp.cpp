#define STB_IMAGE_IMPLEMENTATION
#include "VulkanApp.h"
#include <iostream>
#include <string>

VulkanApp::~VulkanApp(void)
{
	this->vulkan_destroy();
	this->glfw_destroy();
}

void VulkanApp::glfw_init(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);	// is only temporary false

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	this->vid_mode = glfwGetVideoMode(monitor);
	window = glfwCreateWindow(this->vid_mode->width / 2, this->vid_mode->height / 2, "", nullptr, nullptr);
	if (window == nullptr)
		throw std::runtime_error("Failed to create GLFW window!");
}

void VulkanApp::glfw_destroy(void)
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void VulkanApp::vulkan_init(void)
{
	this->make_application_info();
	this->create_instance();
	this->create_surface();
	this->create_physical_device();
	this->create_queues();
	this->create_logical_device();

	this->create_swapchain();
}

void VulkanApp::vulkan_destroy(void)
{
	for (VkImageView val : this->swapchain_image_views)
		vkDestroyImageView(this->device, val, nullptr);
	vkDestroySwapchainKHR(this->device, this->swapchain, nullptr);

	vkDestroyDevice(this->device, nullptr);
	vkDestroySurfaceKHR(this->instance, this->window_surface, nullptr);
	vkDestroyInstance(this->instance, nullptr);
}

void VulkanApp::make_application_info(void)
{
	this->app_info = {};
	this->app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	this->app_info.pNext = nullptr;
	this->app_info.pApplicationName = "Vulkan Abstraction";
	this->app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	this->app_info.pEngineName = "";
	this->app_info.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	this->app_info.apiVersion = VK_API_VERSION_1_2;
}

void VulkanApp::create_instance(void)
{
	std::vector<const char*> layers;
#ifdef VULKAN_ABSTRACTION_DEBUG
	layers.push_back("VK_LAYER_LUNARG_standard_validation");
	layers.push_back("VK_LAYER_KHRONOS_validation");
#endif 

	std::vector<const char*> extensions;
	vk::abstraction::get_requiered_glfw_extensions(extensions);
#ifdef VULKAN_ABSTRACTION_DEBUG
	extensions.push_back("VK_EXT_debug_utils");
#endif

	if (!vk::abstraction::are_instance_layers_supported(layers))
	{
		std::string str = "Some requiered instance layers are not supported!\nRequiered instance layers:\n";
		for (const char* l : layers)
			str += '\t' + std::string(l) + '\n';
		throw std::runtime_error(str);
	}

	if (!vk::abstraction::are_instance_extensions_supported(extensions))
	{
		std::string str = "Some requiered instance extensions are not supported!\nRequiered instance extensions:\n";
		for (const char* e : extensions)
			str += '\t' + std::string(e) + '\n';
		throw std::runtime_error(str);
	}

	VkInstanceCreateInfo instance_create_info = {};
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pNext = nullptr;
	instance_create_info.flags = 0;
	instance_create_info.pApplicationInfo = &this->app_info;
	instance_create_info.enabledLayerCount = layers.size();
	instance_create_info.ppEnabledLayerNames = layers.data();
	instance_create_info.enabledExtensionCount = extensions.size();
	instance_create_info.ppEnabledExtensionNames = extensions.data();

	VkResult result = vkCreateInstance(&instance_create_info, nullptr, &this->instance);
	VULKAN_ASSERT(result);
}

void VulkanApp::create_surface(void)
{
	VkResult result = glfwCreateWindowSurface(this->instance, window, nullptr, &this->window_surface);
	VULKAN_ASSERT(result);
}

void VulkanApp::create_physical_device(void)
{
	std::vector<VkPhysicalDevice> physical_devices;
	vk::abstraction::get_physical_devices(this->instance, physical_devices);
	std::vector<VkPhysicalDeviceProperties> device_properties;
	vk::abstraction::get_physical_device_properties(physical_devices, device_properties);

	vk::abstraction::PhysicalDeviceFilter filter = {};
	filter.sequence = nullptr;			// for local VRAM memory				// for staging memory / buffers
	filter.reqMemoryPropertyFlags = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };
	filter.reqDeviceTypeHirachy = { VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU };
	filter.pSurface = &this->window_surface;
	filter.reqMinImageCount = SWAPCHAIN_IMAGE_COUNT;
	filter.reqMaxImageCount = SWAPCHAIN_IMAGE_COUNT;
	filter.reqSurfaceImageUsageFlags = SURFACE_IMAGE_USAGE;
	filter.reqSurfaceColorFormats = { SURFACE_COLOR_FORMAT };
	filter.reqSurfaceColorSpaces = { SURFACE_COLOR_SPACE };
	filter.reqPresentModes = { PRESENTATION_MODE };
	filter.reqQueueFamilyFlags = { VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT };

	size_t idx;
	vk::abstraction::PhysicalDeviceError error = vk::abstraction::find_suited_physical_device(physical_devices, 0, filter, idx);
	if (error != vk::abstraction::VKA_PYHSICAL_DEVICE_ERROR_NONE)
	{
		throw std::runtime_error(vk::abstraction::physical_device_strerror(error));
	}

	this->physical_device = physical_devices.at(idx);
	std::cout << "Successfully found physical device: " << device_properties.at(idx).deviceName << std::endl;
}

void VulkanApp::create_queues(void)
{
	std::vector<VkQueueFamilyProperties> queue_fam_properties;
	vk::abstraction::get_queue_family_properties(this->physical_device, queue_fam_properties);

	vk::abstraction::QueueFamilyFilter queue_fam_filter = {};
	queue_fam_filter.reqQueueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;
	queue_fam_filter.reqQueueCount = 4;

	size_t idx;
	vk::abstraction::QueueFamilyError error = vk::abstraction::find_suited_queue_family(queue_fam_properties, 0, queue_fam_filter, vk::abstraction::VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL, idx);
	if (error != vk::abstraction::VKA_QUEUE_FAMILY_ERROR_NONE)
	{
		throw std::runtime_error(vk::abstraction::queue_family_strerror(error));
	}

	this->graphics_queue_info.queueFamilyIndex = idx;
	this->graphics_queue_info.usedQueueCount = queue_fam_filter.reqQueueCount;
	this->graphics_queue_info.queueBaseIndex = 0;

	if (!vk::abstraction::validate_queue_families(queue_fam_properties, { this->graphics_queue_info }))
	{
		throw std::runtime_error("Validation of queue families failed!");
	}

	std::cout << "Successfully found queue family, index: " << idx << std::endl;
}

void VulkanApp::create_logical_device(void)
{
	std::vector<float> graphics_queue_priorities(this->graphics_queue_info.usedQueueCount, 1.0f);
	VkDeviceQueueCreateInfo queue_create_info = {};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.pNext = nullptr;
	queue_create_info.flags = 0;
	queue_create_info.queueFamilyIndex = this->graphics_queue_info.queueFamilyIndex;
	queue_create_info.queueCount = this->graphics_queue_info.usedQueueCount;
	queue_create_info.pQueuePriorities = graphics_queue_priorities.data();

	std::vector<const char*> device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	
	if (!vk::abstraction::are_device_extensions_supported(this->physical_device, device_extensions))
	{
		std::string str = "Some requiered device extensions are not supported!\nRequiered device extensions:\n";
		for (const char* e : device_extensions)
			str += '\t' + std::string(e) + '\n';
		throw std::runtime_error(str);
	}

	VkDeviceCreateInfo device_create_info = {};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pNext = nullptr;
	device_create_info.flags = 0;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pQueueCreateInfos = &queue_create_info;
	device_create_info.enabledLayerCount = 0;
	device_create_info.ppEnabledLayerNames = nullptr;
	device_create_info.enabledExtensionCount = device_extensions.size();
	device_create_info.ppEnabledExtensionNames = device_extensions.data();
	device_create_info.pEnabledFeatures = nullptr;

	VkResult result = vkCreateDevice(this->physical_device, &device_create_info, nullptr, &this->device);
	VULKAN_ASSERT(result);

	// get queues from device
	this->graphics_queues = new VkQueue[this->graphics_queue_info.usedQueueCount];
	for (uint32_t i = 0; i < this->graphics_queue_info.usedQueueCount; i++)
		vkGetDeviceQueue(this->device, this->graphics_queue_info.queueFamilyIndex, i + this->graphics_queue_info.queueBaseIndex, this->graphics_queues + i);

	// check for durface support
	VkBool32 supported;
	result = vkGetPhysicalDeviceSurfaceSupportKHR(this->physical_device, this->graphics_queue_info.queueFamilyIndex, this->window_surface, &supported);
	VULKAN_ASSERT(result);

	if (!supported)
	{
		throw std::runtime_error("Physical device does not support requiered surface!");
	}
}

void VulkanApp::create_swapchain(void)
{
	int w, h;
	glfwGetWindowSize(this->window, &w, &h);
	if (w == 0)
	{
		w = 1;
		glfwSetWindowSize(this->window, w, h);
	}

	if (h == 0)
	{
		h = 1;
		glfwSetWindowSize(this->window, w, h);
	}

	VkSwapchainKHR old_swapchain = this->swapchain;

	VkSwapchainCreateInfoKHR swapchain_create_info = {};
	swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info.pNext = nullptr;
	swapchain_create_info.flags = 0;
	swapchain_create_info.surface = this->window_surface;
	swapchain_create_info.minImageCount = SWAPCHAIN_IMAGE_COUNT;
	swapchain_create_info.imageFormat = SURFACE_COLOR_FORMAT;
	swapchain_create_info.imageColorSpace = SURFACE_COLOR_SPACE;
	swapchain_create_info.imageExtent = { static_cast<uint32_t>(w), static_cast<uint32_t>(h) };
	swapchain_create_info.imageArrayLayers = 1;
	swapchain_create_info.imageUsage = SURFACE_IMAGE_USAGE;
	swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain_create_info.queueFamilyIndexCount = 1;
	swapchain_create_info.pQueueFamilyIndices = &this->graphics_queue_info.queueFamilyIndex;
	swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_create_info.presentMode = PRESENTATION_MODE;
	swapchain_create_info.clipped = VK_TRUE;
	swapchain_create_info.oldSwapchain = old_swapchain;

	VkResult result = vk::abstraction::setup_swapchain(this->device, swapchain_create_info, this->swapchain, this->swapchain_image_views);
	VULKAN_ASSERT(result);

	vkDestroySwapchainKHR(this->device, old_swapchain, nullptr);
}

void VulkanApp::init(void)
{
	this->glfw_init();
	this->vulkan_init();
}

void VulkanApp::run(void)
{

}