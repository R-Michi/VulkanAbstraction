/**
 * @brief Helper functions for devices.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::device
{
    /**
     * @brief Queries all available physical devices.
     * @param instance Specifies a valid vulkan instance.
     * @return Returns all available physical devices.
     */
    std::vector<VkPhysicalDevice> get(VkInstance instance);

    /**
     * @brief Searches for a physical device which supports the requirements for the program.
     * @param instance Specifies a valid vulkan instance. This parameter is not used if GLFW is not enabled.
     * @param devices Specifies all available physical devices.
     * @param filter Specifies the requirements for the program.
     * @param prop Optionally returns the properties of the first found physical device.
     * @param mem_prop Optionally returns the memory properties of the first found physical device.
     * @return Returns the first physical device which matches the requirements. If no physical device could be found,
     * VK_NULL_HANDLE is returned.
     */
    VkPhysicalDevice find(
        [[maybe_unused]] VkInstance instance,
        const std::vector<VkPhysicalDevice>& devices,
        const PhysicalDeviceFilter& filter,
        VkPhysicalDeviceProperties* prop = nullptr,
        VkPhysicalDeviceMemoryProperties* mem_prop = nullptr
    );

    /**
     * @brief Checks if a layer at device level is supported.
     * @param device Specifies a valid physical device for which support should be checked.
     * @param layer_name Specifies the name of the layer.
     * @param properties Optionally returns the layer's properties.
     * @return Returns 'true' if the layer is supported and 'false' otherwise.
     */
    bool supports_layer(VkPhysicalDevice device, std::string_view layer_name, VkLayerProperties* properties = nullptr) noexcept;

    /**
     * @brief Checks if multiple layers at device level are supported.
     * @param device Specifies a valid physical device for which support should be checked.
     * @param layer_names Specifies the layer names.
     * @param properties Optionally returns the properties of all layers.
     * @return Returns vka::NPOS if all layers are supported. If at least one layer is not supported, the index of the
     * first not supported layer is returned.
     */
    size_t supports_layers(VkPhysicalDevice device, const std::vector<std::string>& layer_names, VkLayerProperties* properties = nullptr) noexcept;

    /**
     * @brief Checks if an extension at device level is supported.
     * @param device Specifies a valid physical device for which support should be checked.
     * @param extension_name Specifies the name of the extension.
     * @param properties Optionally returns the extension's properties.
     * @return Returns 'true' if the extension is supported and 'false' otherwise.
     */
    bool supports_extension(VkPhysicalDevice device, std::string_view extension_name, VkExtensionProperties* properties = nullptr) noexcept;

    /**
     * @brief Checks if multiple extensions at device level are supported.
     * @param device Specifies a valid physical device for which support should be checked.
     * @param extension_names Specifies the extension names.
     * @param properties Optionally returns the properties of all extensions.
     * @return Returns vka::NPOS if all extensions are supported. If at least one extension is not supported, the index
     * of the first not supported extension is returned.
     */
    size_t supports_extensions(VkPhysicalDevice device, const std::vector<std::string>& extension_names, VkExtensionProperties* properties = nullptr) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "device.inl"
#endif
