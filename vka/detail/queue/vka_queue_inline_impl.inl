/**
* @file     vka_queue_impl.inl
* @brief    Detailed implementation of inline queue functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline uint32_t vka::detail::queue::has_flags(const VkQueueFamilyProperties& properties, VkQueueFlags req_flags) noexcept
{
    return (((properties.queueFlags & req_flags) != req_flags) ? 0xFFFFFFFF : 0);
}

inline uint32_t vka::detail::queue::has_count(const VkQueueFamilyProperties& properties, uint32_t req_count) noexcept
{
    return ((properties.queueCount < req_count) ? 0xFFFFFFFF : 0);
}
