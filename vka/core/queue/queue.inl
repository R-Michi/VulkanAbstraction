/**
 * @brief Implementation for the queue functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

std::vector<VkQueueFamilyProperties> vka::queue::properties(const VkPhysicalDevice& device)
{
    std::vector<VkQueueFamilyProperties> queue_families;
    uint32_t count;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    queue_families.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queue_families.data());

    return queue_families;
}

size_t vka::queue::find(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueFamilyFilter& filter, QueueFamilyPriority priority) noexcept
{
    size_t idx = NPOS;
    VkQueueFlags min_flags = VK_QUEUE_FLAG_BITS_MAX_ENUM;
    for(size_t i = 0; i < queue_families.size(); i++)
    {
        // check if the queue family has required properties
        uint32_t failed = 0;
        failed |= detail::queue::has_flags(queue_families[i], filter.queueFlags);
        failed |= detail::queue::has_count(queue_families[i], filter.queueCount);

        // every check was successful
        if (failed == 0)
        {
            if (priority == VKA_QUEUE_FAMILY_PRIORITY_FIRST)
                return i;
            if (priority == VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL)
            {
                // If there are multiple options, searches for the queue family with the "least power".
                // This means with the queue family which has the least number of additional flags set as specified
                // in the filter.
                const VkQueueFlags cur_flags = queue_families[i].queueFlags ^ filter.queueFlags;
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
