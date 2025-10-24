/**
 * @brief Includes implementation details for the device functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::detail::device
{
    /**
     * @brief Checks if all combinations of memory property flags are supported.
     * @param properties Specifies the memory properties.
     * @param flags Specifies the required flags.
     * @param count Specifies the number of flags.
     * @return Returns true if all flags are supported.
     */
    bool check_memory_properties(const VkPhysicalDeviceMemoryProperties& properties, const VkMemoryPropertyFlags* flags, uint32_t count) noexcept;

    /**
     * @brief Checks if all combinations of queue flags are supported.
     * @param properties Specifies the queue family properties.
     * @param queue_family_count Specifies the number of properties.
     * @param flags Specifies the required flags.
     * @param count Specifies the number of flags.
     * @return Returns true if all flags are supported.
     */
    bool check_queue_flags(const VkQueueFamilyProperties* properties, uint32_t queue_family_count, const VkQueueFlags* flags, uint32_t count) noexcept;

#ifdef VKA_GLFW_ENABLE
    /**
     * @brief Checks if any queue family has surface support.
     * @param instance Specifies the vulkan instance.
     * @param device Specifies the physical device.
     * @param queue_family_count Specifies the number of queue families.
     * @return Returns true if surfaces are supported.
     */
    bool check_surface_support(VkInstance instance, VkPhysicalDevice device, uint32_t queue_family_count) noexcept;
#endif

    /**
     * @brief Checks if the device name has a specific sequence.
     * @param properties Specifies the physical device properties containing the device name.
     * @param sequence Specifies the sequence the device name has to contain.
     * @return Returns true if it contains the specified sequence.
     */
    bool check_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence) noexcept;
} // namespace vka::detail::device

