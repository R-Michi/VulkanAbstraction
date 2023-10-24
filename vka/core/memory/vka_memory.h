/**
* @file     vka_memory.h
* @brief    Functionality regarding memory and memory properties.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka::memory
{
    /*
    * Searches for a supported memory type index for a given memoryTypeBits bit-mask in the
    * physical device's memory types. The memory properties that contains all supported memory
    * types of the physical device is specified by 'properties'. The memory specific memory
    * types is given by a bit mask and specified by 'bits'. For every available memory type a
    * bit is set in the mask at the corresponding index of the memory type. This mask can be
    * queried from VkMemoryRequirements::memoryTypeBits. The memory type flags that are
    * required are specified by 'req_flags'. If the required flags are supported by one memory
    * type, the index of the memory type is returned. Otherwise, if the flags are not
    * supported, vka::NPOS32 is returned.
    */
    uint32_t find_type_index(const VkPhysicalDeviceMemoryProperties& properties, uint32_t bits, VkMemoryPropertyFlags req_flags) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_memory_impl.inl"
#endif
