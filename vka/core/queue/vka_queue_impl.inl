/**
* @file     vka_queue_impl.h
* @brief    Implementation of queue functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

void vka::queue::properties(const VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queue_families)
{
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
    queue_families.resize(count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queue_families.data());
}

size_t vka::queue::find(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueFamilyFilter& filter, QueueFamilyPriority priority) noexcept
{
    size_t idx = vka::NPOS;
    VkQueueFlags min_flags = VK_QUEUE_FLAG_BITS_MAX_ENUM;
    for(size_t i = 0; i < queue_families.size(); i++)
    {
        // check, if queue family has required properties
        uint32_t failed = 0;
        failed |= detail::queue::has_flags(queue_families[i], filter.queueFlags);
        failed |= detail::queue::has_count(queue_families[i], filter.queueCount);

        // every check was successful
        if (failed == 0)
        {
            if (priority == VKA_QUEUE_FAMILY_PRIORITY_FIRST)
                return i;
            else if(priority == VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL)
            {
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
