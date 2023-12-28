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
		#include <stb/stb_image.h>
	#pragma clang diagnostic pop
#else
	#include <stb/stb_image.h>
#endif

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#define VKA_DEBUG
#define VKA_INCLUDE_GLFW
#define VKA_INCLUDE_TINYOBJ
#define VKA_GLFW_ENABLE
#define VKA_MODEL_LOADING_ENABLE
#define VKA_STB_IMAGE_LOAD_ENABLE
#include "vka/vka.h"

class VulkanApp
{
	struct UniformTransformMatrices
	{
		glm::mat4 MVP;
	};

private:
	VkApplicationInfo app_info;
	VkInstance instance;
	VkSurfaceKHR window_surface;

	VkPhysicalDevice physical_device;
	vka::QueueInfo graphics_queue_info;
	VkQueue* graphics_queues;
	VkDevice device;

	VkPhysicalDeviceProperties pdevice_properties;
	VkPhysicalDeviceMemoryProperties memory_properties;

	VkSwapchainKHR swapchain{ VK_NULL_HANDLE };
	vka::AttachmentImage depth_attachment;
	std::vector<VkImageView> swapchain_image_views;
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

	VkSemaphore sem_img_aviable;
	VkSemaphore sem_rendering_done;

	const GLFWvidmode* vid_mode;
	GLFWwindow* window;
	int width, height;

	std::vector<double> frame_times;

	constexpr static uint32_t SWAPCHAIN_IMAGE_COUNT = 3;
	constexpr static VkFormat SURFACE_COLOR_FORMAT = VK_FORMAT_B8G8R8A8_UNORM;
	constexpr static VkFormat DEPTH_FORMAT = VK_FORMAT_D32_SFLOAT;
	constexpr static VkColorSpaceKHR SURFACE_COLOR_SPACE = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	constexpr static VkImageUsageFlags SURFACE_IMAGE_USAGE = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	constexpr static VkPresentModeKHR PRESENTATION_MODE = VK_PRESENT_MODE_FIFO_KHR;

	void glfw_init(void);
	void glfw_destroy(void);

	// vulkan methods
	void vulkan_init(void);
	void vulkan_destroy(void);

	void load_models(void);

	void make_application_info(void);
	void create_instance(void);
	void create_surface(void);
	void create_physical_device(void);
	void create_queues(void);
	void create_logical_device(void);

	void create_swapchain(void);
	void create_depth_attachment(void);
	void create_render_pass(void);
	void create_shaders(void);
	void create_pipeline(void);
	
	void create_framebuffers(void);
	void create_command_pool(void);
	void create_global_command_buffers(void);
	void create_vertex_buffers(void);
	void create_index_buffers(void);
	void create_uniform_buffers(void);
	void create_textures(void);
	void create_descriptors(void);
	void create_semaphores(void);
	
	void record_command_buffers(void);
	void draw_frame(void);
	void update_frame_contents(void);

public:
	VulkanApp(void) = default;
	virtual ~VulkanApp(void);

	void init(void);
	void run(void);
	void shutdown(void);
};