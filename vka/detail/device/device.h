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
     * @brief Checks if one combination of memory property flags is supported.
     * @param properties Specifies the memory properties.
     * @param req_flags Specifies the required flags.
     * @return Returns 'true' if the flags are supported.
     */
    inline bool has_memory_property(const VkPhysicalDeviceMemoryProperties& properties, VkMemoryPropertyFlags req_flags) noexcept;

    /**
     * @brief Checks if one combination of queue flags is supported.
     * @param properties Specifies the queue family properties.
     * @param n Specifies the number of properties.
     * @param req_flags Specifies the required flags.
     * @return Returns 'true' if the flags are supported.
     */
    inline bool has_queue_flag(const VkQueueFamilyProperties* properties, size_t n, VkQueueFlags req_flags) noexcept;

    /**
     * @brief Checks if the device name has a specific sequence.
     * @param properties Specifies the physical device properties containing the device name.
     * @param sequence Specifies the sequence the device name has to contain.
     * @return Returns 0x00 if the sequence was found and 0x01, if not.
     */
    inline uint8_t has_sequence(const VkPhysicalDeviceProperties& properties, const char* sequence) noexcept;

    /**
     * @brief Checks if all combinations of memory property flags are supported.
     * @param properties Specifies the memory properties.
     * @param req_flags Specifies the required flags.
     * @return Returns 0x00 if all flag combinations are supported and 0x02, if not.
     */
    inline uint8_t has_memory_properties(const VkPhysicalDeviceMemoryProperties& properties, const std::vector<VkMemoryPropertyFlags>& req_flags) noexcept;

    /**
     * @brief Checks if all combinations of queue flags are supported.
     * @param properties Specifies the queue family properties.
     * @param n Specifies the number of properties.
     * @param req_flags Specifies the required flags.
     * @return Returns 0x00 if all flag combinations are supported and 0x04, if not.
     */
    inline uint8_t has_queue_flags(const VkQueueFamilyProperties* properties, size_t n, const std::vector<VkQueueFlags>& req_flags) noexcept;

#ifdef VKA_GLFW_ENABLE // requires glfw
    /**
     * @brief Checks if any queue family has surface support.
     * @param instance Specifies the vulkan instance.
     * @param device Specifies the physical device.
     * @param queue_family_count Specifies the number of queue families.
     * @return If any queue family has surface support, 0x00 is returned and 0x08 otherwise.
     */
    inline uint8_t has_surface_support(VkInstance instance, VkPhysicalDevice device, uint32_t queue_family_count) noexcept;
#endif
} // namespace vka::detail::device

#include "device.inl"

