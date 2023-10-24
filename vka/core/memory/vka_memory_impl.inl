/**
* @file     vka_memory_impl.inl
* @brief    Implementation of memory functionality.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

uint32_t vka::memory::find_type_index(const VkPhysicalDeviceMemoryProperties& properties, uint32_t bits, VkMemoryPropertyFlags req_flags) noexcept
{
    // If a memory type is supported, the corresponding bit is set in the memoryTypeBit bit mask.
    // This bit mask is specified by 'bits'. This loop iterates over all set bits in the mask and
    // checks, if the required flags 'req_flags' are contained in the memoryType's propertyFlags.
    // If this is the case, the index of the memory type is returned.
    for (uint32_t i = 0; i < properties.memoryTypeCount; i++)
    {
        if ((bits & (0b1 << i)) && (properties.memoryTypes[i].propertyFlags & req_flags) == req_flags)
            return i;
    }
    return vka::NPOS32;
}
