#pragma once

#include "handle_guard.h"

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::handle_guard<Handle, Parent>::handle_guard(Parent parent, const Handle* handle) noexcept :
    m_parent(parent),
    m_handles(handle),
    m_count(1)
{}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::handle_guard<Handle, Parent>::handle_guard(Parent parent, const Handle* handles, uint32_t count) noexcept :
    m_parent(parent),
    m_handles(handles),
    m_count(count)
{}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::handle_guard<Handle, Parent>::~handle_guard()
{
    if (this->m_handles != nullptr) [[unlikely]]
        this->destroy_handles();
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr void vka::handle_guard<Handle, Parent>::destroy_handles() noexcept
{
    for (uint32_t i = 0; i < this->m_count; i++)
    {
        if (this->m_handles[i] != VK_NULL_HANDLE)
            detail::handle::destroy_f<Handle>(this->m_parent, this->m_handles[i], nullptr);
    }
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr void vka::handle_guard<Handle, Parent>::release() noexcept
{
    this->m_handles = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------- SPECIALIZATION WITH NO PARENT HANDLE ---------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

template<vka::detail::handle::dispatchable_c Handle>
constexpr vka::handle_guard<Handle, void>::handle_guard(const Handle* handle) noexcept :
    m_handles(handle),
    m_count(1)
{}

template<vka::detail::handle::dispatchable_c Handle>
constexpr vka::handle_guard<Handle, void>::handle_guard(const Handle* handles, uint32_t count) noexcept :
    m_handles(handles),
    m_count(count)
{}

template<vka::detail::handle::dispatchable_c Handle>
constexpr vka::handle_guard<Handle, void>::~handle_guard()
{
    if (this->m_handles != nullptr) [[unlikely]]
        this->destroy_handles();
}

template<vka::detail::handle::dispatchable_c Handle>
constexpr void vka::handle_guard<Handle, void>::destroy_handles() noexcept
{
    for (uint32_t i = 0; i < this->m_count; i++)
    {
        if (this->m_handles[i] != VK_NULL_HANDLE)
            detail::handle::destroy_f<Handle>(this->m_handles[i], nullptr);
    }
}

template<vka::detail::handle::dispatchable_c Handle>
constexpr void vka::handle_guard<Handle, void>::release() noexcept
{
    this->m_count = 0;
}