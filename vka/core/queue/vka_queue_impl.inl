/**
* @file     vka_queue_impl.h
* @brief    Implemenation of queue functions.
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
    queue_families.clear();
    
    uint32_t n_queue_families;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &n_queue_families, nullptr);
    VkQueueFamilyProperties _queue_families[n_queue_families];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &n_queue_families, _queue_families);
    
    for (size_t i = 0; i < n_queue_families; i++)
        queue_families.push_back(_queue_families[i]);
}

size_t vka::queue::find(const std::vector<VkQueueFamilyProperties>& queue_families, size_t begin, const QueueFamilyFilter& filter, QueueFamilyPriority priority)
{
    if (begin >= queue_families.size()) return VKA_NPOS;
    
    std::vector<size_t> candidates;
    for(size_t i = 0; i < queue_families.size(); i++)
    {
        uint16_t failed = 0x0000;
        failed |= detail::queue::has_flags(queue_families.at(i), filter.queueFlags);
        failed |= detail::queue::has_count(queue_families.at(i), filter.queueCount);
        if(failed == 0x0000) candidates.push_back(i);
    }

    if(candidates.size() == 0) return VKA_NPOS;

    size_t idx = VKA_NPOS;
    if(priority == VKA_QUEUE_FAMILY_PRIORITY_FIRST)
    {
        idx = candidates.at(0);
    }
    else if(priority == VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL)
    {
        VkQueueFlags min_flags = VK_QUEUE_FLAG_BITS_MAX_ENUM;
        for(size_t i : candidates)
        {
            const VkQueueFlags cur_flags = queue_families.at(i).queueFlags ^ filter.queueFlags;
            if(cur_flags < min_flags)
            {
                min_flags = cur_flags;
                idx = i;
            }
        }
    }
    return idx;
}

bool vka::queue::validate(const std::vector<VkQueueFamilyProperties>& queue_families, const std::vector<QueueInfo>& queue_infos)
{
    std::vector<QueueInfo> maximum_queue_infos;
    for (size_t i = 0; i < queue_families.size(); i++)
        maximum_queue_infos.push_back({ static_cast<uint32_t>(i), 0, 0 });
    
    for (size_t i = 0; i < queue_infos.size(); i++)
    {
        maximum_queue_infos.at(queue_infos.at(i).queueFamilyIndex).usedQueueCount += queue_infos.at(i).usedQueueCount;
        if (queue_families.at(queue_infos.at(i).queueFamilyIndex).queueCount < (queue_infos.at(i).queueBaseIndex + queue_infos.at(i).usedQueueCount))
            return false;
    }
    
    for (size_t i = 0; i < queue_families.size(); i++)
    {
        if (maximum_queue_infos.at(i).usedQueueCount > queue_families.at(i).queueCount)
            return false;
    }
    return true;
}
