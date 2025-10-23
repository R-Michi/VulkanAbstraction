/**
 * @brief Implementation for the push-constant class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "push_constant_class.h"

template<uint32_t N>
inline vka::PushConstants<N>::PushConstants() noexcept :
    m_layout(nullptr)
{}

template<uint32_t N>
inline vka::PushConstants<N>::PushConstants(const PushConstantLayout<N>& layout) :
    m_layout(&layout),
    m_buff(layout.size())
{}

template<uint32_t N>
inline vka::PushConstants<N>::PushConstants(PushConstants&& src) noexcept :
    m_layout(src.m_layout),
    m_buff(std::move(src.m_buff))
{}

template<uint32_t N>
inline vka::PushConstants<N>& vka::PushConstants<N>::operator= (PushConstants&& src) noexcept
{
    this->m_layout = src.m_layout;
    this->m_buff = std::move(src.m_buff);
    return *this;
}

template<uint32_t N>
inline void vka::PushConstants<N>::create(const PushConstantLayout<N>& layout)
{
    if (this->m_buff.empty())
    {
        this->m_layout = &layout;
        this->m_buff.allocate(layout.size());
    }
}

template<uint32_t N>
inline void vka::PushConstants<N>::destroy() noexcept
{
    this->m_buff.free();
}

template<uint32_t N>
inline void vka::PushConstants<N>::push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept
{
    for (uint32_t i = 0; i < N; i++)
        vkCmdPushConstants(cbo, layout, *(this->m_layout)[i].stageFlags, *(this->m_layout)[i].offset, *(this->m_layout)[i].size);
}

template<uint32_t N>
inline vka::PushConstantView vka::PushConstants<N>::at(uint32_t idx) const
{
    if (this->m_buff.empty()) [[unlikely]]
        detail::error::throw_runtime_error(MSG_ACCESS);
    return PushConstantView(this->m_layout->at(idx), this->m_buff.data());
}

template<uint32_t N>
inline vka::PushConstantView vka::PushConstants<N>::operator[] (uint32_t idx) const noexcept
{
    return PushConstantView(*(this->m_layout)[idx], this->m_buff.data());
}

template<uint32_t N>
inline bool vka::PushConstants<N>::is_valid() const noexcept
{
    return !this->m_buff.empty();
}
