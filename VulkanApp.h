#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#define VULKAN_ABSTRACTION_DEBUG
#include "include/vulkan_absraction.h"

class VulkanApp
{
private:
	VkApplicationInfo app_info;
	VkInstance instance;
	VkSurfaceKHR window_surface;
	VkPhysicalDevice physical_device;
	vk::abstraction::QueueInfo graphics_queue_info;
	VkQueue* graphics_queues;
	VkDevice device;
	VkSwapchainKHR swapchain{ VK_NULL_HANDLE };
	std::vector<VkImageView> swapchain_image_views;

	const GLFWvidmode* vid_mode;
	GLFWwindow* window;

	constexpr static uint32_t SWAPCHAIN_IMAGE_COUNT = 3;
	constexpr static VkFormat SURFACE_COLOR_FORMAT = VK_FORMAT_B8G8R8A8_UNORM;
	constexpr static VkColorSpaceKHR SURFACE_COLOR_SPACE = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	constexpr static VkImageUsageFlags SURFACE_IMAGE_USAGE = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	constexpr static VkPresentModeKHR PRESENTATION_MODE = VK_PRESENT_MODE_FIFO_KHR;

	void glfw_init(void);
	void glfw_destroy(void);

	// vulkan methods
	void vulkan_init(void);
	void vulkan_destroy(void);
	void make_application_info(void);
	void create_instance(void);
	void create_surface(void);
	void create_physical_device(void);
	void create_queues(void);
	void create_logical_device(void);
	void create_swapchain(void);
	void create_swapchain_images(void);

public:
	VulkanApp(void) = default;
	virtual ~VulkanApp(void);

	void init(void);
	void run(void);
};