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

#include "../../detail/instance/vka_instance.h"

namespace vka
{
    namespace instance
    {
        /*
        * Checks, if a layer at instance level is supported. The name of the layer is specified by
        * 'layer_name'. Returns true, if the layer is supported and false otherwise. Optionally,
        * the layer's properties are stored in 'properties'.
        */
        bool supports_layer(std::string_view layer_name, VkLayerProperties* properties = nullptr) noexcept;

        /*
        * Checks, if multiple layers at instance level are supported.
        * The names of those layers are specified by 'layer_names'.
        * If any layer is not supported, the index of the first not supported layer within the
        * 'layer_names'-vector is returned.
        * If all layers are supported, then vka::NPOS is returned.
        * Optionally, the properties of the layers are returned and stored within the 'properties' vector.
        */
        size_t supports_layers(const std::vector<std::string>& layer_names, VkLayerProperties* properties = nullptr) noexcept;

        /*
        * Checks, if an extension at instance level is supported.
        * The name of the extension is specified by 'extension_name'.
        * Returns true, if the extension is supported and false otherwise.
        * Optionally, the extension's properties are returned and stored within the 'properties' argument.
        */
        bool supports_extension(std::string_view extension_name, VkExtensionProperties* properties = nullptr) noexcept;

        /*
        * Checks, if multiple extensions at instance level are supported.
        * The names of those extensions are specified by 'extension_names'.
        * If any extension is not supported, the index of the first not supported extension within the
        * 'extension_names'-vector is returned.
        * If all extensions are supported, then vka::NPOS is returned.
        * Optionally, the properties of the extensions are returned and stored within the 'properties' vector.
        */
        size_t supports_extensions(const std::vector<std::string>& extension_names, VkExtensionProperties* properties = nullptr) noexcept;

#ifdef VKA_GLFW_ENABLE

        /*
        * Returns all requiered extensions by GLFW.
        * The extensions are stored withon the 'extensions' vector.
        * The returned extensions dont overwrite the 'extensions' vector,
        * instead those extensions will be appended at the end of the vector.
        */
        void get_glfw_extensions(std::vector<std::string>& extensions);
        void get_glfw_extensions(const char** extensions) noexcept;
#endif
    }

#ifdef VKA_IMPLEMENTATION
    #include "vka_instance_impl.inl"
#endif
}