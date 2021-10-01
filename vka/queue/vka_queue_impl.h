/**
* @file     vka_queue_impl.h
* @brief    Implemenation of queue related functions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "vka_queue.h"

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

bool queue_family_has_flags(const VkQueueFamilyProperties& properties, VkQueueFlags req_flags)
{
    return ((properties.queueFlags & req_flags) == req_flags);
}

bool queue_family_has_count(const VkQueueFamilyProperties& properties, uint32_t req_count)
{
    return properties.queueCount >= req_count;
}

vka::QueueFamilyError vka::queue::find(const std::vector<VkQueueFamilyProperties>& queue_families, size_t begin, const QueueFamilyFilter& filter, QueueFamilyPriority priority, size_t& index)
{
    using namespace vka;
    
    if (begin >= queue_families.size()) return VKA_QUEUE_FAMILY_ERROR_INVALID_PARAMETER;
    
    std::vector<size_t> matching_indices;
    QueueFamilyError error;
    uint32_t error_mask = 0;
    
    for (size_t i = 0; i < queue_families.size(); i++)
    {
        if (!queue_family_has_flags(queue_families[i], filter.reqQueueFlags))
        {
            if (!(error_mask & VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED))
                error = VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED;
            continue;
        }
        else
        {
            error_mask |= VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED;
        }
        
        if (!queue_family_has_count(queue_families[i], filter.reqQueueCount))
        {
            if (!(error_mask & VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED))
                error = VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED;
            continue;
        }
        else
        {
            error_mask |= VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED;
        }
        
        matching_indices.push_back(i);
        error = VKA_QUEUE_FAMILY_ERROR_NONE;
    }
    
    if (error != VKA_QUEUE_FAMILY_ERROR_NONE)
        return error;
    
    if (priority == VKA_QUEUE_FAMILY_PRIORITY_FIRST)
    {
        index = matching_indices.at(0);
    }
    else if (priority == VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL)
    {
        size_t min_idx = 0;
        VkQueueFlags min_current_xor_requiered = queue_families.at(matching_indices.at(0)).queueFlags ^ filter.reqQueueFlags;
        
        for (size_t i = 1; i < matching_indices.size(); i++)
        {
            const VkQueueFlags current_xor_requiered = queue_families.at(matching_indices.at(i)).queueFlags ^ filter.reqQueueFlags;
            
            if (current_xor_requiered < min_current_xor_requiered)
            {
                min_current_xor_requiered = current_xor_requiered;
                min_idx = matching_indices.at(i);
            }
        }
        
        index = min_idx;
    }
    else
    {
        return VKA_QUEUE_FAMILY_ERROR_INVALID_PARAMETER;
    }
    
    return VKA_QUEUE_FAMILY_ERROR_NONE;
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

const char* vka::queue::strerror(QueueFamilyError error)
{
    using namespace vka;
    
    switch (error)
    {
    case VKA_QUEUE_FAMILY_ERROR_NONE:
        return "No error occured!";
    case VKA_QUEUE_FAMILY_ERROR_INVALID_PARAMETER:
        return "Invalid parameter!";
    case VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED:
        return "No queue family found with requiered flags!";
    case VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED:
        return "No queue family found with requiered queue count!";
    default:
        return "Unknown error!";
    }
}
