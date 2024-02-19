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

namespace vka::instance
{
    /**
     * @brief Checks, if a layer at instance level is supported.
     * @param layer_name Specifies the name of the layer.
     * @param properties Optionally returns the layer's properties.
     * @return Returns 'true', if the layer is supported and 'false' otherwise.
     */
    bool supports_layer(std::string_view layer_name, VkLayerProperties* properties = nullptr) noexcept;

    /**
     * @brief Checks, if multiple layers at instance level are supported.
     * @param layer_names Specifies the layer names.
     * @param properties Optionally returns the properties of all layers.
     * @return Returns vka::NPOS, if all layers are supported. If at least one layer is not supported, the index of the
     * first not supported layer is returned.
     */
    size_t supports_layers(const std::vector<std::string>& layer_names, VkLayerProperties* properties = nullptr) noexcept;

    /**
     * @brief Checks, if an extension at instance level is supported.
     * @param extension_name Specifies the name of the extension.
     * @param properties Optionally returns the extension's properties.
     * @return Returns 'true', if the extension is supported and 'false' otherwise.
     */
    bool supports_extension(std::string_view extension_name, VkExtensionProperties* properties = nullptr) noexcept;

    /**
     * @brief Checks, if multiple extensions at instance level are supported.
     * @param extension_names Specifies the extension names.
     * @param properties Optionally returns the properties of all extensions.
     * @return Returns vka::NPOS, if all extensions are supported. If at least one extension is not supported, the index
     * of the first not supported extension is returned.
     */
    size_t supports_extensions(const std::vector<std::string>& extension_names, VkExtensionProperties* properties = nullptr) noexcept;

#ifdef VKA_GLFW_ENABLE
    /**
     * @brief Returns all required extensions by GLFW.
     * @param extensions Appends the GLFW extensions to the specified vector.
     */
    void get_glfw_extensions(std::vector<std::string>& extensions);

    /**
     * @brief Returns all required extensions by GLFW.
     * @details Copies the strings (pointers) from the array that GLFW returns into the parameter array. This way other
     * extensions can later be appended to the array.
     * @param extensions Specifies the array where the extension strings are returned.
     * @return Returns the number of extensions that where copied.
     */
    uint32_t get_glfw_extensions(const char** extensions) noexcept;
#endif
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_instance_impl.inl"
#endif