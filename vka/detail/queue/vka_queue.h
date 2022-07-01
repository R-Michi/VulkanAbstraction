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
            uint16_t has_flags(const VkQueueFamilyProperties& properties, VkQueueFlags req_flags);
            uint16_t has_count(const VkQueueFamilyProperties& properties, uint32_t req_count);
        } // namespace queue  
    } // namespace detail 
} // namespace vka

#ifdef VKA_IMPLEMENTATION
    #include "vka_queue_impl.inl"
#endif
