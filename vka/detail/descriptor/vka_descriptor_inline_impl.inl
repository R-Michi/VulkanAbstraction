/**
* @file     vka_descriptor_inline_impl.inl
* @brief    Detailed implementation of descriptor-manager classes and functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

template<typename Info, uint32_t S>
constexpr vka::detail::descriptor::DescriptorInfoList<Info, S>::DescriptorInfoList(void) noexcept
    : m_infos{}, m_idx(0)
{}

template<typename Info, uint32_t S>
constexpr uint32_t vka::detail::descriptor::DescriptorInfoList<Info, S>::size(void) const noexcept
{
    return S;
}

template<typename Info, uint32_t S>
inline uint32_t vka::detail::descriptor::DescriptorInfoList<Info, S>::count(void) const noexcept
{
    return this->m_idx;
}

template<typename Info, uint32_t S>
inline const Info* vka::detail::descriptor::DescriptorInfoList<Info, S>::data(void) const noexcept
{
    return this->m_infos;
}

template<typename Info>
inline uint32_t vka::detail::descriptor::DescriptorInfoList<Info, 0>::count(void) const noexcept
{
    return this->m_infos.size();
}

template<typename Info>
inline const Info* vka::detail::descriptor::DescriptorInfoList<Info, 0>::data(void) const noexcept
{
    return this->m_infos.data();
}
