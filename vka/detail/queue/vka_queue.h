/**
* @file     vka_queue.h
* @brief    Detailed declaration of queue functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka
{
    namespace detail
    {
        namespace queue
        {
            inline uint32_t has_flags(const VkQueueFamilyProperties& properties, VkQueueFlags req_flags) noexcept;
            inline uint32_t has_count(const VkQueueFamilyProperties& properties, uint32_t req_count) noexcept;
        } // namespace queue  
    } // namespace detail 
} // namespace vka

#include "vka_queue_inline_impl.inl"
