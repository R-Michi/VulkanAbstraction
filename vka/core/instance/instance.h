/**
 * @brief Helper functions for instances.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::instance
{
    /**
     * Checks if a layer at instance level is supported.
     * @param layer_name Name of the layer.
     * @param properties Optionally returns the layer's properties.
     * @return Returns whether the layer is supported.
     */
    bool supports_layer(std::string_view layer_name, VkLayerProperties* properties = nullptr) noexcept;

    /**
     * Checks if multiple layers at instance level are supported.
     * @param layer_names Layer names.
     * @param properties Optionally returns the properties of all layers.
     * @return Returns <c>vka::NPOS</c> if all layers are supported. If at least one layer is not supported, the index
     * of the first not supported layer is returned.
     */
    uint32_t supports_layers(const std::vector<std::string>& layer_names, VkLayerProperties* properties = nullptr) noexcept;

    /**
     * Checks if an extension at instance level is supported.
     * @param extension_name Name of the extension.
     * @param properties Optionally returns the extension's properties.
     * @return Returns whether the extension is supported.
     */
    bool supports_extension(std::string_view extension_name, VkExtensionProperties* properties = nullptr) noexcept;

    /**
     * Checks if multiple extensions at instance level are supported.
     * @param extension_names Extension names.
     * @param properties Optionally returns the properties of all extensions.
     * @return Returns <c>vka::NPOS</c> if all extensions are supported. If at least one extension is not supported, the
     * index of the first not supported extension is returned.
     */
    uint32_t supports_extensions(const std::vector<std::string>& extension_names, VkExtensionProperties* properties = nullptr) noexcept;

#ifdef VKA_GLFW_ENABLE
    /// @return Returns all required extensions by GLFW.
    std::vector<std::string> get_glfw_extensions();

    /**
     * Queries all required extensions by GLFW.
     * @param extensions Array where the extension strings are returned.
     * @return Returns the number of extensions that where copied.
     */
    uint32_t get_glfw_extensions(const char** extensions) noexcept;
#endif
}
