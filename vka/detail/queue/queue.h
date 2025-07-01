/**
 * @brief Includes implementation details for the queue functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::detail::queue
{
    /**
     * @brief Checks if a combination of queue flags is supported.
     * @param properties Specifies the memory properties.
     * @param req_flags Specifies the required queue flags.
     * @return Returns 0x00 if all flag combinations are supported and 0x01, if not.
     */
    inline uint8_t has_flags(const VkQueueFamilyProperties& properties, VkQueueFlags req_flags) noexcept;

    /**
     * @brief Checks if a specific queue count is supported.
     * @param properties Specifies the memory properties.
     * @param req_count Specifies the required queue count.
     * @return Returns 0x00 if all flag combinations are supported and 0x02, if not.
     */
    inline uint8_t has_count(const VkQueueFamilyProperties& properties, uint32_t req_count) noexcept;
} // namespace vka::detail::queue

#include "queue.inl"
