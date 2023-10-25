/**
* @file     vka_common_inline_impl.inl
* @brief    Implementation of commonly used functionality which is inline.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline vka::CommandBufferOTS::CommandBufferOTS(void) noexcept
    : m_device(VK_NULL_HANDLE), m_pool(VK_NULL_HANDLE), m_cbo(VK_NULL_HANDLE)
{}

inline vka::CommandBufferOTS::CommandBufferOTS(VkDevice device, VkCommandPool pool)
    : m_device(device), m_pool(pool), m_cbo(VK_NULL_HANDLE)
{
    this->internal_begin(device, pool);
}

inline vka::CommandBufferOTS::CommandBufferOTS(vka::CommandBufferOTS&& src) noexcept
    : m_device(src.m_device), m_pool(src.m_pool), m_cbo(src.m_cbo)
{
    src.m_cbo = VK_NULL_HANDLE;
}

inline vka::CommandBufferOTS &vka::CommandBufferOTS::operator=(vka::CommandBufferOTS&& src) noexcept
{
    // the command buffer of this may be allocated, if so, destroy it
    this->destroy();
    this->m_device = src.m_device;
    this->m_pool = src.m_pool;
    this->m_cbo = src.m_cbo;
    src.m_cbo = VK_NULL_HANDLE;
    return *this;
}

inline vka::CommandBufferOTS::~CommandBufferOTS(void)
{
    this->destroy();
}

inline void vka::CommandBufferOTS::destroy(void) noexcept
{
    if (this->m_cbo != VK_NULL_HANDLE)
        vkFreeCommandBuffers(this->m_device, this->m_pool, 1, &this->m_cbo);
}

inline void vka::CommandBufferOTS::begin(VkDevice device, VkCommandPool pool)
{
    if (this->m_cbo == VK_NULL_HANDLE)
    {
        this->m_device = device;
        this->m_pool = pool;
        this->internal_begin(device, pool);
    }
}

inline VkCommandBuffer vka::CommandBufferOTS::handle(void) const noexcept
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

/*
 * Initialize the map only once at the first call to that function. Every subsequent call just
 * returns the size of the given format.
 */
inline size_t vka::common::format_sizeof(VkFormat format)
{
    static std::unordered_map<VkFormat, size_t> f2s;    // creates an empty map
    if (f2s.empty()) [[unlikely]]   // this is only called once
        detail::common::init_format_sizeof(f2s);
    return f2s.at(format);
}
