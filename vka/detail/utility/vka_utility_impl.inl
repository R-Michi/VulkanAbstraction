/**
* @file     vka_utility_impl.inl
* @brief    Detailed implementation of utility functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

VkImageUsageFlagBits vka::detail::utility::format_feature_to_image_usage_bit(VkFormatFeatureFlagBits format_feature)
{
    switch (format_feature)
    {
    case VK_FORMAT_FEATURE_TRANSFER_SRC_BIT:
        return VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    case VK_FORMAT_FEATURE_TRANSFER_DST_BIT:
        return VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    case VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT:
        return VK_IMAGE_USAGE_SAMPLED_BIT;
    case VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT:
        return VK_IMAGE_USAGE_STORAGE_BIT;
    case VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT:
        return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    case VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT:
        return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    case VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR:
        return VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV;
    case VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT:
        return VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT;
    case VK_FORMAT_FEATURE_FLAG_BITS_MAX_ENUM:
        return VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM;
    default:
        return static_cast<VkImageUsageFlagBits>(0);
    };
    return static_cast<VkImageUsageFlagBits>(0);
}

VkFormatFeatureFlagBits vka::detail::utility::image_usage_to_format_feature_bit(VkImageUsageFlagBits image_usage)
{
    switch (image_usage)
    {
    case VK_IMAGE_USAGE_TRANSFER_SRC_BIT:
        return VK_FORMAT_FEATURE_TRANSFER_SRC_BIT;
    case VK_IMAGE_USAGE_TRANSFER_DST_BIT:
        return VK_FORMAT_FEATURE_TRANSFER_DST_BIT;
    case VK_IMAGE_USAGE_SAMPLED_BIT:
        return VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT;
    case VK_IMAGE_USAGE_STORAGE_BIT:
        return VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT;
    case VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT:
        return VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT;
    case VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT:
        return VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    case VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV:
        return VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
    case VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT:
        return VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT;
    case VK_IMAGE_USAGE_FLAG_BITS_MAX_ENUM:
        return VK_FORMAT_FEATURE_FLAG_BITS_MAX_ENUM;
    default:
        return static_cast<VkFormatFeatureFlagBits>(0);
    
    };
    return static_cast<VkFormatFeatureFlagBits>(0);
}

std::map<VkFormat, size_t> vka::detail::utility::init_format_sizeof(void) noexcept
{
    std::map<VkFormat, size_t> f2s;

    f2s[VK_FORMAT_UNDEFINED] = 0;
    f2s[VK_FORMAT_R4G4_UNORM_PACK8] = 1;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R4G4B4A4_UNORM_PACK16); i <= static_cast<uint32_t>(VK_FORMAT_A1R5G5B5_UNORM_PACK16); i++)
        f2s[static_cast<VkFormat>(i)] = 2;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R8_UNORM); i <= static_cast<uint32_t>(VK_FORMAT_R8_SRGB); i++)
        f2s[static_cast<VkFormat>(i)] = 1;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R8G8_UNORM); i <= static_cast<uint32_t>(VK_FORMAT_R8G8_SRGB); i++)
        f2s[static_cast<VkFormat>(i)] = 2;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R8G8B8_UNORM); i <= static_cast<uint32_t>(VK_FORMAT_B8G8R8_SRGB); i++)
        f2s[static_cast<VkFormat>(i)] = 3;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R8G8B8A8_UNORM); i <= static_cast<uint32_t>(VK_FORMAT_A2B10G10R10_SINT_PACK32); i++)
        f2s[static_cast<VkFormat>(i)] = 4;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R16_UNORM); i <= static_cast<uint32_t>(VK_FORMAT_R16_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 2;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R16G16_UNORM); i <= static_cast<uint32_t>(VK_FORMAT_R16G16_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 4;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R16G16B16_UNORM); i <= static_cast<uint32_t>(VK_FORMAT_R16G16B16_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 6;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R16G16B16A16_UNORM); i <= static_cast<uint32_t>(VK_FORMAT_R16G16B16A16_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 8;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R32_UINT); i <= static_cast<uint32_t>(VK_FORMAT_R32_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 4;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R32G32_UINT); i <= static_cast<uint32_t>(VK_FORMAT_R32G32_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 8;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R32G32B32_UINT); i <= static_cast<uint32_t>(VK_FORMAT_R32G32B32_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 12;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R32G32B32A32_UINT); i <= static_cast<uint32_t>(VK_FORMAT_R32G32B32A32_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 16;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R64_UINT); i <= static_cast<uint32_t>(VK_FORMAT_R64_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 8;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R64G64_UINT); i <= static_cast<uint32_t>(VK_FORMAT_R64G64_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 16;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R64G64B64_UINT); i <= static_cast<uint32_t>(VK_FORMAT_R64G64B64_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 24;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_R64G64B64A64_UINT); i <= static_cast<uint32_t>(VK_FORMAT_R64G64B64A64_SFLOAT); i++)
        f2s[static_cast<VkFormat>(i)] = 32;
    f2s[VK_FORMAT_B10G11R11_UFLOAT_PACK32] = 4;
    f2s[VK_FORMAT_E5B9G9R9_UFLOAT_PACK32] = 4;
    f2s[VK_FORMAT_D16_UNORM] = 2;
    f2s[VK_FORMAT_X8_D24_UNORM_PACK32] = 4;
    f2s[VK_FORMAT_D32_SFLOAT] = 4;
    f2s[VK_FORMAT_S8_UINT] = 1;
    f2s[VK_FORMAT_D16_UNORM_S8_UINT] = 3;
    f2s[VK_FORMAT_D24_UNORM_S8_UINT] = 4;
    f2s[VK_FORMAT_D32_SFLOAT_S8_UINT] = 4;
    f2s[VK_FORMAT_BC1_RGB_UNORM_BLOCK] = 8;
    f2s[VK_FORMAT_BC1_RGB_SRGB_BLOCK] = 8;
    f2s[VK_FORMAT_BC1_RGBA_UNORM_BLOCK] = 8;
    f2s[VK_FORMAT_BC1_RGBA_SRGB_BLOCK] = 8;
    f2s[VK_FORMAT_BC2_UNORM_BLOCK] = 16;
    f2s[VK_FORMAT_BC2_SRGB_BLOCK] = 16;
    f2s[VK_FORMAT_BC3_UNORM_BLOCK] = 16;
    f2s[VK_FORMAT_BC3_SRGB_BLOCK] = 16;
    f2s[VK_FORMAT_BC4_UNORM_BLOCK] = 8;
    f2s[VK_FORMAT_BC4_SNORM_BLOCK] = 8;
    f2s[VK_FORMAT_BC5_UNORM_BLOCK] = 16;
    f2s[VK_FORMAT_BC5_SNORM_BLOCK] = 16;
    f2s[VK_FORMAT_BC6H_UFLOAT_BLOCK] = 16;
    f2s[VK_FORMAT_BC6H_SFLOAT_BLOCK] = 16;
    f2s[VK_FORMAT_BC7_UNORM_BLOCK] = 16;
    f2s[VK_FORMAT_BC7_SRGB_BLOCK] = 16;
    f2s[VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK ] = 8;
    f2s[VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK] = 8;
    f2s[VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK] = 8;
    f2s[VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK] = 8;
    f2s[VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK] = 16;
    f2s[VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK] = 16;
    f2s[VK_FORMAT_EAC_R11_UNORM_BLOCK] = 8;
    f2s[VK_FORMAT_EAC_R11_SNORM_BLOCK] = 8;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_EAC_R11G11_UNORM_BLOCK); i <= static_cast<uint32_t>(VK_FORMAT_ASTC_12x12_SRGB_BLOCK); i++)
        f2s[static_cast<VkFormat>(i)] = 16;
    f2s[VK_FORMAT_G8B8G8R8_422_UNORM] = 4;
    f2s[VK_FORMAT_B8G8R8G8_422_UNORM] = 4;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM); i <= static_cast<uint32_t>(VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM); i++)
        f2s[static_cast<VkFormat>(i)] = 1;
    f2s[VK_FORMAT_R10X6_UNORM_PACK16] = 2;
    f2s[VK_FORMAT_R10X6G10X6_UNORM_2PACK16] = 4;
    f2s[VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16] = 8;
    f2s[VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16] = 8;
    f2s[VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16] = 8;
    f2s[VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_R12X4_UNORM_PACK16] = 2;
    f2s[VK_FORMAT_R12X4G12X4_UNORM_2PACK16] = 4;
    f2s[VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16] = 8;
    f2s[VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16] = 8;
    f2s[VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16] = 8;
    f2s[VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G16B16G16R16_422_UNORM] = 8;
    f2s[VK_FORMAT_B16G16R16G16_422_UNORM] = 8;
    f2s[VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM] = 6;
    f2s[VK_FORMAT_G16_B16R16_2PLANE_420_UNORM] = 6;
    f2s[VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM] = 6;
    f2s[VK_FORMAT_G16_B16R16_2PLANE_422_UNORM] = 6;
    f2s[VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM] = 6;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG); i <= static_cast<uint32_t>(VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG); i++)
        f2s[static_cast<VkFormat>(i)] = 8;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT); i <= static_cast<uint32_t>(VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT); i++)
        f2s[static_cast<VkFormat>(i)] = 16;
    f2s[VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT] = 2;
    f2s[VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT] = 2;
    return f2s;
}


vka::detail::utility::CommandBufferWrapper::CommandBufferWrapper(VkDevice device, const VkCommandBufferAllocateInfo& ai)
{
    this->device = device;
    this->pool = ai.commandPool;
    this->cbos.resize(ai.commandBufferCount);
    this->last_error = vkAllocateCommandBuffers(device, &ai, this->cbos.data());
}
vka::detail::utility::CommandBufferWrapper::~CommandBufferWrapper(void)
{
    if(this->result() == VK_SUCCESS)
        vkFreeCommandBuffers(this->device, this->pool, this->cbos.size(), this->cbos.data());
}

const std::vector<VkCommandBuffer>& vka::detail::utility::CommandBufferWrapper::get(void) const noexcept
{
    return this->cbos;
}
VkResult vka::detail::utility::CommandBufferWrapper::result(void) const noexcept
{
    return last_error;
}

