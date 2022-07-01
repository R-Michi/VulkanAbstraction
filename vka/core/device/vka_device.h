/**
* @file     vka_device.h
* @brief    Useful device functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../detail/device/vka_device.h"

namespace vka
{
    namespace device
    {
        /**
        * @brief                Get all physical devices of a instance.
        * @param[in] instance   vulkan instance
        * @param[out] devices   all physical devices
        */
        void get(VkInstance instance, std::vector<VkPhysicalDevice>& devices);

        /**
        * @brief                    Get properties of (multiple) devices.
        * @param[in] device         physical devices
        * @param[out] properties    the device's properties
        */
        void properties(const std::vector<VkPhysicalDevice>& devices, std::vector<VkPhysicalDeviceProperties>& properties);

        /**
        * @brief                Finds the best matching physical device.
        * @param[in] devices    physical device candidates
        * @param[in] begin      begin index for searching
        * @param[in] filter     physical device filter
        * @return               index of the best matching device
        */
        size_t find(const std::vector<VkPhysicalDevice>& devices, size_t begin, const PhysicalDeviceFilter& filter);

        /**
        * @brief                    Checks if one layer at device level is supported.
        * @param[in]  layer_name    name of the layer
        * @param[out] _property     returned layer properties (optional)
        * @return                   Boolean wether a layer is supported or not.
        */
        bool is_layer_supported(VkPhysicalDevice device, const char* layer_name, VkLayerProperties* _property = nullptr);

        /**
        * @brief                    Checks if multiple layers at device level are supported.
        * @param[in]  layer_names   Vector of layer names to be checked id they are supported.
        * @param[out] idx           Index within the layer_names vector of the not supported layer.
        *                           If layer is supported, the index is set to VKA_INVALID_SIZE.
        * @param[out] _properties   returned layer properties (optional)
        * @return                   Boolean wether all layers are supported or not.
        */
        bool are_layers_supported(VkPhysicalDevice device, const std::vector<const char*>& layer_names, size_t& idx, std::vector<VkLayerProperties>* _properties = nullptr);

        /**
        * @brief                        Checks if one extension at device level is supported.
        * @param[in]  extension_name    name of the extension
        * @param[out] _property         returned extension properties (optional)
        * @return                       Boolean wether a extension is supported or not.
        */
        bool is_extension_supported(VkPhysicalDevice device, const char* extension_name, VkExtensionProperties* _property = nullptr);

        /**
        * @brief                        Checks if multiple extensions at device level are supported.
        * @param[in]  extension_names   Vector of extension names to be checked id they are supported.
        * @param[out] idx               Index within the extension_names vector of the not supported layer.
        *                               If extension is supported, the index is set to VKA_INVALID_SIZE.
        * @param[out] _properties       returned extension properties (optional)
        * @return                       Boolean wether all extensions are supported or not.
        */
        bool are_extensions_supported(VkPhysicalDevice device, const std::vector<const char*>& extension_names, size_t& idx, std::vector<VkExtensionProperties>* _properties = nullptr);
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_device_impl.inl"
#endif
