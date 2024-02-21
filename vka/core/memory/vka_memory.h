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
    /**
     * @brief Searches for a supported memory-type index.
     * @details For every supported memory-type of an object, a bit is set in a bit-mask. The index of a bit in the mask
     * corresponds to a memory-type in the memory-properties of a physical device. This function checks, if a memory-
     * type is available for an object (bit-mask) that supports certain property flags.
     * @param properties Specifies the memory properties of a physical device.
     * @param bits Specifies the memory-type bit mask.
     * @param req_flags Specifies the required memory property flags.
     * @return Returns the index of the memory type, if the required flags are supported. Otherwise, vka::NPOS32 is
     * returned.
     */
    uint32_t find_type_index(const VkPhysicalDeviceMemoryProperties& properties, uint32_t bits, VkMemoryPropertyFlags req_flags) noexcept;
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_memory_impl.inl"
#endif
