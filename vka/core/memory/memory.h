/**
 * @brief Helper functions for creating vulkan memory.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::memory
{
    /**
     * Searches for a supported memory-type index.
     * @param properties Memory properties of the physical device.
     * @param bits Memory-type bit mask.
     * @param req_flags Required memory property flags.
     * @return Returns the index of the memory type if the required flags are supported. Otherwise, <c>vka::NPOS</c> is
     * returned.
     */
    uint32_t find_type_index(const VkPhysicalDeviceMemoryProperties& properties, uint32_t bits, VkMemoryPropertyFlags req_flags) noexcept;
}
