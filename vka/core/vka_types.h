/**
* @file     vka_types.h
* @brief    Basic type definitions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "vka_global.h"

namespace vka
{
#ifdef VKA_MODEL_LOADING_ENABLE
    using real_t = tinyobj::real_t;
#endif

    enum QueueFamilyPriority : uint32_t
    {
        VKA_QUEUE_FAMILY_PRIORITY_FIRST = 0x00000001,
        VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL = 0x00000002,
        VKA_QUEUE_FAMILY_PRIORITY_MAX_ENUM = 0x7FFFFFFF
    };

    enum VertexAttributeType : uint32_t
    {
        VKA_VERTEX_ATTRIBUTE_TYPE_NONE = 0,
        VKA_VERTEX_ATTRIBUTE_TYPE_POSITION = 1,
        VKA_VERTEX_ATTRIBUTE_TYPE_NORMAL = 3,
        VKA_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATE = 4,
        VKA_VERTEX_ATTRIBUTE_TYPE_POSITION_W_EXT = 100,
        VKA_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATE_W_EXT = 101,
        VKA_VERTEX_ATTRIBUTE_TYPE_COLOR_EXT = 102,
        VKA_VERTEX_ATTRIBUTE_TYPE_MAX_ENUM = 0x7FFFFFFF
    };

    enum ModelLoadOptionFlagBits : uint32_t
    {
        VKA_MODEL_LOAD_OPTION_DEFAULT = 0x00000000,
        VKA_MODEL_LOAD_OPTION_IGNORE_MATERIAL = 0x00000001,
        VKA_MODEL_LOAD_OPTION_FORCE_PER_MESH_MATERIAL = 0x00000002,
        VKA_MODEL_LOAD_OPTION_MAX_ENUM = 0x7FFFFFFF
    };
    using ModelLoadOptionFlags = uint32_t;

    struct PhysicalDeviceFilter
    {
        const char*                         sequence;
        std::vector<VkMemoryPropertyFlags>  memoryPropertyFlags;
        std::vector<VkPhysicalDeviceType>   deviceTypeHirachy;
        std::vector<VkQueueFlags>           queueFamilyFlags;
        bool                                surfaceSupport;
    };

    struct QueueFamilyFilter
    {
    	VkQueueFlags    queueFlags;
    	uint32_t        queueCount;
    };

    struct QueueInfo
    {
        uint32_t queueFamilyIndex;
        uint32_t usedQueueCount;
        uint32_t queueBaseIndex;
    };

    struct VertexAttribute
    {
        VertexAttributeType type;
        size_t              spacing;
    };
}