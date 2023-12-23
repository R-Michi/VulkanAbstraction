/**
* @file     vka_push_constant.h
* @brief    Detailed push constant implementation file of inline functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline vka::detail::PushConstantBuffer::PushConstantBuffer(void) noexcept :
    m_size(0), m_data(nullptr)
{}

inline vka::detail::PushConstantBuffer::PushConstantBuffer(uint32_t size) : PushConstantBuffer()
{
    this->allocate(size);
}

inline vka::detail::PushConstantBuffer::PushConstantBuffer(PushConstantBuffer&& src) noexcept :
    m_size(src.m_size), m_data(src.m_data)
{
    src.m_size = 0;
    src.m_data = nullptr;
}

inline vka::detail::PushConstantBuffer& vka::detail::PushConstantBuffer::operator= (PushConstantBuffer&& src) noexcept
{
    this->free();
    this->m_size = src.m_size;
    this->m_data = src.m_data;
    src.m_size = 0;
    src.m_data = nullptr;
    return *this;
}

inline vka::detail::PushConstantBuffer::~PushConstantBuffer(void)
{
    this->free_memory();
}

inline void vka::detail::PushConstantBuffer::free(void) noexcept
{
    this->free_memory();
    this->m_size = 0;
    this->m_data = nullptr;
}

inline void* vka::detail::PushConstantBuffer::data(void) noexcept
{
    return this->m_data;
}

inline uint32_t vka::detail::PushConstantBuffer::size(void) const noexcept
{
    return this->m_size;
}

inline bool vka::detail::PushConstantBuffer::empty(void) const noexcept
{
    return (this->m_size == 0);
}
