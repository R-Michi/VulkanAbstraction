/**
 * @brief Inline implementation for common functionality.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "common.h"

inline vka::CommandBufferOTS::CommandBufferOTS() noexcept :
    m_device(VK_NULL_HANDLE),
    m_pool(VK_NULL_HANDLE),
    m_cbo(VK_NULL_HANDLE)
{}

inline vka::CommandBufferOTS::CommandBufferOTS(VkDevice device, VkCommandPool pool) :
    m_device(device),
    m_pool(pool),
    m_cbo(VK_NULL_HANDLE)
{
    this->internal_begin();
}

inline vka::CommandBufferOTS::CommandBufferOTS(CommandBufferOTS&& src) noexcept :
    m_device(src.m_device),
    m_pool(src.m_pool),
    m_cbo(src.m_cbo)
{
    src.m_cbo = VK_NULL_HANDLE;
}

inline vka::CommandBufferOTS &vka::CommandBufferOTS::operator= (CommandBufferOTS&& src) noexcept
{
    // the command buffer of this may be allocated, if so, destroy it
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_pool = src.m_pool;
    this->m_cbo = src.m_cbo;
    src.m_cbo = VK_NULL_HANDLE;
    return *this;
}

inline vka::CommandBufferOTS::~CommandBufferOTS()
{
    this->destroy_handles();
}

inline void vka::CommandBufferOTS::begin(VkDevice device, VkCommandPool pool)
{
    if (this->m_cbo == VK_NULL_HANDLE)
    {
        this->m_device = device;
        this->m_pool = pool;
        this->internal_begin();
    }
}

inline VkCommandBuffer vka::CommandBufferOTS::handle() const noexcept
{
    return this->m_cbo;
}


constexpr VkImageUsageFlags vka::common::cvt_ff2iu(VkFormatFeatureFlags format_feature) noexcept
{
    constexpr size_t n_bits = 8 * sizeof(VkFlags);

    VkImageUsageFlags flags = 0;
    for (size_t i = 0; i < n_bits; i++)
        flags |= detail::common::ff2iu_bit(static_cast<VkFormatFeatureFlagBits>(format_feature & (1 << i)));
    return flags;
}

constexpr VkFormatFeatureFlags vka::common::cvt_iu2ff(VkImageUsageFlags image_usage) noexcept
{
    constexpr size_t n_bits = 8 * sizeof(VkFlags);

    VkFormatFeatureFlags flags = 0;
    for (size_t i = 0; i < n_bits; i++)
        flags |= detail::common::iu2ff_bit(static_cast<VkImageUsageFlagBits>(image_usage & (1 << i)));
    return flags;
}

inline void vka::common::cvt_stdstr2ccpv(const std::vector<std::string>& std_in, const char** ccp_out, size_t buff_size) noexcept
{
    for (size_t i = 0; i < std_in.size() && i < buff_size; i++)
        ccp_out[i] = std_in[i].c_str();
}

constexpr VkExtent3D vka::common::mip_extent(VkExtent3D extent, uint32_t level) noexcept
{
    if (level == 0) return extent;
    const uint32_t tmpx = extent.width >> level;
    const uint32_t tmpy = extent.height >> level;
    const uint32_t tmpz = extent.depth >> level;
    return {
        tmpx < 1 ? 1 : tmpx,
        tmpy < 1 ? 1 : tmpy,
        tmpz < 1 ? 1 : tmpz
    };
}

constexpr VkExtent3D vka::common::next_mip_extent(VkExtent3D extent) noexcept
{
    const uint32_t tmpx = extent.width >> 1;
    const uint32_t tmpy = extent.height >> 1;
    const uint32_t tmpz = extent.depth >> 1;
    return {
        tmpx < 1 ? 1 : tmpx,
        tmpy < 1 ? 1 : tmpy,
        tmpz < 1 ? 1 : tmpz
    };
}

constexpr VkOffset3D vka::common::next_mip_extent(VkOffset3D extent) noexcept
{
    const int32_t tmpx = extent.x >> 1;
    const int32_t tmpy = extent.y >> 1;
    const int32_t tmpz = extent.z >> 1;
    return {
        tmpx < 1 ? 1 : tmpx,
        tmpy < 1 ? 1 : tmpy,
        tmpz < 1 ? 1 : tmpz
    };
}

