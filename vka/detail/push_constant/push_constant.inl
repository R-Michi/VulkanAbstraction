/**
 * @brief Inline implementation for the push-constant buffer.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "push_constant.h"

constexpr vka::detail::PushConstantBuffer::PushConstantBuffer() noexcept :
    m_size(0),
    m_data(nullptr)
{}

inline vka::detail::PushConstantBuffer::PushConstantBuffer(uint32_t size) : PushConstantBuffer()
{
    this->allocate(size);
}

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

inline void vka::detail::PushConstantBuffer::allocate(uint32_t size)
{
    if (this->m_data == nullptr)
    {
        this->m_data = std::malloc(size);
        error::check_memory(this->m_data);
        this->m_size = size;
    }
}

constexpr void vka::detail::PushConstantBuffer::free_memory() const noexcept
{
    if (this->m_data != nullptr)
        std::free(this->m_data);
}

constexpr void vka::detail::PushConstantBuffer::free() noexcept
{
    this->free_memory();
    this->m_size = 0;
    this->m_data = nullptr;
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
    return (this->m_size == 0);
}
