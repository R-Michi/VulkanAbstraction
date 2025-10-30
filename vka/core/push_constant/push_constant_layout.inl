/**
 * @brief Implementation for the push-constant layout class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "push_constant_class.h"

template<uint32_t N>
constexpr vka::PushConstantLayout<N>::PushConstantLayout(uint32_t size) noexcept :
    m_ranges{}, m_total_size(0), m_max_size(size), m_idx(0)
{}

template<uint32_t N>
constexpr void vka::PushConstantLayout<N>::init(uint32_t size) noexcept
{
    // can only be modified if no ranges have been added yet
    if (this->m_idx == 0)
        this->m_max_size = size;
}

template<uint32_t N>
constexpr uint32_t vka::PushConstantLayout<N>::round_size(uint32_t s) noexcept
{
    // equivalent to '(s - 1) / 4 * 4 + 4'
    return s == 0 ? 0 : (((s - 1) >> 2) << 2) + 4;
}

template<uint32_t N>
constexpr void vka::PushConstantLayout<N>::add(uint32_t size, VkShaderStageFlags stages)
{
    if (this->m_idx >= N) [[unlikely]]
        detail::error::throw_out_of_range(MSG_RANGE);

    size = round_size(size);
    const uint32_t new_size = this->m_total_size + size;
    if (new_size > this->m_max_size) [[unlikely]]
        detail::error::throw_runtime_error(MSG_SIZE);

    m_ranges[this->m_idx++] = { stages, this->m_total_size, size };
    this->m_total_size += size;
}

// ReSharper disable once CppRedundantInlineSpecifier
template<uint32_t N>
inline vka::PushConstants<N> vka::PushConstantLayout<N>::create_push_constants()
{
    return PushConstants<N>(*this);
}

template<uint32_t N>
constexpr uint32_t vka::PushConstantLayout<N>::count() noexcept
{
    return N;
}

template<uint32_t N>
constexpr uint32_t vka::PushConstantLayout<N>::size() const noexcept
{
    return this->m_total_size;
}

template<uint32_t N>
constexpr const VkPushConstantRange* vka::PushConstantLayout<N>::ranges() const noexcept
{
    return this->m_ranges.data();
}

template<uint32_t N>
constexpr VkPushConstantRange vka::PushConstantLayout<N>::at(uint32_t idx) const
{
    return this->m_ranges.at(idx);
}

template<uint32_t N>
constexpr VkPushConstantRange vka::PushConstantLayout<N>::operator[] (uint32_t idx) const noexcept
{
    return this->m_ranges[idx];
}

template<uint32_t N>
constexpr void vka::PushConstantLayout<N>::validate() const
{
    if (this->m_idx < N) [[unlikely]]
        detail::error::throw_runtime_error(MSG_UNUSED);
}
