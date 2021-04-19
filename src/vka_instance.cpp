#include "../include/vulkan_absraction.h"
#include <string.h>
#include <stdexcept>

bool vk::abstraction::is_instance_layer_supported(const char* layer_name, VkLayerProperties* _property)
{
	uint32_t n_layers;
	VkResult res = vkEnumerateInstanceLayerProperties(&n_layers, nullptr);	// get number of layers
	if (res != VK_SUCCESS) return false;

	VkLayerProperties properties[n_layers];
	res = vkEnumerateInstanceLayerProperties(&n_layers, properties);		// get layer properties
	if (res != VK_SUCCESS) return false;

	for (uint32_t i = 0; i < n_layers; i++)
	{
		if (strcmp(layer_name, properties[i].layerName) == 0)
		{
			if(_property != nullptr)
				*_property = properties[i];
			return true;
		}
	}
	
	return false;
}

bool vk::abstraction::are_instance_layers_supported(std::vector<const char*> layer_names, std::vector<VkLayerProperties>* _properties)
{
	uint32_t n_layers;
	VkResult res = vkEnumerateInstanceLayerProperties(&n_layers, nullptr);	// get number of layers
	if (res != VK_SUCCESS) return false;

	VkLayerProperties properties[n_layers];
	res = vkEnumerateInstanceLayerProperties(&n_layers, properties);		// get layer properties
	if (res != VK_SUCCESS) return false;

	for (size_t i = 0; i < layer_names.size(); i++)
	{
		bool contains = false;
		for (uint32_t j = 0; j < n_layers; j++)
		{
			if (strcmp(layer_names[i], properties[j].layerName) == 0)	// check if current layer is supported
			{
				contains = true;
				if (_properties != nullptr)
					_properties->push_back(properties[j]);				// add, if layer is contained
			}
		}

		if (!contains)
		{
			if(_properties != nullptr)
				_properties->clear();	// if one layer is not contained, clear property vector
			return false;
		}
	}

	return true;
}

bool vk::abstraction::is_instance_extension_supported(const char* extension_name, VkExtensionProperties* _property)
{
	uint32_t n_extensions;
	VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, nullptr);	// get number of extensions
	if (res != VK_SUCCESS) return false;

	VkExtensionProperties properties[n_extensions];
	res = vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, properties);		// get extension properties
	if (res != VK_SUCCESS) return false;

	for (uint32_t i = 0; i < n_extensions; i++)
	{
		if (strcmp(extension_name, properties[i].extensionName) == 0)
		{
			if (_property != nullptr)
				*_property = properties[i];
			return true;
		}
	}

	return false;
}

bool vk::abstraction::are_instance_extensions_supported(std::vector<const char*> extension_names, std::vector<VkExtensionProperties>* _properties)
{
	uint32_t n_extensions;
	VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, nullptr);	// get number of layers
	if (res != VK_SUCCESS) return false;

	VkExtensionProperties properties[n_extensions];
	res = vkEnumerateInstanceExtensionProperties(nullptr, &n_extensions, properties);		// get layer properties
	if (res != VK_SUCCESS) return false;

	for (size_t i = 0; i < extension_names.size(); i++)
	{
		bool contains = false;
		for (uint32_t j = 0; j < n_extensions; j++)
		{
			if (strcmp(extension_names[i], properties[j].extensionName) == 0)	// check if current layer is supported
			{
				contains = true;
				if (_properties != nullptr)
					_properties->push_back(properties[j]);				// add, if layer is contained
			}
		}

		if (!contains)
		{
			if (_properties != nullptr)
				_properties->clear();	// if one layer is not contained, clear property vector
			return false;
		}
	}

	return true;
}

void vk::abstraction::get_requiered_glfw_extensions(std::vector<const char*>& glfw_extensions)
{
	if (!glfwVulkanSupported())
		throw std::runtime_error("Tried to read requiered GLFW extensions, but vulkan is not supported!");

	uint32_t n_extensions;
	const char** extensions = glfwGetRequiredInstanceExtensions(&n_extensions);
	for (uint32_t i = 0; i < n_extensions; i++)
		glfw_extensions.push_back(extensions[i]);
}