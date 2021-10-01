/**
* @file     vka_types.h
* @brief    Basic type definitions.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "vka_global.h"

namespace vka
{
    struct PhysicalDeviceFilter
    {
        const char*                         sequence;
        std::vector<VkMemoryPropertyFlags>  reqMemoryPropertyFlags;
        std::vector<VkPhysicalDeviceType>   reqDeviceTypeHirachy;
        const VkSurfaceKHR*                 pSurface;
        uint32_t                            reqMinImageCount;
        uint32_t                            reqMaxImageCount;
        VkImageUsageFlags                   reqSurfaceImageUsageFlags;
        std::vector<VkFormat>               reqSurfaceColorFormats;
        std::vector<VkColorSpaceKHR>        reqSurfaceColorSpaces;
        std::vector<VkPresentModeKHR>       reqPresentModes;
        std::vector<VkQueueFlags>           reqQueueFamilyFlags;
    };

    struct QueueFamilyFilter
    {
    	VkQueueFlags    reqQueueFlags;
    	uint32_t        reqQueueCount;
    };

    struct QueueInfo
    {
        uint32_t queueFamilyIndex;
        uint32_t usedQueueCount;
        uint32_t queueBaseIndex;
    };

    enum PhysicalDeviceError : uint32_t
    {
        VKA_PYHSICAL_DEVICE_ERROR_NONE = 0x00000000,
        VKA_PYHSICAL_DEVICE_ERROR_SEQUENCE_FAILED = 0x00000001,
        VKA_PYHSICAL_DEVICE_ERROR_MEMORY_PROPERTIES_FAILED = 0x00000002,
        VKA_PYHSICAL_DEVICE_ERROR_MIN_IMAGE_COUNT_FAILED = 0x00000004,
        VKA_PYHSICAL_DEVICE_ERROR_MAX_IMAGE_COUNT_FAILED = 0x0000008,
        VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_USAGE_FAILED = 0x00000010,
        VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_FORMATS_FAILED = 0x00000020,
        VKA_PYHSICAL_DEVICE_ERROR_SURFACE_IMAGE_COLOR_SPACE_FAILED = 0x00000040,
        VKA_PYHSICAL_DEVICE_ERROR_PRESENT_MODE_FAILD = 0x0000080,
        VKA_PYHSICAL_DEVICE_ERROR_QUEUE_FLAGS_FAILED = 0x00000100,
        VKA_PYHSICAL_DEVICE_ERROR_DEVICE_TYPE_FAILED = 0x00000200,
        VKA_PYHSICAL_DEVICE_ERROR_INVALID_PARAMETER = 0x0000400,
        VKA_PYHSICAL_DEVICE_ERROR_MAX_ENUM = 0x7FFFFFFF
    };

    enum QueueFamilyPriority
    {
        VKA_QUEUE_FAMILY_PRIORITY_FIRST = 0x1,
        VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL = 0x2,
        VKA_QUEUE_FAMILY_PRIORITY_MAX_ENUM = 0x7FFFFFFF
    };

    enum QueueFamilyError : uint32_t
    {
        VKA_QUEUE_FAMILY_ERROR_NONE = 0x00000000,
        VKA_QUEUE_FAMILY_ERROR_QUEUE_FLAGS_FAILED = 0x00000001,
        VKA_QUEUE_FAMILY_ERROR_QUEUE_COUNT_FAILED = 0x00000002,
        VKA_QUEUE_FAMILY_ERROR_INVALID_PARAMETER = 0x00000004,
        VKA_QUEUE_FAMILY_ERROR_MAX_ENUM = 0x7FFFFFFF
    };
}