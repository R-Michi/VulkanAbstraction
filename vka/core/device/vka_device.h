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
        /*
        * Returns all aviable physical devices of a given instance.
        * The instance is specified by 'instance' and the returned
        * physical devices are stored within 'devices'.
        */
        void get(VkInstance instance, std::vector<VkPhysicalDevice>& devices);

        /*
        * Finds a physical device which matches the specifications specified by a filter and
        * returns the corresponding index within the 'devices'-vector.
        * The filter is a PhysicalDeviceFilter structure and is given by 'filter'.
        */
        size_t find(VkInstance instance, const std::vector<VkPhysicalDevice>& devices, const PhysicalDeviceFilter& filter);

        /*
        * Checks, if a layer at device level is supported by a given physical device.
        * The physical device which should support the layer is specified by 'device' and the
        * name of the layer is specified by 'layer_name'.
        * Returns true, if the physical device supports the layer and false otherwise.
        * Optionally, the layer's properties are returned and stored within the 'properties' argument.
        */
        bool supports_layer(VkPhysicalDevice device, const std::string& layer_name, VkLayerProperties* properties = nullptr);

        /*
        * Checks, if multiple layers at device level are supported by a given physical device.
        * The physical device which should support those layers is spcified by 'device' and the
        * names of those layers are specified by 'layer_names'.
        * If any layer is not supported, the index of the first not supported layer within the
        * 'layer_names'-vector is returned.
        * If all layers are supported, then vka::NPOS is returned.
        * Optionally, the properties of the layers are returned and stored within the 'properties' vector.
        */
        size_t supports_layers(VkPhysicalDevice device, const std::vector<std::string>& layer_names, std::vector<VkLayerProperties>* properties = nullptr);

        /*
        * Checks, if an extension at device level is supported by a given physical device.
        * The physical device which should support the extension is specified by 'device' and the
        * name of the extension is specified by 'extension_name'.
        * Returns true, if the physical device supports the extension and false otherwise.
        * Optionally, the extension's properties are returned and stored within the 'properties' argument.
        */
        bool supports_extension(VkPhysicalDevice device, const std::string& extension_name, VkExtensionProperties* properties = nullptr);

        /*
        * Checks, if multiple extensions at device level are supported by a given physical device.
        * The physical device which should support those extensions is spcified by 'device' and the
        * names of those extensions are specified by 'extension_names'.
        * If any extension is not supported, the index of the first not supported extension within the
        * 'extension_names'-vector is returned.
        * If all extensions are supported, then vka::NPOS is returned.
        * Optionally, the properties of the extensions are returned and stored within the 'properties' vector.
        */
        size_t supports_extensions(VkPhysicalDevice device, const std::vector<std::string>& extension_names, std::vector<VkExtensionProperties>* properties = nullptr);
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_device_impl.inl"
#endif
