/**
* @file     VulkanApp.h
* @brief    Header file for testing vulkan application.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wempty-body"
    #pragma clang diagnostic ignored "-Wparentheses-equality"
		#include <stb_image.h>
#pragma clang diagnostic pop
#else
	#include <stb_image.h>
#endif

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>

#define VKA_INCLUDE_GLFW
#define VKA_INCLUDE_TINYOBJ
#define VKA_GLFW_ENABLE
#define VKA_MODEL_LOADING_ENABLE
#define VKA_STB_IMAGE_LOAD_ENABLE

#include <vka/vka.h>

class VkaExample
{
	struct UniformTransformMatrices
	{
		glm::mat4 MVP;
	};

	struct QueueInfo
	{
		VkQueue queue;
		uint32_t family_index;
	};

private:
	constexpr static uint32_t SWAPCHAIN_IMAGE_COUNT = 4;
	constexpr static VkFormat SURFACE_COLOR_FORMAT = VK_FORMAT_B8G8R8A8_UNORM;
	constexpr static VkFormat DEPTH_FORMAT = VK_FORMAT_D32_SFLOAT;
	constexpr static VkColorSpaceKHR SURFACE_COLOR_SPACE = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	constexpr static VkImageUsageFlags SURFACE_IMAGE_USAGE = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	constexpr static VkPresentModeKHR PRESENTATION_MODE = VK_PRESENT_MODE_MAILBOX_KHR;

	VkApplicationInfo app_info;
	VkInstance instance;
	VkSurfaceKHR window_surface;

	VkPhysicalDevice physical_device;
	QueueInfo graphics_queue;
	VkDevice device;

	VkPhysicalDeviceProperties pdevice_properties;
	VkPhysicalDeviceMemoryProperties memory_properties;

	VkSwapchainKHR swapchain{ VK_NULL_HANDLE };
	vka::AttachmentImage depth_attachment;
	vka::unique_handle<VkImageView[]> swapchain_image_views;
	std::vector<VkFramebuffer> swapchain_framebuffers;

	VkRenderPass render_pass;
	vka::Shader shaders[2];
	VkPipelineLayout pipeline_layout;
	VkPipeline pipeline;

	VkCommandPool command_pool;
	std::vector<VkCommandBuffer> swapchain_command_buffers;

	uint32_t index_count;
	std::vector<vka::real_t> vertices;
	std::vector<uint32_t> indices;
	vka::Buffer vertex_buffer;
	vka::Buffer index_buffer;
	vka::Buffer uniform_buffer;

	vka::Texture texture;

    VkDescriptorPool dpool;
    vka::DescriptorLayoutArray<1> descriptor_layouts;
	vka::DescriptorSetArray<1> descriptors;

	VkSemaphore sem_img_available[SWAPCHAIN_IMAGE_COUNT];
	VkSemaphore sem_rendering_done[SWAPCHAIN_IMAGE_COUNT];
	VkFence fence_render[SWAPCHAIN_IMAGE_COUNT];

	const GLFWvidmode* vid_mode;
	GLFWwindow* window;
	int width, height;

	std::vector<double> frame_times;

	void glfw_init();
	void glfw_destroy() const;

	// vulkan methods
	void vulkan_init();
	void vulkan_destroy();

	void load_models();

	void make_application_info();
	void create_instance();
	void create_surface();
	void create_physical_device();
	void create_logical_device();

	void create_swapchain();
	void create_depth_attachment();
	void create_render_pass();
	void create_shaders();
	void create_pipeline();
	
	void create_framebuffers();
	void create_command_pool();
	void create_global_command_buffers();
	void create_vertex_buffers();
	void create_index_buffers();
	void create_uniform_buffers();
	void create_textures();
	void create_descriptors();
	void create_semaphores();
	
	void record_command_buffers() const;
	void draw_frame() const;
	void update_frame_contents();

public:
	VkaExample() = default;
	virtual ~VkaExample();

	void init();
	void run();
	void shutdown();
};