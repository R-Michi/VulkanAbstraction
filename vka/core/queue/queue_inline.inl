/**
 * @brief Inline implementation for the queue functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

inline bool vka::queue::validate(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueInfo& info) noexcept
{
    return (info.queueOffset + info.usedQueueCount) <= queue_families[info.queueFamilyIndex].queueCount;
}
