#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#define VULKAN_ABSTRACTION_DEBUG
#include "include/vulkan_absraction.h"

class VulkanApp
{
	struct vertex_t
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 tex_coords;

		static void binding_description(std::vector<VkVertexInputBindingDescription>& binding_descriptions);
		static void attribute_description(std::vector<VkVertexInputAttributeDescription>& attribute_descriptions);
	};

	struct UniformTranformMatrices
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

	VkSwapchainKHR swapchain{ VK_NULL_HANDLE };
	vka::AttachmentImage depth_attachment;
	std::vector<VkImageView> swapchain_image_views;
	std::vector<VkFramebuffer> swapchain_framebuffers;

	VkRenderPass render_pass;
	vka::ShaderProgram main_shader;
	VkPipelineLayout pipeline_layout;
	VkPipeline pipeline;

	VkCommandPool command_pool;
	std::vector<VkCommandBuffer> swapchain_command_buffers;

	std::vector<vertex_t> vertices;
	vka::Buffer vertex_buffer;
	std::vector<uint32_t> indices;
	vka::Buffer index_buffer;
	vka::Buffer uniform_buffer;

	vka::Texture texture;

	VkDescriptorPool descriptor_pool;
	std::vector<VkDescriptorSetLayout> descriptor_set_layouts;
	std::vector<VkDescriptorSet> descriptor_sets;

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
	constexpr static VkPresentModeKHR PRESENTATION_MODE = VK_PRESENT_MODE_IMMEDIATE_KHR;

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

	void create_descriptor_pool(void);
	void create_descriptor_set_layouts(void);
	void create_descriptor_sets(void);
	void update_descriptor_sets(void);

	void create_semaphores(void);
	
	void record_command_buffers(void);
	void draw_frame(void);
	void update_frame_contents(void);

public:
	VulkanApp(void);
	virtual ~VulkanApp(void);

	void init(void);
	void run(void);
};