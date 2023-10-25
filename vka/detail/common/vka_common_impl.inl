/**
* @file     vka_utility_impl.inl
* @brief    Detailed implementation of common functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

void vka::detail::common::init_format_sizeof(std::unordered_map<VkFormat, size_t>& f2s)
{
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
    f2s[VK_FORMAT_D32_SFLOAT_S8_UINT] = 8;
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
    f2s[VK_FORMAT_G8_B8R8_2PLANE_444_UNORM] = 3;
    f2s[VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16] = 6;
    f2s[VK_FORMAT_G16_B16R16_2PLANE_444_UNORM] = 6;
    f2s[VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT] = 2;
    f2s[VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT] = 2;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG); i <= static_cast<uint32_t>(VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG); i++)
        f2s[static_cast<VkFormat>(i)] = 8;
    for(uint32_t i = static_cast<uint32_t>(VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT); i <= static_cast<uint32_t>(VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT); i++)
        f2s[static_cast<VkFormat>(i)] = 16;
    f2s[VK_FORMAT_R16G16_S10_5_NV] = 4;
}
