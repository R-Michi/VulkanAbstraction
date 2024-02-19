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
    /**
     * @brief Queries all queue families (actually their properties) from a physical device.
     * @param device Specifies a valid physical device.
     * @param queue_families Specifies a vector in which to store all queue family properties.
     */
    void properties(const VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queue_families);

    /**
     * @brief Searches for queue family which supports the requirements for the program.
     * @param queue_families Specifies all available queue family properties.
     * @param filter Specifies the requirements for the program.
     * @param priority Specifies a priority for the search operation.\n
     *  - VKA_QUEUE_FAMILY_PRIORITY_FIRST Chooses the first queue family that meets the requirements.\n
     *  - VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL Chooses the queue family which has the least amount of additional queue\n
     *  family flags to those specified in the filter, if multiple queue families meet the requirements. In other words,
     *  if multiple queue families satisfy the requirements, the queue family with the "least power" is chosen.
     * @return Returns the index of the found queue family or vka::NPOS, if no queue family could be found.
     */
    size_t find(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueFamilyFilter& filter, QueueFamilyPriority priority) noexcept;

    /**
     * @brief This function validates a QueueInfo structure.
     * @details A queue info structure specifies a range of queues of a queue family. Finding a queue family only
     * guarantees that this family has the required number of queues. However, it does not guarantee that a range of
     * queues is within the bounds of defined (valid) queue-indices. This function validates that the range of queue-
     * indices specified in a QueueInfo is in the bounds of the defined queue-index range.
     * @param queue_families Specifies all available queue family properties.
     * @param info Specifies the QueueInfo structure to check.
     * @return Returns 'true', if the validation was successful and 'false', if the QueueInfo structure contains any
     * invalid queue-indices.
     */
    inline bool validate(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueInfo& info) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_queue_impl.inl"
#endif
#include "vka_queue_inline_impl.inl"
