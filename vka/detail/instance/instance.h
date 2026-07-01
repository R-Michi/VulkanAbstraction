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
     * Checks if a layer at instance level is supported.
     * @param properties Layers of the vulkan instance.
     * @param count Number of layers to check.
     * @param layer_name Name of the layer to check for.
     * @return Returns the index of the layer. If the layer is not supported, <c>vka::NPOS</c> is returned.
     */
    uint32_t supports_layer(const VkLayerProperties* properties, uint32_t count, std::string_view layer_name) noexcept;

    /**
     * Checks if an extension at instance level is supported.
     * @param properties Extensions of the vulkan instance.
     * @param count Number of extensions to check.
     * @param extension_name Name of the extension to check for.
     * @return Returns the index of the extension. If the extension is not supported, <c>vka::NPOS</c> is returned.
     */
    uint32_t supports_extension(const VkExtensionProperties* properties, uint32_t count, std::string_view extension_name) noexcept;
}
