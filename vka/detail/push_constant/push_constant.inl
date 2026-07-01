/**
 * @brief Inline implementation for the push-constant buffer.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include <iostream>

#include "push_constant.h"

constexpr vka::detail::PushConstantBuffer::PushConstantBuffer() noexcept :
    m_size(0),
    m_data(nullptr)
{}

constexpr vka::detail::PushConstantBuffer::PushConstantBuffer(uint32_t size) :
    m_size(size),
    m_data(operator new(size))
{}

constexpr vka::detail::PushConstantBuffer::PushConstantBuffer(PushConstantBuffer&& src) noexcept :
    m_size(src.m_size),
    m_data(src.m_data)
{
    src.m_data = nullptr;
}

constexpr vka::detail::PushConstantBuffer::~PushConstantBuffer()
{
    this->free_memory();
}

constexpr vka::detail::PushConstantBuffer& vka::detail::PushConstantBuffer::operator= (PushConstantBuffer&& src) noexcept
{
    this->free_memory();
    this->m_size = src.m_size;
    this->m_data = src.m_data;
    src.m_data = nullptr;
    return *this;
}

constexpr void* vka::detail::PushConstantBuffer::data() noexcept
{
    return this->m_data;
}

constexpr const void* vka::detail::PushConstantBuffer::data() const noexcept
{
    return this->m_data;
}

constexpr uint32_t vka::detail::PushConstantBuffer::size() const noexcept
{
    return this->m_size;
}

constexpr bool vka::detail::PushConstantBuffer::empty() const noexcept
{
    return this->m_size == 0;
}

constexpr void vka::detail::PushConstantBuffer::free_memory() const noexcept
{
    operator delete(this->m_data);
}