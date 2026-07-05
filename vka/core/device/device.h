/**
 * @brief Helper functions for devices.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
    struct PhysicalDeviceRequirements
    {
        VkPhysicalDeviceType            type;
        const VkMemoryPropertyFlags*    memoryPropertyFlags;
        uint32_t                        memoryPropertyFlagsCount;
        const VkQueueFlags*             queueFamilyFlags;
        uint32_t                        queueFamilyFlagsCount;
        bool                            surfaceSupport;
        const char*                     sequence;
    };
}

namespace vka::device
{
    /**
     * Queries all available physical devices.
     * @param instance Vulkan instance.
     * @return Returns all available physical devices.
     */
    std::vector<VkPhysicalDevice> get(VkInstance instance);

    /**
     * Checks requirements for a physical device.
     * @param instance Vulkan instance. This parameter is only used if GLFW is enabled.
     * @param device Physical device for which the requirements are checked.
     * @param requirements Requirements to check.
     * @param prop Optionally returns the properties if the check was successful.
     * @param mem_prop Optionally returns the memory properties if the check was successful.
     * @return Returns <c>true</c> if all requirements are satisfied.
     * @note If the check was not successful, <c>prop</c> and <c>mem_prop</c> are unaffected.
     */
    bool check_requirements(
        [[maybe_unused]] VkInstance instance,
        VkPhysicalDevice device,
        const PhysicalDeviceRequirements& requirements,
        VkPhysicalDeviceProperties* prop = nullptr,
        VkPhysicalDeviceMemoryProperties* mem_prop = nullptr
    ) noexcept;

    /**
     * Searches for a physical device which supports the requirements for the program.
     * @param instance Vulkan instance. This parameter is only used if GLFW is enabled.
     * @param devices Available physical devices.
     * @param requirements Requirements to check.
     * @param prop Optionally returns the properties if a physical device was found.
     * @param mem_prop Optionally returns the memory properties if a physical device was found.
     * @return Returns the index of the physical device. Returns <c>vka::NPOS</c> if no physical device was found.
     * @note If no physical device was found, <c>prop</c> and <c>mem_prop</c> are unaffected.
     */
    uint32_t find(
        [[maybe_unused]] VkInstance instance,
        const std::vector<VkPhysicalDevice>& devices,
        const PhysicalDeviceRequirements& requirements,
        VkPhysicalDeviceProperties* prop = nullptr,
        VkPhysicalDeviceMemoryProperties* mem_prop = nullptr
    ) noexcept;

    /**
     * Checks if a layer at device level is supported.
     * @param device Physical device for which support should be checked.
     * @param layer_name Name of the layer.
     * @param properties Optionally returns the layer's properties.
     * @return Returns whether the layer is supported.
     */
    bool supports_layer(VkPhysicalDevice device, std::string_view layer_name, VkLayerProperties* properties = nullptr) noexcept;

    /**
     * Checks if multiple layers at device level are supported.
     * @param device Physical device for which support should be checked.
     * @param layer_names Layer names.
     * @param properties Optionally returns the properties of all layers.
     * @return Returns <c>vka::NPOS</c> if all layers are supported. If at least one layer is not supported, the index
     * of the first not supported layer is returned.
     */
    uint32_t supports_layers(VkPhysicalDevice device, const std::vector<std::string>& layer_names, VkLayerProperties* properties = nullptr) noexcept;

    /**
     * Checks if an extension at device level is supported.
     * @param device Physical device for which support should be checked.
     * @param extension_name Name of the extension.
     * @param properties Optionally returns the extension's properties.
     * @return Returns whether the extension is supported.
     */
    bool supports_extension(VkPhysicalDevice device, std::string_view extension_name, VkExtensionProperties* properties = nullptr) noexcept;

    /**
     * Checks if multiple extensions at device level are supported.
     * @param device Physical device for which support should be checked.
     * @param extension_names Extension names.
     * @param properties Optionally returns the properties of all extensions.
     * @return Returns <c>vka::NPOS</c> if all extensions are supported. If at least one extension is not supported, the
     * index of the first not supported extension is returned.
     */
    uint32_t supports_extensions(VkPhysicalDevice device, const std::vector<std::string>& extension_names, VkExtensionProperties* properties = nullptr) noexcept;
}
