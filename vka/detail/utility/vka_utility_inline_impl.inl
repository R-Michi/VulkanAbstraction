/**
* @file     vka_utility_impl.inl
* @brief    Detailed implementation of inline utility functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

constexpr VkImageUsageFlagBits vka::detail::utility::ff2iu_bit(VkFormatFeatureFlagBits format_feature) noexcept
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

constexpr VkFormatFeatureFlagBits vka::detail::utility::iu2ff_bit(VkImageUsageFlagBits image_usage) noexcept
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



template<size_t N>
inline vka::detail::utility::CommandBufferWrapper<N>::CommandBufferWrapper(VkDevice device, const VkCommandBufferAllocateInfo& ai) noexcept
    : cbos{ VK_NULL_HANDLE }, last_result(VK_SUCCESS), device(device), pool(ai.commandPool)
{
    this->last_result = vkAllocateCommandBuffers(device, &ai, cbos);
}

template<size_t N>
inline vka::detail::utility::CommandBufferWrapper<N>::~CommandBufferWrapper(void)
{
    if (this->cbos[0] != VK_NULL_HANDLE)
        vkFreeCommandBuffers(this->device, this->pool, N, this->cbos);
}

template<size_t N>
inline VkCommandBuffer vka::detail::utility::CommandBufferWrapper<N>::operator[] (size_t i) const noexcept
{
    return this->cbos[i];
}

template<size_t N>
inline const VkCommandBuffer* vka::detail::utility::CommandBufferWrapper<N>::pat(size_t i) const noexcept
{
    return this->cbos + i;
}

template<size_t N>
inline VkResult vka::detail::utility::CommandBufferWrapper<N>::result(void) const noexcept
{
    return this->last_result;
}



inline vka::detail::utility::CommandBufferWrapper<1>::CommandBufferWrapper(VkDevice device, const VkCommandBufferAllocateInfo& ai) noexcept
    : cbo(VK_NULL_HANDLE), last_result(VK_SUCCESS), device(device), pool(ai.commandPool)
{
    this->last_result = vkAllocateCommandBuffers(device, &ai, &cbo);
}

inline vka::detail::utility::CommandBufferWrapper<1>::~CommandBufferWrapper(void)
{
    if (this->cbo != VK_NULL_HANDLE)
        vkFreeCommandBuffers(this->device, this->pool, 1, &this->cbo);
}

inline VkCommandBuffer vka::detail::utility::CommandBufferWrapper<1>::get(void) const noexcept
{
    return this->cbo;
}

inline const VkCommandBuffer* vka::detail::utility::CommandBufferWrapper<1>::pget(void) const noexcept
{
    return &this->cbo;
}

inline vka::detail::utility::CommandBufferWrapper<1>::operator VkCommandBuffer(void) const noexcept
{
    return this->cbo;
}

inline const VkCommandBuffer* vka::detail::utility::CommandBufferWrapper<1>::operator& (void) const noexcept
{
    return &this->cbo;
}

inline VkResult vka::detail::utility::CommandBufferWrapper<1>::result(void) const noexcept
{
    return this->last_result;
}
