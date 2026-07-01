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
     * Checks if all combinations of memory property flags are supported.
     * @param properties Memory properties of the physical device.
     * @param flags Required memory property flags.
     * @param count Number of entries the array <c>flags</c> contains.
     * @return Returns <c>true</c> if all flags are supported.
     */
    bool check_memory_properties(const VkPhysicalDeviceMemoryProperties& properties, const VkMemoryPropertyFlags* flags, uint32_t count) noexcept;

    /**
     * Checks if all combinations of queue flags are supported.
     * @param properties Queue family properties of the physical device.
     * @param queue_family_count Number of queue families. For every queue family there must be an entry in the
     * <c>properties</c> array.
     * @param flags Required queue flags.
     * @param count Number of entries the array <c>flags</c> contains.
     * @return Returns <c>true</c> if all flags are supported.
     */
    bool check_queue_flags(const VkQueueFamilyProperties* properties, uint32_t queue_family_count, const VkQueueFlags* flags, uint32_t count) noexcept;

#ifdef VKA_GLFW_ENABLE
    /**
     * Checks if any queue family has surface support.
     * @param instance Vulkan instance.
     * @param device Physical device for which to check surface support.
     * @param queue_family_count Number of queue families of the physical device.
     * @return Returns <c>true</c> if surfaces are supported.
     */
    bool check_surface_support(VkInstance instance, VkPhysicalDevice device, uint32_t queue_family_count) noexcept;
#endif

    /**
     * Checks if the device name has a specific sequence.
     * @param properties Physical device properties containing the device name.
     * @param sequence Sequence the device name has to contain.
     * @return Returns <c>true</c> if the name contains the specified sequence.
     */
    bool check_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence) noexcept;
} // namespace vka::detail::device

