/**
 * @brief Helper functions for queues.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::queue
{
    /**
     * @brief Queries all queue families (actually their properties) from a physical device.
     * @param device Specifies a valid physical device.
     * @return Returns a vector containing all queue family properties.
     */
    std::vector<VkQueueFamilyProperties> properties(const VkPhysicalDevice& device);

    /**
     * @brief Searches for queue family which supports the requirements for the program.
     * @param queue_families Specifies all available queue family properties.
     * @param requirements Specifies the requirements for the program.
     * @param priority Specifies a priority for the search operation.\n
     *  - VKA_QUEUE_FAMILY_PRIORITY_FIRST Chooses the first queue family that meets the requirements.\n
     *  - VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL Chooses the queue family which has the least number of additional queue\n
     *  family flags to those specified in the filter, if multiple queue families meet the requirements. In other words,
     *  if multiple queue families satisfy the requirements, the queue family with the "least power" is chosen.
     * @return Returns the index of the found queue family or vka::NPOS if no queue family could be found.
     */
    uint32_t find(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueFamilyRequirements& requirements, QueueFamilyPriority priority) noexcept;
}
