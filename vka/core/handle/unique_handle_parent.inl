#pragma once

#include "unique_handle.h"

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle, N, Parent>::unique_handle(Parent parent, Handle handle) noexcept :
    m_parent(parent),
    m_handle(handle)
{}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle, N, Parent>::unique_handle(unique_handle&& src) noexcept :
    m_parent(src.m_parent),
    m_handle(src.m_handle)
{
    src.m_handle = VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle, N, Parent>::~unique_handle()
{
    this->destroy_handle();
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle, N, Parent>& vka::unique_handle<Handle, N, Parent>::operator= (null_handle_t) noexcept
{
    this->destroy_handle();
    this->m_handle = VK_NULL_HANDLE;
    return *this;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle, N, Parent>& vka::unique_handle<Handle, N, Parent>::operator= (unique_handle&& src) noexcept
{
    this->destroy_handle();
    this->m_parent = src.m_parent;
    this->m_handle = src.m_handle;
    src.m_handle = VK_NULL_HANDLE;
    return *this;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr bool vka::unique_handle<Handle, N, Parent>::operator== (null_handle_t) const noexcept
{
    return this->m_handle == VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr bool vka::unique_handle<Handle, N, Parent>::operator== (const unique_handle& src) const noexcept
{
    return this->m_handle == src.m_handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr bool vka::unique_handle<Handle, N, Parent>::operator!= (null_handle_t) const noexcept
{
    return this->m_handle != VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr bool vka::unique_handle<Handle, N, Parent>::operator!= (const unique_handle& src) const noexcept
{
    return this->m_handle != src.m_handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle, N, Parent>::operator bool() const noexcept
{
    return this->m_handle != VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle, N, Parent>::destroy_handle() noexcept
{
    if (this->m_handle != VK_NULL_HANDLE)
        detail::handle::destroy_f<Handle>(this->m_parent, this->m_handle, nullptr);
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr Handle vka::unique_handle<Handle, N, Parent>::get() const noexcept
{
    return this->m_handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr Parent vka::unique_handle<Handle, N, Parent>::parent() const noexcept
{
    return this->m_parent;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr Handle vka::unique_handle<Handle, N, Parent>::release() noexcept
{
    Handle handle = this->m_handle;
    this->m_handle = VK_NULL_HANDLE;
    return handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle, N, Parent>::reset(Handle handle) noexcept
{
    this->destroy_handle();
    this->m_handle = handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle, N, Parent>::reset(Parent parent, Handle handle) noexcept
{
    this->destroy_handle();
    this->m_parent = parent;
    this->m_handle = handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle, N, Parent>::destroy() noexcept
{
    this->destroy_handle();
    this->m_handle = VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
void vka::unique_handle<Handle, N, Parent>::swap(unique_handle& handle) noexcept
{
    Handle tmp_handle = this->m_handle;
    Parent tmp_parent = this->m_parent;
    this->m_parent = handle.m_parent;
    this->m_handle = handle.m_handle;
    handle.m_parent = tmp_parent;
    handle.m_handle = tmp_handle;
}