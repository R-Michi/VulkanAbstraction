/**
 * @brief Implementation for the queue functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include "../../vka.h"

std::vector<VkQueueFamilyProperties> vka::queue::properties(const VkPhysicalDevice& device)
{
    std::vector<VkQueueFamilyProperties> queue_families;
    uint32_t count;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    queue_families.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queue_families.data());

    return queue_families;
}

uint32_t vka::queue::find(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueFamilyRequirements& requirements, QueueFamilyPriority priority) noexcept
{
    uint32_t idx = NPOS;
    VkQueueFlags min_flags = VK_QUEUE_FLAG_BITS_MAX_ENUM;
    for(uint32_t i = 0; i < queue_families.size(); i++)
    {
        // every check was successful
        if (detail::queue::check_requirements(requirements, queue_families[i]))
        {
            if (priority == QueueFamilyPriority::FIRST)
                return i;
            if (priority == QueueFamilyPriority::OPTIMAL)
            {
                // If there are multiple options, searches for the queue family with the "least power".
                // This means with the queue family which has the least number of additional flags set as specified
                // in the requirements.
                const VkQueueFlags cur_flags = queue_families[i].queueFlags ^ requirements.queueFlags;
                if (cur_flags < min_flags)
                {
                    min_flags = cur_flags;
                    idx = i;
                }
            }
        }
    }
    return idx;
}
