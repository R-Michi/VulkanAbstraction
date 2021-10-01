/**
* @file     vka_device.h
* @brief    Useful device related functions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../vka_global.h"
#include "../vka_types.h"

namespace vka
{
    namespace device
    {
        /**
        * @brief Get all physical devices of a instance.
        * @param[in] instance: vulkan instance
        * @param[out] devices: all physical devices
        */
        void get(const VkInstance& instance, std::vector<VkPhysicalDevice>& devices);

        /**
        * @brief Get properties of (multiple) devices.
        * @param[in] device: physical devices
        * @param[out] properties: the device's properties
        */
        void properties(const std::vector<VkPhysicalDevice>& devices, std::vector<VkPhysicalDeviceProperties>& properties);

        /**
        * @brief Finds the best matching physical device.
        * @param[in] devices: physical device candidates
        * @param[in] begin: begin index for searching
        * @param[in] filter: physical device filter
        * @param[out] index: index of the device of the devices vector @param devices
        * @return error if no physical device was found
        */
        PhysicalDeviceError find(const std::vector<VkPhysicalDevice>& devices, size_t begin, const PhysicalDeviceFilter& filter, size_t& index);

        /**
        * @brief Converts FindPhysicalDeviceError enum to string.
        * @param[in] error: error code
        * @return String of the error code.
        */
        const char* strerror(PhysicalDeviceError error);

        /**
        * @brief Checks if one layer at instance level is supported.
        * @param[in] device: physical device
        * @param[in] layer_name: name of the layer
        * @param[out] _property: returned layer properties (optional)
        * @return 'true' if layer is supported, 'false' if layer is not supported
        */
        bool is_layer_supported(VkPhysicalDevice device, const char* layer_name, VkLayerProperties* _property = nullptr);

        /**
        * @brief Checks if multiple layers at instance level are supported.
        * @param[in] device: physical device
        * @param[in] layer_names: names of multiple layers
        * @param[out] _properties: returned layer properties (optional)
        * @return 'true' if all layers are supported, 'false' if one or more layers are not supported
        */
        bool are_layers_supported(VkPhysicalDevice device, std::vector<const char*> layer_names, std::vector<VkLayerProperties>* _properties = nullptr);

        /**
        * @brief Checks if one extension at instance level is supported.
        * @param[in] device: physial device
        * @param extension_name[in]: name of the extension
        * @param _property: returned extension properties (optional)
        * @return 'true' if extension is supported, 'false' if extension is not supported
        */
        bool is_extension_supported(VkPhysicalDevice device, const char* extension_name, VkExtensionProperties* _property = nullptr);

        /**
        * @brief Checks if multiple extensions at instance level are supported.
        * @param[in] device: physical device
        * @param[in] extension_names: name of multiple extensions
        * @param[out] _properties: returned extension properties (optional)
        * @return 'true' if all extensions are supported, 'false' if one or more extensions are not supported
        */
        bool are_extensions_supported(VkPhysicalDevice device, std::vector<const char*> extension_names, std::vector<VkExtensionProperties>* _properties = nullptr);
    }
}
