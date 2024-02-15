/**
* @file     VulkanApp.cpp
* @brief    Implementation of the testing vulkan application.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "VulkanApp.h"
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

VulkanApp::~VulkanApp(void)
{
	double res = 0.0f;
	for (double d : this->frame_times)
		res += d;
	res /= (double)this->frame_times.size();
	std::cout << "AVG MSPT: " << res * 1000.0 << std::endl;
	std::cout << "AVG FPS: " << 1.0 / res << std::endl;
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

	glfwGetWindowSize(this->window, &this->width, &this->height);
	glfwSetWindowPos(window, this->vid_mode->width / 4, this->vid_mode->height / 4);
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
	this->create_render_pass();
	this->create_depth_attachment();
	this->create_shaders();
	this->create_framebuffers();
	this->create_command_pool();
	this->create_global_command_buffers();
	this->create_vertex_buffers();
	this->create_index_buffers();
	this->create_uniform_buffers();
	this->create_textures();
	this->create_descriptors();

	this->create_pipeline();
	this->create_semaphores();
	
	this->record_command_buffers();
}

void VulkanApp::vulkan_destroy(void)
{
	vkDeviceWaitIdle(this->device);

	vkDestroySemaphore(this->device, this->sem_img_aviable, nullptr);
	vkDestroySemaphore(this->device, this->sem_rendering_done, nullptr);

	this->texture.destroy();
	this->uniform_buffer.destroy();
	this->index_buffer.destroy();
	this->vertex_buffer.destroy();
	vkFreeCommandBuffers(this->device, this->command_pool, this->swapchain_command_buffers.size(), this->swapchain_command_buffers.data());
	vkDestroyCommandPool(this->device, this->command_pool, nullptr);

	for (VkFramebuffer fbo : this->swapchain_framebuffers)
		vkDestroyFramebuffer(this->device, fbo, nullptr);

	vkDestroyPipelineLayout(this->device, this->pipeline_layout, nullptr);
	vkDestroyPipeline(this->device, this->pipeline, nullptr);

	this->shaders[0].destroy();
	this->shaders[1].destroy();
	this->descriptors.destroy();
    this->descriptor_layouts.destroy();
    vkDestroyDescriptorPool(this->device, this->dpool, nullptr);

	vkDestroyRenderPass(this->device, this->render_pass, nullptr);

	this->depth_attachment.destroy();
	for (VkImageView val : this->swapchain_image_views)
		vkDestroyImageView(this->device, val, nullptr);
	vkDestroySwapchainKHR(this->device, this->swapchain, nullptr);

	delete[] this->graphics_queues;
	vkDestroyDevice(this->device, nullptr);
	vkDestroySurfaceKHR(this->instance, this->window_surface, nullptr);
	vkDestroyInstance(this->instance, nullptr);
}


void VulkanApp::load_models(void)
{
	vka::Model model;
	model.load("../../../assets/models/test.obj", vka::VKA_MODEL_LOAD_OPTION_IGNORE_MATERIAL);

	const std::vector<vka::VertexAttribute> merge_attribs = {
		{ vka::VKA_VERTEX_ATTRIBUTE_TYPE_POSITION, 0 },
		{ vka::VKA_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATE, 0 },
		{ vka::VKA_VERTEX_ATTRIBUTE_TYPE_NORMAL, 0 }	
	};
	
	uint32_t base_index = 0;
	for(const vka::Mesh& mesh : model.meshes())
	{
		mesh.merge(this->vertices, merge_attribs);
		for(uint32_t i : mesh.indices())
			this->indices.push_back(base_index + i);
		base_index += mesh.vertex_count();
		std::cout << "Mesh material ID count: " << mesh.materials().size() << std::endl;
		std::cout << "Mesh triangle count:    " << mesh.primitive_count() << std::endl;
	}
    const uint32_t idx_count = this->indices.size();
    const uint32_t vertex_count = base_index;

	std::cout << "Number if meshes: " << model.meshes().size() << std::endl;
	std::cout << "Number of materials: " << model.materials().size() << std::endl;
	std::cout << "Number of vertices: " << vertex_count << std::endl;
	std::cout << "Number of indices: " << idx_count << std::endl;
}


void VulkanApp::make_application_info(void)
{
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
	std::vector<std::string> layers;
#ifdef VKA_DEBUG
	layers.emplace_back("VK_LAYER_KHRONOS_validation");
#endif 
	layers.emplace_back("VK_LAYER_LUNARG_monitor");

	std::vector<std::string> extensions;
	vka::instance::get_glfw_extensions(extensions);
#ifdef VKA_DEBUG
	//extensions.push_back("VK_EXT_debug_utils");
#endif

	size_t idx;
	if ((idx = vka::instance::supports_layers(layers)) != vka::NPOS)
	{
		std::string str = "Instance layer \"";
		str += std::string(layers.at(idx));
		str += "\" is not supported.";
		throw std::runtime_error(str);
	}

	if ((idx = vka::instance::supports_extensions(extensions)) != vka::NPOS)
	{
		std::string str = "Instance extension \"";
		str += std::string(extensions.at(idx));
		str += "\" is not supported.";
		throw std::runtime_error(str);
	}

	const char* _layers[layers.size()], *_extensions[extensions.size()];
	vka::common::cvt_stdstr2ccpv(layers, _layers);
	vka::common::cvt_stdstr2ccpv(extensions, _extensions);

	VkInstanceCreateInfo instance_create_info;
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pNext = nullptr;
	instance_create_info.flags = 0;
	instance_create_info.pApplicationInfo = &this->app_info;
	instance_create_info.enabledLayerCount = layers.size();
	instance_create_info.ppEnabledLayerNames = _layers;
	instance_create_info.enabledExtensionCount = extensions.size();
	instance_create_info.ppEnabledExtensionNames = _extensions;

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
	vka::device::get(this->instance, physical_devices);

	vka::PhysicalDeviceFilter filter;
	filter.sequence = nullptr;			// for local VRAM memory				// for staging memory / buffers
	filter.memoryPropertyFlags = { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
									  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
									  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT};
	filter.deviceTypeHirachy = { VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU };
	filter.queueFamilyFlags = { VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT };
	filter.surfaceSupport = true;

	size_t idx = vka::device::find(this->instance, physical_devices, filter, &this->pdevice_properties, &this->memory_properties);
	if (idx == vka::NPOS)
		throw std::runtime_error("Failed to find physical device");

	this->physical_device = physical_devices.at(idx);

	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(this->physical_device, &properties);
	std::cout << "Successfully found physical device: " << properties.deviceName << std::endl;
}

void VulkanApp::create_queues(void)
{
	std::vector<VkQueueFamilyProperties> queue_fam_properties;
	vka::queue::properties(this->physical_device, queue_fam_properties);

	vka::QueueFamilyFilter queue_fam_filter = {};
	queue_fam_filter.queueFlags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;
	queue_fam_filter.queueCount = 4;

	size_t idx = vka::queue::find(queue_fam_properties, queue_fam_filter, vka::VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL);
	if(idx == vka::NPOS)
	{
		throw std::runtime_error("Failed to find queue family.");
	}

	this->graphics_queue_info.queueFamilyIndex = idx;
	this->graphics_queue_info.usedQueueCount = queue_fam_filter.queueCount;
	this->graphics_queue_info.queueOffset = 0;

	if (!vka::queue::validate(queue_fam_properties, { this->graphics_queue_info }))
		throw std::runtime_error("Validation of queue families failed!");

	std::cout << "Successfully found queue family, index: " << idx << std::endl;
}

void VulkanApp::create_logical_device(void)
{
	std::vector<float> graphics_queue_priorities(this->graphics_queue_info.usedQueueCount, 1.0f);
	VkDeviceQueueCreateInfo queue_create_info;
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.pNext = nullptr;
	queue_create_info.flags = 0;
	queue_create_info.queueFamilyIndex = this->graphics_queue_info.queueFamilyIndex;
	queue_create_info.queueCount = this->graphics_queue_info.usedQueueCount;
	queue_create_info.pQueuePriorities = graphics_queue_priorities.data();

	std::vector<std::string> device_extensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	
	size_t idx;
	if ((idx = vka::device::supports_extensions(this->physical_device, device_extensions)) != vka::NPOS)
	{
		std::string str = "Device extension \"";
		str += std::string(device_extensions.at(idx));
		str += "\" is not supported.";
		throw std::runtime_error(str);
	}

	const char* _extensions[device_extensions.size()];
	vka::common::cvt_stdstr2ccpv(device_extensions, _extensions);

	VkDeviceCreateInfo device_create_info;
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pNext = nullptr;
	device_create_info.flags = 0;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pQueueCreateInfos = &queue_create_info;
	device_create_info.enabledLayerCount = 0;
	device_create_info.ppEnabledLayerNames = nullptr;
	device_create_info.enabledExtensionCount = device_extensions.size();
	device_create_info.ppEnabledExtensionNames = _extensions;
	device_create_info.pEnabledFeatures = nullptr;

	VkResult result = vkCreateDevice(this->physical_device, &device_create_info, nullptr, &this->device);
	VULKAN_ASSERT(result);

	// get queues from device
	this->graphics_queues = new VkQueue[this->graphics_queue_info.usedQueueCount];
	for (uint32_t i = 0; i < this->graphics_queue_info.usedQueueCount; i++)
		vkGetDeviceQueue(this->device, this->graphics_queue_info.queueFamilyIndex, i + this->graphics_queue_info.queueOffset, this->graphics_queues + i);

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
	VkSwapchainKHR old_swapchain = this->swapchain;

	VkSwapchainCreateInfoKHR swapchain_create_info;
	swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info.pNext = nullptr;
	swapchain_create_info.flags = 0;
	swapchain_create_info.surface = this->window_surface;
	swapchain_create_info.minImageCount = SWAPCHAIN_IMAGE_COUNT;
	swapchain_create_info.imageFormat = SURFACE_COLOR_FORMAT;
	swapchain_create_info.imageColorSpace = SURFACE_COLOR_SPACE;
	swapchain_create_info.imageExtent = { static_cast<uint32_t>(this->width), static_cast<uint32_t>(this->height) };
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

	this->swapchain_image_views.resize(swapchain_create_info.minImageCount);
	vka::swapchain::setup(this->device, swapchain_create_info, this->swapchain, this->swapchain_image_views.data());
	vkDestroySwapchainKHR(this->device, old_swapchain, nullptr);
}

void VulkanApp::create_depth_attachment(void)
{
	VkComponentMapping component_mapping = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
	const vka::AttachmentImageCreateInfo ci = {
		.imageFormat = DEPTH_FORMAT,
		.imageExtent = { static_cast<uint32_t>(this->width), static_cast<uint32_t>(this->height) },
		.imageSamples = VK_SAMPLE_COUNT_1_BIT,
		.imageUsage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.imageQueueFamilyIndexCount = 1,
		.imageQueueFamilyIndices = &this->graphics_queue_info.queueFamilyIndex,
		.viewFormat = DEPTH_FORMAT,
		.viewComponentMapping = component_mapping,
		.viewAspectMask = VK_IMAGE_ASPECT_DEPTH_BIT
	};
	this->depth_attachment.create(this->device, this->physical_device, this->memory_properties, ci);
}

void VulkanApp::create_render_pass(void)
{
	constexpr static size_t N_ATTACHMENTS = 2;

	std::vector<VkAttachmentDescription> attachment_descriptions(N_ATTACHMENTS);
	// color attachment description
	attachment_descriptions[0].flags = 0;
	attachment_descriptions[0].format = SURFACE_COLOR_FORMAT;
	attachment_descriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachment_descriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment_descriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachment_descriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment_descriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment_descriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment_descriptions[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// depth attachment description
	attachment_descriptions[1].flags = 0;
	attachment_descriptions[1].format = DEPTH_FORMAT;
	attachment_descriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
	attachment_descriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment_descriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment_descriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment_descriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment_descriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachment_descriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	std::vector<VkAttachmentReference> attachment_references(N_ATTACHMENTS);
	// color attachment reference
	attachment_references[0].attachment = 0;	
	attachment_references[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// depth attachment reference
	attachment_references[1].attachment = 1;
	attachment_references[1].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription main_pass;
	main_pass.flags = 0;
	main_pass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	main_pass.inputAttachmentCount = 0;
	main_pass.pInputAttachments = nullptr;
	main_pass.colorAttachmentCount = 1;
	main_pass.pColorAttachments = attachment_references.data() + 0;
	main_pass.pResolveAttachments = nullptr;
	main_pass.pDepthStencilAttachment = attachment_references.data() + 1;
	main_pass.preserveAttachmentCount = 0;
	main_pass.pPreserveAttachments = nullptr;

	VkSubpassDependency main_pass_dependency;
	main_pass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	main_pass_dependency.dstSubpass = 0;
	main_pass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	main_pass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	main_pass_dependency.srcAccessMask = 0;
	main_pass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	main_pass_dependency.dependencyFlags = 0;

	VkRenderPassCreateInfo render_pass_create_info;
	render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_create_info.pNext = nullptr;
	render_pass_create_info.flags = 0;
	render_pass_create_info.attachmentCount = attachment_descriptions.size();
	render_pass_create_info.pAttachments = attachment_descriptions.data();
	render_pass_create_info.subpassCount = 1;
	render_pass_create_info.pSubpasses = &main_pass;
	render_pass_create_info.dependencyCount = 1;
	render_pass_create_info.pDependencies = &main_pass_dependency;

	VkResult result = vkCreateRenderPass(this->device, &render_pass_create_info, nullptr, &this->render_pass);
	VULKAN_ASSERT(result);
}

void VulkanApp::create_shaders(void)
{
	shaders[0].create(this->device, "../../../assets/shaders/bin/main.vert.spv");
	shaders[1].create(this->device, "../../../assets/shaders/bin/main.frag.spv");
}

void VulkanApp::create_pipeline(void)
{
	std::vector<VkVertexInputBindingDescription> bindings(1);
	bindings[0].binding = 0;
	bindings[0].stride = 8 * sizeof(vka::real_t);
	bindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	std::vector<VkVertexInputAttributeDescription> attributes(3);
	attributes[0].location = 0;
	attributes[0].binding = 0;
	attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributes[0].offset = 0;

	attributes[1].location = 1;
	attributes[1].binding = 0;
	attributes[1].format = VK_FORMAT_R32G32_SFLOAT;
	attributes[1].offset = 3 * sizeof(vka::real_t);

	attributes[2].location = 2;
	attributes[2].binding = 0;
	attributes[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributes[2].offset = 5 * sizeof(vka::real_t);

	VkPipelineVertexInputStateCreateInfo vertex_input_create_info;
	vertex_input_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_create_info.pNext = nullptr;
	vertex_input_create_info.flags = 0;
	vertex_input_create_info.vertexBindingDescriptionCount = bindings.size();
	vertex_input_create_info.pVertexBindingDescriptions = bindings.data();
	vertex_input_create_info.vertexAttributeDescriptionCount = attributes.size();
	vertex_input_create_info.pVertexAttributeDescriptions = attributes.data();
	
	VkViewport view_port;
	view_port.x = 0.0f;
	view_port.y = 0.0f;
	view_port.width = static_cast<float>(this->width);
	view_port.height = static_cast<float>(this->height);
	view_port.minDepth = 0.0f;
	view_port.maxDepth = 1.0f;

	VkRect2D scissor;
	scissor.offset = { 0, 0 };
	scissor.extent = { static_cast<uint32_t>(this->width), static_cast<uint32_t>(this->height) };

	VkPipelineViewportStateCreateInfo view_port_create_info;
	view_port_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	view_port_create_info.pNext = nullptr;
	view_port_create_info.flags = 0;
	view_port_create_info.viewportCount = 1;
	view_port_create_info.pViewports = &view_port;
	view_port_create_info.scissorCount = 1;
	view_port_create_info.pScissors = &scissor;

	VkPipelineInputAssemblyStateCreateInfo input_assambly_create_info;
	input_assambly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assambly_create_info.pNext = nullptr;
	input_assambly_create_info.flags = 0;
	input_assambly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assambly_create_info.primitiveRestartEnable = VK_FALSE;

	VkPipelineRasterizationStateCreateInfo rasterization_create_info;
	rasterization_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterization_create_info.pNext = nullptr;
	rasterization_create_info.flags = 0;
	rasterization_create_info.depthClampEnable = VK_FALSE;
	rasterization_create_info.rasterizerDiscardEnable = VK_FALSE;
	rasterization_create_info.polygonMode = VK_POLYGON_MODE_FILL;
	rasterization_create_info.cullMode = VK_CULL_MODE_NONE;
	rasterization_create_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterization_create_info.depthBiasEnable = VK_FALSE;
	rasterization_create_info.depthBiasConstantFactor = 0.0f;
	rasterization_create_info.depthBiasClamp = 0;
	rasterization_create_info.depthBiasSlopeFactor = 0.0f;
	rasterization_create_info.lineWidth = 1.0f;

	VkPipelineMultisampleStateCreateInfo multisample_create_info;
	multisample_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisample_create_info.pNext = nullptr;
	multisample_create_info.flags = 0;
	multisample_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisample_create_info.sampleShadingEnable = VK_FALSE;
	multisample_create_info.minSampleShading = 1.0f;
	multisample_create_info.pSampleMask = nullptr;
	multisample_create_info.alphaToCoverageEnable = VK_FALSE;
	multisample_create_info.alphaToOneEnable = VK_FALSE;

	VkPipelineDepthStencilStateCreateInfo depth_test_create_info;
	depth_test_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth_test_create_info.pNext = nullptr;
	depth_test_create_info.flags = 0;
	depth_test_create_info.depthTestEnable = VK_TRUE;
	depth_test_create_info.depthWriteEnable = VK_TRUE;
	depth_test_create_info.depthCompareOp = VK_COMPARE_OP_LESS;
	depth_test_create_info.depthBoundsTestEnable = VK_FALSE;
	depth_test_create_info.stencilTestEnable = VK_FALSE;
	depth_test_create_info.front = {};
	depth_test_create_info.back = {};
	depth_test_create_info.minDepthBounds = 0.0f;
	depth_test_create_info.maxDepthBounds = 1.0f;

	VkPipelineColorBlendAttachmentState color_blend_attachment;
	color_blend_attachment.blendEnable = VK_TRUE;
	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;
	color_blend_attachment.colorWriteMask = 0x0000000F;

	VkPipelineColorBlendStateCreateInfo color_blend_create_info;
	color_blend_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blend_create_info.pNext = nullptr;
	color_blend_create_info.flags = 0;
	color_blend_create_info.logicOpEnable = VK_FALSE;
	color_blend_create_info.logicOp = VK_LOGIC_OP_NO_OP;
	color_blend_create_info.attachmentCount = 1;
	color_blend_create_info.pAttachments = &color_blend_attachment;
	color_blend_create_info.blendConstants[0] = 0.0f;
	color_blend_create_info.blendConstants[0] = 0.0f;
	color_blend_create_info.blendConstants[0] = 0.0f;
	color_blend_create_info.blendConstants[0] = 0.0f;

	std::vector<VkDynamicState> dynamic_states = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamic_state_create_info;
	dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state_create_info.pNext = nullptr;
	dynamic_state_create_info.flags = 0;
	dynamic_state_create_info.dynamicStateCount = dynamic_states.size();
	dynamic_state_create_info.pDynamicStates = dynamic_states.data();

	VkPipelineLayoutCreateInfo layout_create_info;
	layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layout_create_info.pNext = nullptr;
	layout_create_info.flags = 0;
	layout_create_info.setLayoutCount = this->descriptor_layouts.count(),
	layout_create_info.pSetLayouts = this->descriptor_layouts.layouts(),
	layout_create_info.pushConstantRangeCount = 0;
	layout_create_info.pPushConstantRanges = nullptr;

	VkResult result = vkCreatePipelineLayout(this->device, &layout_create_info, nullptr, &this->pipeline_layout);
	VULKAN_ASSERT(result);

	const VkPipelineShaderStageCreateInfo shader_stages[2] = {
		this->shaders[0].make_stage(VK_SHADER_STAGE_VERTEX_BIT),
		this->shaders[1].make_stage(VK_SHADER_STAGE_FRAGMENT_BIT)
	};

	VkGraphicsPipelineCreateInfo pipeline_create_info;
	pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_create_info.pNext = nullptr;
	pipeline_create_info.flags = 0;
	pipeline_create_info.stageCount = 2;
	pipeline_create_info.pStages = shader_stages;
	pipeline_create_info.pVertexInputState = &vertex_input_create_info;
	pipeline_create_info.pInputAssemblyState = &input_assambly_create_info;
	pipeline_create_info.pTessellationState = nullptr;
	pipeline_create_info.pViewportState = &view_port_create_info;
	pipeline_create_info.pRasterizationState = &rasterization_create_info;
	pipeline_create_info.pMultisampleState = &multisample_create_info;
	pipeline_create_info.pDepthStencilState = &depth_test_create_info;
	pipeline_create_info.pColorBlendState = &color_blend_create_info;
	pipeline_create_info.pDynamicState = &dynamic_state_create_info;
	pipeline_create_info.layout = pipeline_layout;
	pipeline_create_info.renderPass = this->render_pass;
	pipeline_create_info.subpass = 0;
	pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
	pipeline_create_info.basePipelineIndex = -1;

	result = vkCreateGraphicsPipelines(this->device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &this->pipeline);
	VULKAN_ASSERT(result);
}


void VulkanApp::create_framebuffers(void)
{
	for (VkImageView view : this->swapchain_image_views)
	{
		VkImageView attachments[2] = {
			view,
			this->depth_attachment.view()
		};

		VkFramebufferCreateInfo fbo_creare_info = {};
		fbo_creare_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbo_creare_info.pNext = nullptr;
		fbo_creare_info.flags = 0;
		fbo_creare_info.renderPass = this->render_pass;
		fbo_creare_info.attachmentCount = 2;
		fbo_creare_info.pAttachments = attachments;
		fbo_creare_info.width = static_cast<uint32_t>(this->width);
		fbo_creare_info.height = static_cast<uint32_t>(this->height);
		fbo_creare_info.layers = 1;

		VkFramebuffer fbo;
		VkResult result = vkCreateFramebuffer(this->device, &fbo_creare_info, nullptr, &fbo);
		VULKAN_ASSERT(result);

		this->swapchain_framebuffers.push_back(fbo);
	}
}

void VulkanApp::create_command_pool(void)
{
	VkCommandPoolCreateInfo command_pool_create_info;
	command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_create_info.pNext = nullptr;
	command_pool_create_info.flags = 0;
	command_pool_create_info.queueFamilyIndex = this->graphics_queue_info.queueFamilyIndex;

	VkResult result = vkCreateCommandPool(this->device, &command_pool_create_info, nullptr, &this->command_pool);
	VULKAN_ASSERT(result);
}

void VulkanApp::create_global_command_buffers(void)
{
	this->swapchain_command_buffers.resize(this->swapchain_framebuffers.size());

	VkCommandBufferAllocateInfo cbo_alloc_info;
	cbo_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbo_alloc_info.pNext = nullptr;
	cbo_alloc_info.commandPool = this->command_pool;
	cbo_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cbo_alloc_info.commandBufferCount = this->swapchain_command_buffers.size();

	VkResult result = vkAllocateCommandBuffers(this->device, &cbo_alloc_info, this->swapchain_command_buffers.data());
	VULKAN_ASSERT(result);
}

void VulkanApp::create_vertex_buffers(void)
{
	VkDeviceSize size = sizeof(vka::real_t) * this->vertices.size();
	vka::BufferCreateInfo create_info = {
		.bufferFlags = 0,
		.bufferSize = size,
		.bufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.bufferSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.bufferQueueFamilyIndexCount = 1,
		.bufferQueueFamilyIndices = &this->graphics_queue_info.queueFamilyIndex,
		.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

    vka::Buffer staging_buffer(this->device, this->memory_properties, create_info);

	void* buff = staging_buffer.map(0, size);
	memcpy(buff, this->vertices.data(), size);
	staging_buffer.unmap();
	this->vertices.clear();
	this->vertices.shrink_to_fit();

	create_info.bufferUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	create_info.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	this->vertex_buffer.create(this->device, this->memory_properties, create_info);

    vka::CommandBufferOTS cbo(this->device, this->command_pool);
    this->vertex_buffer.copy(cbo.handle(), staging_buffer);
    cbo.end_wait(this->graphics_queues[0]);
}

void VulkanApp::create_index_buffers(void)
{
	VkDeviceSize size = this->indices.size() * sizeof(uint32_t);
	vka::BufferCreateInfo create_info = {
		.bufferFlags = 0,
		.bufferSize = size,
		.bufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.bufferSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.bufferQueueFamilyIndexCount = 1,
		.bufferQueueFamilyIndices = &this->graphics_queue_info.queueFamilyIndex,
		.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};
	
	vka::Buffer staging_buffer(this->device, this->memory_properties, create_info);

	void* buff = staging_buffer.map(0, size);
	memcpy(buff, this->indices.data(), size);
	staging_buffer.unmap();
	this->index_count = this->indices.size();
	this->indices.clear();
	this->indices.shrink_to_fit();

	create_info.bufferUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	create_info.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	this->index_buffer.create(this->device, this->memory_properties, create_info);

    vka::CommandBufferOTS cbo(this->device, this->command_pool);
    this->index_buffer.copy(cbo.handle(), staging_buffer);
    cbo.end_wait(this->graphics_queues[0]);
}

void VulkanApp::create_uniform_buffers(void)
{
	const vka::BufferCreateInfo create_info = {
		.bufferFlags = 0,
		.bufferSize = sizeof(UniformTransformMatrices),
		.bufferUsage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		.bufferSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.bufferQueueFamilyIndexCount = 1,
		.bufferQueueFamilyIndices = &this->graphics_queue_info.queueFamilyIndex,
		.memoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT
	};
	this->uniform_buffer.create(this->device, this->memory_properties, create_info);
}

void VulkanApp::create_textures(void)
{
	VkExtent3D size;
	const void* const data[2] = {
		vka::Texture::load_image<vka::VKA_IMAGE_DATA_TYPE_INT8, 4>("../../../assets/textures/texture.png", size),
		vka::Texture::load_image<vka::VKA_IMAGE_DATA_TYPE_INT8, 4>("../../../assets/textures/texture2.jpeg", size)
	};

	const vka::TextureCreateInfo create_info = {
		.imageFlags = 0,
		.imageType = VK_IMAGE_TYPE_2D,
		.imageFormat = VK_FORMAT_R8G8B8A8_UNORM,
		.imageExtent = size,
		.imageArrayLayers = 2,
		.imageQueueFamilyIndexCount = 1,
		.imageQueueFamilyIndices = &this->graphics_queue_info.queueFamilyIndex,
		.samplerMagFilter = VK_FILTER_NEAREST,
		.samplerMinFilter = VK_FILTER_NEAREST,
		.samplerMipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.samplerAddressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.samplerAddressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.samplerAddressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		.samplerLodBias = 0.0f,
		.samplerAnisotropyEnable = VK_FALSE,
		.samplerMaxAnisotropy = 0.0f,
		.samplerCompareEnable = VK_FALSE,
		.samplerCompareOp = VK_COMPARE_OP_ALWAYS,
		.samplerMinLod = 0.0f,
		.samplerMaxLod = (float)vka::Texture::level_count(size) - 1,	// max LOD = highest mip-level index = mip-level count - 1
		.samplerBorderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK,
		.samplerUnnormalizedCoordinates = VK_FALSE,
		.generateMipMap = true
	};

	vka::TextureViewCreateInfo view = {
		.flags = 0,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = VK_FORMAT_R8G8B8A8_UNORM,
		.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY },
		.baseArrayLayer = 0,
		.layerCount = 1
	};

	this->texture.create(this->device, this->memory_properties, create_info);

	this->texture.create_view(view);
	view.baseArrayLayer = 1;
	this->texture.create_view(view);

	vka::Buffer staging_buffer;
	this->texture.load_staging(data, staging_buffer, this->memory_properties, this->graphics_queue_info.queueFamilyIndex, 2);

    vka::CommandBufferOTS cbo(this->device, this->command_pool);
    this->texture.load(cbo.handle(), staging_buffer, 0, 2);
    this->texture.finish(cbo.handle(), VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
    cbo.end_wait(this->graphics_queues[0]);
}

void VulkanApp::create_descriptors(void)
{
    const VkDescriptorPoolSize sizes[2] = {
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 }
    };

    const VkDescriptorPoolCreateInfo ci = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = vka::DescriptorSetArray<1>::POOL_FLAGS,
        .maxSets = 1,
        .poolSizeCount = 2,
        .pPoolSizes = sizes
    };
    VkResult result = vkCreateDescriptorPool(this->device, &ci, nullptr, &this->dpool);
    VULKAN_ASSERT(result);

    vka::DescriptorSetBindingList<1> bindings;
    bindings.push(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
    bindings.push(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);

    this->descriptor_layouts.create(this->device, bindings, 0);
    this->descriptors.create(this->dpool, this->descriptor_layouts);

    const VkDescriptorBufferInfo buffer_info = vka::descriptor::make_buffer_info(this->uniform_buffer);
    const VkDescriptorImageInfo image_info = vka::descriptor::make_image_info(this->texture, 1);
    vka::DescriptorSetArray<1>::UpdateOperation<2> update = this->descriptors.op_update<2>();
    update.write(0, 0, 0, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &image_info);
    update.write(0, 1, 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &buffer_info);
    update.execute();
}

void VulkanApp::create_semaphores(void)
{
	VkSemaphoreCreateInfo semaphore_create_info;
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphore_create_info.pNext = nullptr;
	semaphore_create_info.flags = 0;

	VkResult result = vkCreateSemaphore(this->device, &semaphore_create_info, nullptr, &this->sem_img_aviable);
	VULKAN_ASSERT(result);
	result = vkCreateSemaphore(this->device, &semaphore_create_info, nullptr, &this->sem_rendering_done);
	VULKAN_ASSERT(result);
}


void VulkanApp::record_command_buffers(void)
{
	VkCommandBufferBeginInfo command_buffer_begin_info;
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.pNext = nullptr;
	command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	command_buffer_begin_info.pInheritanceInfo = nullptr;

	for (size_t i = 0; i < this->swapchain_framebuffers.size(); i++)
	{
		VkResult result = vkBeginCommandBuffer(this->swapchain_command_buffers.at(i), &command_buffer_begin_info);
		VULKAN_ASSERT(result);

		VkRenderPassBeginInfo render_pass_begin_info;
		render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_begin_info.pNext = nullptr;
		render_pass_begin_info.renderPass = this->render_pass;
		render_pass_begin_info.framebuffer = this->swapchain_framebuffers.at(i);
		render_pass_begin_info.renderArea.offset = { 0, 0 };
		render_pass_begin_info.renderArea.extent = { static_cast<uint32_t>(this->width), static_cast<uint32_t>(this->height) };

		std::vector<VkClearValue> clear_values(2);
		clear_values[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };	// clear color
		clear_values[1].depthStencil.depth = 1.0f;

		render_pass_begin_info.clearValueCount = clear_values.size();
		render_pass_begin_info.pClearValues = clear_values.data();

		vkCmdBeginRenderPass(this->swapchain_command_buffers.at(i), &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(this->swapchain_command_buffers.at(i), VK_PIPELINE_BIND_POINT_GRAPHICS, this->pipeline);

		VkViewport view_port;
		view_port.x = 0;
		view_port.y = 0;
		view_port.width = static_cast<float>(this->width);
		view_port.height = static_cast<float>(this->height);
		view_port.minDepth = 0.0f;
		view_port.maxDepth = 1.0f;
		vkCmdSetViewport(this->swapchain_command_buffers.at(i), 0, 1, &view_port);

		VkRect2D scissor;
		scissor.offset = { 0, 0 };
		scissor.extent = { static_cast<uint32_t>(this->width), static_cast<uint32_t>(this->height) };
		vkCmdSetScissor(this->swapchain_command_buffers.at(i), 0, 1, &scissor);

		// bind buffers
		VkDeviceSize offset = 0;
		VkBuffer vertex_buffer_handle = this->vertex_buffer.handle();
		vkCmdBindVertexBuffers(this->swapchain_command_buffers.at(i), 0, 1, &vertex_buffer_handle, &offset);

		VkBuffer index_buffer_handle = this->index_buffer.handle();
		vkCmdBindIndexBuffer(this->swapchain_command_buffers.at(i), index_buffer_handle, offset, VK_INDEX_TYPE_UINT32);

		// bind descriptor sets
        this->descriptors.bind(this->swapchain_command_buffers.at(i), VK_PIPELINE_BIND_POINT_GRAPHICS, this->pipeline_layout);

		vkCmdDrawIndexed(this->swapchain_command_buffers.at(i), this->index_count, 1, 0, 0, 0);

		vkCmdEndRenderPass(this->swapchain_command_buffers.at(i));

		result = vkEndCommandBuffer(this->swapchain_command_buffers.at(i));
		VULKAN_ASSERT(result);
	}
}

void VulkanApp::init(void)
{
	this->load_models();
	this->glfw_init();
	this->vulkan_init();
}

void VulkanApp::draw_frame(void)
{
	uint32_t img_index;
	vkAcquireNextImageKHR(this->device, this->swapchain, ~(0UI64), this->sem_img_aviable, VK_NULL_HANDLE, &img_index);

	VkSubmitInfo submit_info;
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pNext = nullptr;
	submit_info.waitSemaphoreCount = 1;
	submit_info.pWaitSemaphores = &this->sem_img_aviable;
	VkPipelineStageFlags wait_stage_mask[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submit_info.pWaitDstStageMask = wait_stage_mask;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &this->swapchain_command_buffers.at(img_index);
	submit_info.signalSemaphoreCount = 1;					  
	submit_info.pSignalSemaphores = &this->sem_rendering_done;

	VkResult result = vkQueueSubmit(this->graphics_queues[0], 1, &submit_info, VK_NULL_HANDLE);
	VULKAN_ASSERT(result);

	VkPresentInfoKHR present_info;
	present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.pNext = nullptr;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores = &this->sem_rendering_done;
	present_info.swapchainCount = 1;
	present_info.pSwapchains = &this->swapchain;
	present_info.pImageIndices = &img_index;
	present_info.pResults = nullptr;

	result = vkQueuePresentKHR(this->graphics_queues[0], &present_info);
	VULKAN_ASSERT(result);
}

void VulkanApp::update_frame_contents(void)
{
	UniformTransformMatrices utm = {};

	glm::mat4 model(1.0f);
	model = glm::rotate(model, static_cast<float>(M_PI * 0.1f * glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 2.0f, -3.5f), glm::vec3(0.0f, 0.7f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), static_cast<float>(this->width) / static_cast<float>(this->height), 0.001f, 1000.0f);
	projection[1][1] *= -1.0f;
	utm.MVP = projection * view * model;

	void* _map = this->uniform_buffer.map(0, sizeof(UniformTransformMatrices));
	memcpy(_map, &utm, sizeof(UniformTransformMatrices));
	this->uniform_buffer.unmap();
}

void VulkanApp::run(void)
{
	while (!glfwGetKey(this->window, GLFW_KEY_ESCAPE) && !glfwWindowShouldClose(this->window))
	{
		double t0_render = glfwGetTime();

		glfwPollEvents();

		this->update_frame_contents();
		this->draw_frame();

		double t_render = glfwGetTime() - t0_render;

		std::cout << "\r" << 1.0 / t_render << "FPS                    ";
		this->frame_times.push_back(t_render);
	}
	std::cout << std::endl;
}

void VulkanApp::shutdown(void)
{
	this->vulkan_destroy();
	this->glfw_destroy();
}
