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
     * @brief Checks all physical device requirements.
     * @return Returns true if all requirements are satisfied.
     */
    bool check_requirements(
        const PhysicalDeviceRequirements& requirements,
        [[maybe_unused]] VkInstance instance,
        [[maybe_unused]] VkPhysicalDevice device,
        const VkPhysicalDeviceProperties& properties,
        const VkPhysicalDeviceMemoryProperties& memory_properties,
        const VkQueueFamilyProperties* queue_family_properties,
        uint32_t queue_family_count
    ) noexcept;
} // namespace vka::detail::device

