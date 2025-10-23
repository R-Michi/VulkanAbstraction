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
     * @brief Checks all queue family requirements.
     * @param requirements Specifies the requirements.
     * @param properties Specifies the properties of the queue family.
     * @return Returns true if all requirements are satisfied.
     */
    inline bool check_requirements(QueueFamilyRequirements requirements, const VkQueueFamilyProperties& properties) noexcept;
} // namespace vka::detail::queue
