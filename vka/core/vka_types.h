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
        VKA_QUEUE_FAMILY_PRIORITY_FIRST = 0,
        VKA_QUEUE_FAMILY_PRIORITY_OPTIMAL = 1,
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

#ifdef VKA_STB_IMAGE_LOAD_ENABLE
    enum ImageDataType
    {
        VKA_IMAGE_DATA_TYPE_INT8,
        VKA_IMAGE_DATA_TYPE_INT16,
        VKA_IMAGE_DATA_TYPE_FLOAT
    };
#endif

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
        uint32_t queueOffset;
    };

    struct VertexAttribute
    {
        VertexAttributeType type;
        size_t              spacing;
    };

    struct AttachmentImageCreateInfo
    {
        VkFormat                imageFormat;
        VkExtent2D              imageExtent;
        VkSampleCountFlagBits   imageSamples;
        VkImageUsageFlags       imageUsage;
        VkSharingMode           imageSharingMode;
        uint32_t                imageQueueFamilyIndexCount;
        const uint32_t*         imageQueueFamilyIndices;
        VkFormat                viewFormat;
        VkComponentMapping      viewComponentMapping;
        VkImageAspectFlags      viewAspectMask;
    };

    // see documentation of VkBufferCreateInfo and VkMemoryAllocateInfo
    struct BufferCreateInfo
    {
        VkBufferCreateFlags     bufferFlags;
        VkDeviceSize            bufferSize;
        VkBufferUsageFlags      bufferUsage;
        VkSharingMode           bufferSharingMode;
        uint32_t                bufferQueueFamilyIndexCount;
        const uint32_t*         bufferQueueFamilyIndices;
        VkMemoryPropertyFlags   memoryPropertyFlags;
    };

    // see documentation of VkImageCreateInfo and VkSamplerCreateInfo
    struct TextureCreateInfo
    {
        VkImageCreateFlags      imageFlags;
        VkImageType             imageType;
        VkFormat                imageFormat;
        VkExtent3D              imageExtent;
        uint32_t                imageArrayLayers;
        uint32_t                imageQueueFamilyIndexCount;
        const uint32_t*         imageQueueFamilyIndices;
        VkFilter                samplerMagFilter;
        VkFilter                samplerMinFilter;
        VkSamplerMipmapMode     samplerMipmapMode;
        VkSamplerAddressMode    samplerAddressModeU;
        VkSamplerAddressMode    samplerAddressModeV;
        VkSamplerAddressMode    samplerAddressModeW;
        float                   samplerLodBias;
        uint32_t                samplerAnisotropyEnable;
        float                   samplerMaxAnisotropy;
        uint32_t                samplerCompareEnable;
        VkCompareOp             samplerCompareOp;
        float                   samplerMinLod;
        float                   samplerMaxLod;
        VkBorderColor           samplerBorderColor;
        uint32_t                samplerUnnormalizedCoordinates;
        bool                    generateMipMap;
    };

    // image view create info for texture, see documentation of VkImageViewCreateInfo
    struct TextureViewCreateInfo
    {
        VkImageViewCreateFlags  flags;
        VkImageViewType         viewType;
        VkFormat                format;
        VkComponentMapping      components;
        uint32_t                baseArrayLayer;
        uint32_t                layerCount;
    };
}