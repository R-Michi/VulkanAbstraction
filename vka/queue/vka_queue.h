/**
* @file     vka_queue.h
* @brief    Useful queue related functions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../vka_global.h"
#include "../vka_types.h"

namespace vka
{
    namespace queue
    {
        /**
        * @brief Get all queue family properties of a physical device.
        * @param[in] device: physical device
        * @param[out] queue_families: queue family properties
        */
        void properties(const VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& queue_families);

        /**
        * @brief Finds the best matching physical queue family index.
        * @param[in] queue_families: queue family properties, MUST contain all to work properly
        * @param[in] begin: begin index for searching
        * @param[in] filter: queue family filter
        * @param[in] priority: search priority
        * @param[out] index: index of the best matching property of the queue family vector @param queue_families,
        *   corresponds to the queue family index
        */
        QueueFamilyError find(const std::vector<VkQueueFamilyProperties>& queue_families, size_t begin, const QueueFamilyFilter& filter, QueueFamilyPriority priority, size_t& index);

        /**
        * @brief Converts QueueFamiliyError enum to string.
        * @param[in] error: error code
        * @return string of the error code
        */
        const char* strerror(QueueFamilyError error);

        /**
        * @brief Checks if the used number of queues does not exceed the maximum queue count for that family.
        * @param[in] queue_families: aviable queue families, MUST contain all!
        * @param[in] queue_infos: queue infos
        * @return 'true' if the validation was successful, 'false' if validation was not successful
        */
        bool validate(const std::vector<VkQueueFamilyProperties>& queue_families, const std::vector<QueueInfo>& queue_infos);
    }
}