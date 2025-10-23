/**
 * @brief Implementation details for the instance functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include "../../vka.h"

uint32_t vka::detail::instance::supports_layer(const VkLayerProperties* properties, uint32_t count, std::string_view layer_name) noexcept
{
    for (uint32_t i = 0; i < count; i++)  // count must be uint32_t required by vulkan
    {
        if (layer_name == properties[i].layerName)
            return i;
    }
    return NPOS;
}

uint32_t vka::detail::instance::supports_extension(const VkExtensionProperties* properties, uint32_t count, std::string_view extension_name) noexcept
{
    for (uint32_t i = 0; i < count; i++)  // count must be uint32_t required by vulkan
    {
        if (extension_name == properties[i].extensionName)
            return i;
    }
    return NPOS;
}
