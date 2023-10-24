/**
* @file     vka_queue.h
* @brief    Useful queue functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../detail/queue/vka_queue.h"

namespace vka::queue
{
    /*
    * Stores all queue family properties of a physical device in 'queue_families'.
    * The physical device is specified by 'device'.
    */
    void properties(const VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queue_families);

    /*
    * Searches for the best matching queue family with a given filter and priority. All the
    * queue family properties used for the search operation is specified by 'queue_families'.
    * The filter is specified by 'filter' and the priority by 'priority'. Returns the index of
    * the best matching queue family in the 'queue_families' array.
    */
    size_t find(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueFamilyFilter& filter, QueueFamilyPriority priority) noexcept;

    /*
    * This function validates a queue info structure. A queue info structure consists of a
    * range of queues of a certain queue family. The range consists of an offset
    * (QueueInfo::queueOffset) and an amount (QueueInfo::usedQueueCount). This function checks,
    * if the range exceeds the maximum number of queues of the family specified in the
    * QueueInfo structure (QueueInfo::queueFamilyIndex). Checks, if:
    * 'offset' + 'amount' <= 'maximum queue count'. Returns true, if the validation was
    * successful and false otherwise.
    */
    inline bool validate(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueInfo& info) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_queue_impl.inl"
#endif
#include "vka_queue_inline_impl.inl"
