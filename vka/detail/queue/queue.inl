/**
 * @brief Implementation details for the queue functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "queue.h"

inline bool vka::detail::queue::check_requirements(QueueFamilyRequirements requirements, const VkQueueFamilyProperties& properties) noexcept
{
    if ((properties.queueFlags & requirements.queueFlags) != requirements.queueFlags) return false;
    if (properties.queueCount < requirements.queueCount) return false;
    return true;
}
