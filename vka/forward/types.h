/**
 * @brief Includes types used by vka.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
#ifdef VKA_MODEL_LOADING_ENABLE
    using real_t = tinyobj::real_t;
#endif

    using null_handle_t = decltype(VK_NULL_HANDLE);

    using ColorFormatArray = std::array<VkFormat, 123>;
    using DepthFormatArray = std::array<VkFormat, 6>;
    using StencilFormatArray = std::array<VkFormat, 4>;
    using DepthStencilFormatArray = std::array<VkFormat, 3>;

    enum class QueueFamilyPriority : uint8_t
    {
        FIRST,
        OPTIMAL,
    };

    enum class VertexAttributeType : uint32_t // deprecated
    {
        NONE = 0,
        POSITION = 1,
        NORMAL = 3,
        TEXTURE_COORDINATE = 4,
        POSITION_W_EXT = 100,
        TEXTURE_COORDINATE_W_EXT = 101,
        COLOR_EXT = 102,
    };

    enum class ModelLoadOptionFlagBits : uint32_t // deprecated
    {
        DEFAULT = 0x00000000,
        IGNORE_MATERIAL = 0x00000001,
        FORCE_PER_MESH_MATERIAL = 0x00000002,
    };
    using ModelLoadOptionFlags = uint32_t;

    struct PhysicalDeviceRequirements
    {
        VkPhysicalDeviceType            type;
        const VkMemoryPropertyFlags*    memoryPropertyFlags;
        uint32_t                        memoryPropertyFlagsCount;
        const VkQueueFlags*             queueFamilyFlags;
        uint32_t                        queueFamilyFlagsCount;
        bool                            surfaceSupport;
        const char*                     sequence;
    };

    struct QueueFamilyRequirements
    {
    	VkQueueFlags    queueFlags;
    	uint32_t        queueCount;
    };

    struct VertexAttribute // deprecated
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
        const void*             pBufferNext;
        VkBufferCreateFlags     bufferFlags;
        VkDeviceSize            bufferSize;
        VkBufferUsageFlags      bufferUsage;
        VkSharingMode           bufferSharingMode;
        uint32_t                bufferQueueFamilyIndexCount;
        const uint32_t*         bufferQueueFamilyIndices;
        const void*             pMemoryNext;
        VkMemoryPropertyFlags   memoryPropertyFlags;
    };

    // image view create-info for texture, see documentation of VkImageViewCreateInfo
    struct TextureViewCreateInfo
    {
        VkImageViewCreateFlags  flags;
        VkImageViewType         viewType;
        VkFormat                format;
        VkComponentMapping      components;
        uint32_t                baseArrayLayer;
        uint32_t                layerCount;
    };

    // see documentation of VkImageCreateInfo and VkSamplerCreateInfo
    struct TextureCreateInfo
    {
        VkImageCreateFlags              imageFlags;
        VkImageType                     imageType;
        VkFormat                        imageFormat;
        VkExtent3D                      imageExtent;
        uint32_t                        imageArrayLayers;
        uint32_t                        imageQueueFamilyIndexCount;
        const uint32_t*                 imageQueueFamilyIndices;
        VkFilter                        samplerMagFilter;
        VkFilter                        samplerMinFilter;
        VkSamplerMipmapMode             samplerMipmapMode;
        VkSamplerAddressMode            samplerAddressModeU;
        VkSamplerAddressMode            samplerAddressModeV;
        VkSamplerAddressMode            samplerAddressModeW;
        float                           samplerLodBias;
        uint32_t                        samplerAnisotropyEnable;
        float                           samplerMaxAnisotropy;
        uint32_t                        samplerCompareEnable;
        VkCompareOp                     samplerCompareOp;
        float                           samplerMinLod;
        float                           samplerMaxLod;
        VkBorderColor                   samplerBorderColor;
        uint32_t                        samplerUnnormalizedCoordinates;
        uint32_t                        viewCount;
        const TextureViewCreateInfo*    views;
        bool                            generateMipMap;
        VkCommandBuffer                 commandBuffer;
    };

    /**
     * Contains information for staging buffers of texture loaders.
     * - <c>queueFamilyIndex</c>: Specifies the queue family of the staging buffer.
     * - <c>memoryPropertyFlags</c>: Specify memory property flags that are mappable.
     * - <c>memoryProperties</c>: Specifies the memory properties of the used device.
     */
    struct TextureLoadInfo
    {
        uint32_t                                queueFamilyIndex;
        VkMemoryPropertyFlags                   memoryPropertyFlags;
        const VkPhysicalDeviceMemoryProperties* memoryProperties;
    };
}
