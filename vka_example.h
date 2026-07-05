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

#define VKA_INCLUDE_TINYOBJ
#define VKA_MODEL_LOADING_ENABLE
#include <glm/glm.hpp>
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
	constexpr static uint32_t SWAPCHAIN_IMAGE_COUNT = 3;
	constexpr static VkFormat SURFACE_COLOR_FORMAT = VK_FORMAT_B8G8R8A8_UNORM;
	constexpr static VkFormat DEPTH_FORMAT = VK_FORMAT_D32_SFLOAT;
	constexpr static VkColorSpaceKHR SURFACE_COLOR_SPACE = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	constexpr static VkImageUsageFlags SURFACE_IMAGE_USAGE = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	constexpr static VkPresentModeKHR PRESENTATION_MODE = VK_PRESENT_MODE_MAILBOX_KHR;

	VkApplicationInfo app_info;
	VkInstance instance;

	VkPhysicalDevice physical_device;
	QueueInfo graphics_queue;
	VkDevice device;

	VkPhysicalDeviceProperties pdevice_properties;
	VkPhysicalDeviceMemoryProperties memory_properties;

	vka::Window window;
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
    vka::DescriptorLayouts descriptor_layouts;
	vka::DescriptorSets descriptors;

	vka::Renderer renderer;

	static void glfw_init();
	static void glfw_destroy();

	// vulkan methods
	void vulkan_init();
	void vulkan_destroy();

	void load_models();

	void make_application_info();
	void create_instance();
	void create_physical_device();
	void create_logical_device();

	void create_window();
	void create_image_views();
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
	void create_renderer();

	
	void record_command_buffers() const;
	void update_frame_contents();

	void resize_window();

public:
	VkaExample() = default;
	~VkaExample() = default;

	void init();
	void run();
	void shutdown();
};