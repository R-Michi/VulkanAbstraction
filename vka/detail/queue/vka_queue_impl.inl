/**
* @file     vka_queue_impl.inl
* @brief    Detailed implementation of queue functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

uint16_t vka::detail::queue::has_flags(const VkQueueFamilyProperties& properties, VkQueueFlags req_flags)
{
    return (((properties.queueFlags & req_flags) != req_flags) ? 0x0001 : 0x0000);
}
uint16_t vka::detail::queue::has_count(const VkQueueFamilyProperties& properties, uint32_t req_count)
{
    return ((properties.queueCount < req_count) ? 0x0002 : 0x0000);
}
