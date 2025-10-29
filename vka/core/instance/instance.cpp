/**
 * @brief Implementation for the instance functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include <vka/vka.h>

bool vka::instance::supports_layer(std::string_view layer_name, VkLayerProperties* properties) noexcept
{
    uint32_t layer_count;
    VkResult res = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (res != VK_SUCCESS && res != VK_INCOMPLETE) return false;

    VkLayerProperties* const layer_properties = (VkLayerProperties*)alloca(layer_count * sizeof(VkLayerProperties));
    res = vkEnumerateInstanceLayerProperties(&layer_count, layer_properties);
    if (res != VK_SUCCESS && res != VK_INCOMPLETE) return false;

    const size_t idx = detail::instance::supports_layer(layer_properties, layer_count, layer_name);
    const bool b = (idx != NPOS);
    if (b && properties != nullptr)
        *properties = layer_properties[idx];
    return b;
}

uint32_t vka::instance::supports_layers(const std::vector<std::string>& layer_names, VkLayerProperties* properties) noexcept
{
    uint32_t layer_count;
    VkResult res = vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkLayerProperties* const layer_properties = (VkLayerProperties*)alloca(layer_count * sizeof(VkLayerProperties));
    res = vkEnumerateInstanceLayerProperties(&layer_count, layer_properties);
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < layer_names.size(); i++)
    {
        size_t j;
        if ((j = detail::instance::supports_layer(layer_properties, layer_count, layer_names[i])) == NPOS)   // layer 'layer_names[i]' is not supported
            return i;
        if (properties != nullptr)
            properties[i] = layer_properties[j];
    }
    return NPOS;
}

bool vka::instance::supports_extension(std::string_view extension_name, VkExtensionProperties* properties) noexcept
{
    uint32_t extension_count;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties* const extension_properties = (VkExtensionProperties*)alloca(extension_count * sizeof(VkExtensionProperties));
    res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties);
    if (res != VK_SUCCESS) return false;

    const size_t idx = detail::instance::supports_extension(extension_properties, extension_count, extension_name);
    const bool b = (idx != NPOS);
    if (b && properties != nullptr)
        *properties = extension_properties[idx];
    return b;
}

uint32_t vka::instance::supports_extensions(const std::vector<std::string>& extension_names, VkExtensionProperties* properties) noexcept
{
    uint32_t extension_count;
    VkResult res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    if (res != VK_SUCCESS) return false;

    VkExtensionProperties* const extension_properties = (VkExtensionProperties*)alloca(extension_count * sizeof(VkExtensionProperties));
    res = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extension_properties);
    if (res != VK_SUCCESS) return false;

    for (size_t i = 0; i < extension_names.size(); i++)
    {
        size_t j;
        if ((j = detail::instance::supports_extension(extension_properties, extension_count, extension_names[i])) == NPOS) // extension 'extension_names[i]' is not supported
            return i;
        if (properties != nullptr)
            properties[i] = extension_properties[j];
    }
    return NPOS;
}

#ifdef VKA_GLFW_ENABLE
std::vector<std::string> vka::instance::get_glfw_extensions()
{
    std::vector<std::string> extensions;
    uint32_t count;

    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&count);
    extensions.reserve(count);
    for (uint32_t i = 0; i < count; i++)
        extensions.emplace_back(glfw_extensions[i]);

    return extensions;
}

uint32_t vka::instance::get_glfw_extensions(const char** extensions) noexcept
{
    uint32_t count;
    const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&count);
    for (uint32_t i = 0; i < count; i++)
        extensions[i] = glfw_extensions[i];
    return count;
}
#endif
