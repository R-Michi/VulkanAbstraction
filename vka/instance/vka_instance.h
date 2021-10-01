/**
* @file     vka_instance.h
* @brief    Useful instance related functions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../vka_global.h"

namespace vka
{
    namespace instance
    {
        /**
        * @brief Checks if one layer at instance level is supported.
        * @param[in] layer_name: name of the layer
        * @param[out] _property: returned layer properties (optional)
        * @return 'true' if layer is supported, 'false' if layer is not supported
        */
        bool is_layer_supported(const char* layer_name, VkLayerProperties* _property = nullptr);

        /**
        * @brief Checks if multiple layers at instance level are supported.
        * @param[in] layer_names: names of multiple layers
        * @param[out] _properties: returned layer properties (optional)
        * @return 'true' if all layers are supported, 'false' if one or more layers are not supported
        */
        bool are_layers_supported(std::vector<const char*> layer_names, std::vector<VkLayerProperties>* _properties = nullptr);

        /**
        * @brief Checks if one extension at instance level is supported.
        * @param[in] extension_name: name of the extension
        * @param[out] _property: returned extension properties (optional)
        * @return 'true' if extension is supported, 'false' if extension is not supported
        */
        bool is_extension_supported(const char* extension_name, VkExtensionProperties* _property = nullptr);

        /**
        * @brief Checks if multiple extensions at instance level are supported.
        * @param[in] extension_names: names of multiple extensions
        * @param[out] _properties: returned extension properties (optional)
        * @return 'true' if all extensions are supported, 'false' if one or more extensions are not supported
        */
        bool are_extensions_supported(std::vector<const char*> extension_names, std::vector<VkExtensionProperties>* _properties = nullptr);

        /**
        * @param[out] glfw_extensions: returns all extensions requiered for GLFW
        */
        void get_glfw_extensions(std::vector<const char*>& glfw_extensions);
    }
}