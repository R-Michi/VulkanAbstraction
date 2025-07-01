/**
 * @brief Implementation details for the queue functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

inline uint8_t vka::detail::queue::has_flags(const VkQueueFamilyProperties& properties, VkQueueFlags req_flags) noexcept
{
    return (properties.queueFlags & req_flags) != req_flags ? 0x01 : 0;
}

inline uint8_t vka::detail::queue::has_count(const VkQueueFamilyProperties& properties, uint32_t req_count) noexcept
{
    return properties.queueCount < req_count ? 0x02 : 0;
}
