/**
* @file     vka_instance.h
* @brief    Useful instance functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka
{
    namespace instance
    {
        /**
        * @brief                    Checks if one layer at instance level is supported.
        * @param[in]  layer_name    name of the layer
        * @param[out] _property     returned layer properties (optional)
        * @return                   Boolean wether a layer is supported or not.
        */
        bool is_layer_supported(const char* layer_name, VkLayerProperties* _property = nullptr);

        /**
        * @brief                    Checks if multiple layers at instance level are supported.
        * @param[in]  layer_names   Vector of layer names to be checked id they are supported.
        * @param[out] idx           Index within the layer_names vector of the not supported layer.
        *                           If layer is supported, the index is set to VKA_INVALID_SIZE.
        * @param[out] _properties   returned layer properties (optional)
        * @return                   Boolean wether all layers are supported or not.
        */
        bool are_layers_supported(const std::vector<const char*>& layer_names, size_t& idx, std::vector<VkLayerProperties>* _properties = nullptr);

        /**
        * @brief                        Checks if one extension at instance level is supported.
        * @param[in]  extension_name    name of the extension
        * @param[out] _property         returned extension properties (optional)
        * @return                       Boolean wether a extension is supported or not.
        */
        bool is_extension_supported(const char* extension_name, VkExtensionProperties* _property = nullptr);

        /**
        * @brief                        Checks if multiple extensions at instance level are supported.
        * @param[in]  extension_names   Vector of extension names to be checked id they are supported.
        * @param[out] idx               Index within the extension_names vector of the not supported layer.
        *                               If extension is supported, the index is set to VKA_INVALID_SIZE.
        * @param[out] _properties       returned extension properties (optional)
        * @return                       Boolean wether all extensions are supported or not.
        */
        bool are_extensions_supported(const std::vector<const char*>& extension_names, size_t& idx, std::vector<VkExtensionProperties>* _properties = nullptr);

#ifndef VKA_GLFW_DISABLE
        /**
        * @param[out] glfw_extensions returns all extensions requiered for GLFW
        */
        void get_glfw_extensions(std::vector<const char*>& glfw_extensions);
#endif
    }

#ifdef VKA_IMPLEMENTATION
    #include "vka_instance_impl.inl"
#endif
}