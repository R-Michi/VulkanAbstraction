/**
 * @brief Implementation for the push-constant class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "push_constant_top.h"

template<uint32_t N>
constexpr vka::PushConstants<N>::PushConstants() noexcept :
    m_ranges{}
{}

template<uint32_t N>
inline vka::PushConstants<N>::PushConstants(const PushConstantLayout<N>& layout) :
    m_ranges(layout.ranges_array()),
    m_buff(layout.size())
{}

template<uint32_t N>
constexpr vka::PushConstants<N>::operator bool() const noexcept
{
    return !this->m_buff.empty();
}

template<uint32_t N>
inline vka::PushConstantView vka::PushConstants<N>::operator[] (uint32_t idx) noexcept
{
    return PushConstantView(this->m_ranges[idx], this->m_buff.data());
}

template<uint32_t N>
inline vka::PushConstantView vka::PushConstants<N>::at(uint32_t idx)
{
    if (this->m_buff.empty()) [[unlikely]]
        detail::error::throw_runtime_error(MSG_ACCESS);
    return PushConstantView(this->m_ranges.at(idx), this->m_buff.data());
}

template<uint32_t N>
constexpr uint32_t vka::PushConstants<N>::count() noexcept
{
    return N;
}

template<uint32_t N>
constexpr uint32_t vka::PushConstants<N>::size() const noexcept
{
    return this->m_buff.size();
}

template<uint32_t N>
constexpr const VkPushConstantRange* vka::PushConstants<N>::ranges() const noexcept
{
    return this->m_ranges.data();
}

template<uint32_t N>
inline void vka::PushConstants<N>::push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept
{
    for (uint32_t i = 0; !this->m_buff.empty() && i < N; i++)
    {
        const void* ptr = detail::common::add_cvp(this->m_buff.data(), this->m_ranges[i].offset);
        vkCmdPushConstants(cbo, layout, this->m_ranges[i].stageFlags, this->m_ranges[i].offset, this->m_ranges[i].size, ptr);
    }
}

