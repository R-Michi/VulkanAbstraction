#include "../include/vulkan_absraction.h"

void vk::abstraction::get_queue_family_properties(const VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queue_families)
{
	queue_families.clear();

	uint32_t n_queue_families;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &n_queue_families, nullptr);
	VkQueueFamilyProperties _queue_families[n_queue_families];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &n_queue_families, _queue_families);

	for (size_t i = 0; i < n_queue_families; i++)
		queue_families.push_back(_queue_families[i]);
}

bool queue_family_has_flags(const VkQueueFamilyProperties& properties, VkQueueFlags req_flags)
{
	return ((properties.queueFlags & req_flags) == req_flags);
}

bool queue_family_has_count(const VkQueueFamilyProperties& properties, uint32_t req_count)
{
	return properties.queueCount >= req_count;
}

vk::abstraction::QueueFamilyError vk::abstraction::find_suited_queue_family(const std::vector<VkQueueFamilyProperties>& queue_families, size_t begin, const QueueFamilyFilter& filter, QueueFamilyPriority priority, size_t& index)
{
	using namespace vk::abstraction;

	if (begin >= queue_families.size()) return VKA_QUEUE_FAMILY_ERROR_INVALID_PARAMETER;

	std::vector<size_t> matching_indices;
	QueueFamilyError error;
	uint32_t error_mask = 0;

	for (size_t i = 0; i < queue_families.size(); i++)
	{
		if (!queue_family_has_flags(queue_families[i], filter.reqQueueFlags))
		{
			if(!(error_mask & VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED))
				error = VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED;
			continue;
		}
		else
		{
			error_mask |= VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED;
		}

		if (!queue_family_has_count(queue_families[i], filter.reqQueueCount))
		{
			if(!(error_mask & VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED))
				error = VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED;
			continue;
		}
		else
		{
			error_mask |= VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED;
		}

		matching_indices.push_back(i);
		error = VKA_QUEUE_FAMILY_ERROR_NONE;
	}

	if (error != VKA_QUEUE_FAMILY_ERROR_NONE)
		return error;

	if (priority == VKA_QUEUE_FAMILY_PRIORITY_FIRST)
	{
		index = matching_indices.at(0);
	}
	else if (priority == VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL)
	{
		size_t min_idx = 0;
		VkQueueFlags min_current_xor_requiered = queue_families.at(matching_indices.at(0)).queueFlags ^ filter.reqQueueFlags;

		for (size_t i = 1; i < matching_indices.size(); i++)
		{
			const VkQueueFlags current_xor_requiered = queue_families.at(matching_indices.at(i)).queueFlags ^ filter.reqQueueFlags;

			if (current_xor_requiered < min_current_xor_requiered)
			{
				min_current_xor_requiered = current_xor_requiered;
				min_idx = matching_indices.at(i);
			}
		}

		index = min_idx;
	}
	else
	{
		return VKA_QUEUE_FAMILY_ERROR_INVALID_PARAMETER;
	}

	return VKA_QUEUE_FAMILY_ERROR_NONE;
}

const char* vk::abstraction::queue_family_strerror(QueueFamilyError error)
{
	using namespace vk::abstraction;

	switch (error)
	{
	case VKA_QUEUE_FAMILY_ERROR_NONE:
		return "No error occured!";
	case VKA_QUEUE_FAMILY_ERROR_INVALID_PARAMETER:
		return "Invalid parameter!";
	case VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED:
		return "No queue family found with requiered flags!";
	case VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED:
		return "No queue family found with requiered queue count!";
	default:
		return "Unknown error!";
	}
}

bool vk::abstraction::validate_queue_families(const std::vector<VkQueueFamilyProperties>& queue_families, const std::vector<QueueInfo>& queue_infos)
{
	std::vector<QueueInfo> maximum_queue_infos;
	for (size_t i = 0; i < queue_families.size(); i++)
		maximum_queue_infos.push_back({ static_cast<uint32_t>(i), 0, 0});

	for (size_t i = 0; i < queue_infos.size(); i++)
	{
		maximum_queue_infos.at(queue_infos.at(i).queueFamilyIndex).usedQueueCount += queue_infos.at(i).usedQueueCount;
		if (queue_families.at(queue_infos.at(i).queueFamilyIndex).queueCount < (queue_infos.at(i).queueBaseIndex + queue_infos.at(i).usedQueueCount))
			return false;
	}

	for (size_t i = 0; i < queue_families.size(); i++)
	{
		if (maximum_queue_infos.at(i).usedQueueCount > queue_families.at(i).queueCount)
			return false;
	}
	return true;
}

bool vk::abstraction::is_device_layer_supported(VkPhysicalDevice device, const char* layer_name, VkLayerProperties* _property)
{
	uint32_t n_layers;
	VkResult res = vkEnumerateDeviceLayerProperties(device, &n_layers, nullptr);	// get number of layers
	if (res != VK_SUCCESS) return false;

	VkLayerProperties properties[n_layers];
	res = vkEnumerateDeviceLayerProperties(device, &n_layers, properties);		// get layer properties
	if (res != VK_SUCCESS) return false;

	for (uint32_t i = 0; i < n_layers; i++)
	{
		if (strcmp(layer_name, properties[i].layerName) == 0)
		{
			if (_property != nullptr)
				*_property = properties[i];
			return true;
		}
	}

	return false;
}

bool vk::abstraction::are_device_layers_supported(VkPhysicalDevice device, std::vector<const char*> layer_names, std::vector<VkLayerProperties>* _properties)
{
	uint32_t n_layers;
	VkResult res = vkEnumerateDeviceLayerProperties(device, &n_layers, nullptr);	// get number of layers
	if (res != VK_SUCCESS) return false;

	VkLayerProperties properties[n_layers];
	res = vkEnumerateDeviceLayerProperties(device, &n_layers, properties);		// get layer properties
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
			if (_properties != nullptr)
				_properties->clear();	// if one layer is not contained, clear property vector
			return false;
		}
	}

	return true;
}

bool vk::abstraction::is_device_extension_supported(VkPhysicalDevice device, const char* extension_name, VkExtensionProperties* _property)
{
	uint32_t n_extensions;
	VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, nullptr);	// get number of extensions
	if (res != VK_SUCCESS) return false;

	VkExtensionProperties properties[n_extensions];
	res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, properties);		// get extension properties
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

bool vk::abstraction::are_device_extensions_supported(VkPhysicalDevice device, std::vector<const char*> extension_names, std::vector<VkExtensionProperties>* _properties)
{
	uint32_t n_extensions;
	VkResult res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, nullptr);	// get number of layers
	if (res != VK_SUCCESS) return false;

	VkExtensionProperties properties[n_extensions];
	res = vkEnumerateDeviceExtensionProperties(device, nullptr, &n_extensions, properties);		// get layer properties
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