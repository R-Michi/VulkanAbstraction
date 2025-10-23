/**
 * @brief Includes implementation details for the instance functions.
 * @note Some functions are also borrowed by device functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::detail::instance
{
    /**
     * @brief Checks if a layer at instance level is supported.
     * @param properties Specifies the layer properties.
     * @param count Specifies the number of layer properties.
     * @param layer_name Specifies the name of the layer.
     * @return Returns the index of the layer. If the layer is not supported, vka::NPOS is returned.
     */
    uint32_t supports_layer(const VkLayerProperties* properties, uint32_t count, std::string_view layer_name) noexcept;

    /**
     * @brief Checks if an extension at instance level is supported.
     * @param properties Specifies the extension properties.
     * @param count Specifies the number of extension properties.
     * @param extension_name Specifies the name of the extension.
     * @return Returns the index of the extension. If the extension is not supported, vka::NPOS is returned.
     */
    uint32_t supports_extension(const VkExtensionProperties* properties, uint32_t count, std::string_view extension_name) noexcept;
}
