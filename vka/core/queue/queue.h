/**
 * @brief Helper functions for queues.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
    /**
     * Specifies the search priority for <c>find()</c>.
     * - <c>FIRST</c> -- Chooses the first queue family that meets the requirements.
     * - <c>OPTIMAL</c> -- Chooses the queue family which has the least number of additional queue family flags.
     */
    enum class QueueFamilyPriority : uint8_t
    {
        FIRST,
        OPTIMAL,
    };

    using QueueFamilyRequirements = detail::queue::QueueFamilyRequirements;
}

namespace vka::queue
{
    /**
     * Queries the properties of all queue families from a physical device.
     * @param device Physical device to query the properties from.
     * @return Returns a vector containing all queue family properties.
     */
    std::vector<VkQueueFamilyProperties> properties(const VkPhysicalDevice& device);

    /**
     * Searches for queue family which supports the specified requirements.
     * @param queue_families All available queue family properties.
     * @param requirements Specifies the requirements.
     * @param priority Priority for the search operation.
     * @return Returns the index of the found queue family or <c>vka::NPOS</c> if no queue family was found.
     */
    uint32_t find(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueFamilyRequirements& requirements, QueueFamilyPriority priority) noexcept;
}
