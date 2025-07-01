/**
 * @brief Inline implementation for the push-constant buffer.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

inline vka::detail::PushConstantBuffer::PushConstantBuffer() noexcept :
    m_size(0),
    m_data(nullptr)
{}

inline vka::detail::PushConstantBuffer::PushConstantBuffer(uint32_t size) : PushConstantBuffer()
{
    this->allocate(size);
}

inline vka::detail::PushConstantBuffer::PushConstantBuffer(PushConstantBuffer&& src) noexcept :
    m_size(src.m_size),
    m_data(src.m_data)
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

inline vka::detail::PushConstantBuffer::~PushConstantBuffer()
{
    this->free_memory();
}

inline void vka::detail::PushConstantBuffer::allocate(uint32_t size)
{
    if (this->m_data == nullptr)
    {
        this->m_data = operator new(size);
        error::check_memory(this->m_data);
        this->m_size = size;
    }
}

inline void vka::detail::PushConstantBuffer::free_memory() const noexcept
{
    if (this->m_data != nullptr)
        std::free(this->m_data);
}

inline void vka::detail::PushConstantBuffer::free() noexcept
{
    this->free_memory();
    this->m_size = 0;
    this->m_data = nullptr;
}

inline void* vka::detail::PushConstantBuffer::data() noexcept
{
    return this->m_data;
}

inline const void* vka::detail::PushConstantBuffer::data() const noexcept
{
    return this->m_data;
}

inline uint32_t vka::detail::PushConstantBuffer::size() const noexcept
{
    return this->m_size;
}

inline bool vka::detail::PushConstantBuffer::empty() const noexcept
{
    return (this->m_size == 0);
}
