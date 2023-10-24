/**
* @file     vka_queue_inline_impl.h
* @brief    Implementation of inline queue functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline bool vka::queue::validate(const std::vector<VkQueueFamilyProperties>& queue_families, const QueueInfo& info) noexcept
{
    return ((info.queueOffset + info.usedQueueCount) <= queue_families[info.queueFamilyIndex].queueCount);
}
