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

namespace vka
{
    namespace queue
    {
        /**
        * @brief                        Get all queue family properties of a physical device.
        * @param[in]  device            physical device
        * @param[out] queue_families    queue family properties
        */
        void properties(const VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queue_families);

        /**
        * @brief                        Finds the best matching queue family index.
        * @param[in]  queue_families    queue family properties, MUST contain all to work properly
        * @param[in]  begin             begin index for searching
        * @param[in]  filter            queue family filter
        * @param[in]  priority          search priority
        * @return                       Index of the best matching queue familiy within the queue family property vector.
        */
        size_t find(const std::vector<VkQueueFamilyProperties>& queue_families, size_t begin, const QueueFamilyFilter& filter, QueueFamilyPriority priority);

        /**
        * @brief                        Checks if the used number of queues does not exceed the maximum queue count for a family.
        * @param[in] queue_families     aviable queue families, MUST contain all!
        * @param[in] queue_infos        queue infos
        * @return                       Boolean wether validation was successful.
        */
        bool validate(const std::vector<VkQueueFamilyProperties>& queue_families, const std::vector<QueueInfo>& queue_infos);
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_queue_impl.inl"
#endif
