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
     * Checks requirements for a queue family.
     * @param requirements Requirements to check for.
     * @param properties Properties of the queue family to check.
     * @return Returns <c>true</c> if all requirements are satisfied.
     */
    inline bool check_requirements(QueueFamilyRequirements requirements, const VkQueueFamilyProperties& properties) noexcept;
} // namespace vka::detail::queue
