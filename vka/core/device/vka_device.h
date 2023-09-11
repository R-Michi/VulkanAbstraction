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
        * Returns all aviable physical devices of a given instance. The instance is specified by
        * 'instance' and the returned physical devices are stored within 'devices'.
        * NOTE: An std::vector is used because the number of devices is not known in advance.
        * However, a good heuristic can be made of how many physical divices are aviable with which
        * memory can be reserved in the vector to speed up querying.
        */
        void get(VkInstance instance, std::vector<VkPhysicalDevice>& devices);

        /*
        * Finds a physical device which matches the specifications specified by a filter and
        * returns the corresponding index within the 'devices'-vector. The filter is a
        * PhysicalDeviceFilter structure and is given by 'filter'. Optinally, the physical device's
        * properties and memory properties are returned and stored in 'prop' and 'mem_prop'.
        */
        size_t find(VkInstance instance, const std::vector<VkPhysicalDevice>& devices, const PhysicalDeviceFilter& filter, VkPhysicalDeviceProperties* prop = nullptr, VkPhysicalDeviceMemoryProperties* mem_prop = nullptr);

        /*
        * Checks, if a layer at device level is supported by a given physical device. The physical
        * device which should support the layer is specified by 'device' and the name of the layer
        * is specified by 'layer_name'. Returns true, if the physical device supports the layer and
        * false otherwise. Optionally, the layer's properties are stored in 'properties'.
        */
        bool supports_layer(VkPhysicalDevice device, std::string_view layer_name, VkLayerProperties* properties = nullptr) noexcept;

        /*
        * Checks, if multiple layers at device level are supported by a given physical device.
        * The physical device which should support those layers is spcified by 'device' and the
        * names of those layers are specified by 'layer_names'. If any layer is not supported, the
        * index of the first not supported layer within the 'layer_names'-vector is returned.
        * If all layers are supported, then vka::NPOS is returned. Optionally, the properties of
        * the layers are stored in the 'properties' array.
        */
        size_t supports_layers(VkPhysicalDevice device, const std::vector<std::string>& layer_names, VkLayerProperties* properties = nullptr) noexcept;

        /*
        * Checks, if an extension at device level is supported by a given physical device.
        * The physical device which should support the extension is specified by 'device' and the
        * name of the extension is specified by 'extension_name'. Returns true, if the physical
        * device supports the extension and false otherwise. Optionally, the extension's properties
        * are stored in 'properties'.
        */
        bool supports_extension(VkPhysicalDevice device, std::string_view extension_name, VkExtensionProperties* properties = nullptr) noexcept;

        /*
        * Checks, if multiple extensions at device level are supported by a given physical device.
        * The physical device which should support those extensions is spcified by 'device' and the
        * names of those extensions are specified by 'extension_names'. If any extension is not
        * supported, the index of the first not supported extension within the 
        * 'extension_names'-vector is returned. If all extensions are supported, then vka::NPOS is
        * returned. Optionally, the properties of the extensions are stored in the 'properties' array.
        */
        size_t supports_extensions(VkPhysicalDevice device, const std::vector<std::string>& extension_names, VkExtensionProperties* properties = nullptr) noexcept;
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_device_impl.inl"
#endif
