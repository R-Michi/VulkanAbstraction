#pragma once

#include "unique_handle.h"

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr vka::unique_handle<Handle, N, void>::unique_handle(Handle handle) noexcept :
    m_handle(handle)
{}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr vka::unique_handle<Handle, N, void>::unique_handle(unique_handle&& src) noexcept :
    m_handle(src.m_handle)
{
    src.m_handle = VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr vka::unique_handle<Handle, N, void>::~unique_handle()
{
    this->destroy_handle();
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr vka::unique_handle<Handle, N, void>& vka::unique_handle<Handle, N, void>::operator= (null_handle_t) noexcept
{
    this->destroy_handle();
    this->m_handle = VK_NULL_HANDLE;
    return *this;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr vka::unique_handle<Handle, N, void>& vka::unique_handle<Handle, N, void>::operator= (unique_handle&& src) noexcept
{
    this->destroy_handle();
    this->m_handle = src.m_handle;
    src.m_handle = VK_NULL_HANDLE;
    return *this;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr bool vka::unique_handle<Handle, N, void>::operator== (null_handle_t) const noexcept
{
    return this->m_handle == VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr bool vka::unique_handle<Handle, N, void>::operator== (const unique_handle& src) const noexcept
{
    return this->m_handle == src.m_handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr bool vka::unique_handle<Handle, N, void>::operator!= (null_handle_t) const noexcept
{
    return this->m_handle != VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr bool vka::unique_handle<Handle, N, void>::operator!= (const unique_handle& src) const noexcept
{
    return this->m_handle != src.m_handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr vka::unique_handle<Handle, N, void>::operator bool() const noexcept
{
    return this->m_handle != VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr void vka::unique_handle<Handle, N, void>::destroy_handle() noexcept
{
    if (this->m_handle != VK_NULL_HANDLE)
        detail::handle::destroy_f<Handle>(this->m_handle, nullptr);
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr Handle vka::unique_handle<Handle, N, void>::get() const noexcept
{
    return this->m_handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr Handle vka::unique_handle<Handle, N, void>::release() noexcept
{
    Handle handle = this->m_handle;
    this->m_handle = VK_NULL_HANDLE;
    return handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr void vka::unique_handle<Handle, N, void>::reset(Handle handle) noexcept
{
    this->destroy_handle();
    this->m_handle = handle;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
constexpr void vka::unique_handle<Handle, N, void>::destroy() noexcept
{
    this->destroy_handle();
    this->m_handle = VK_NULL_HANDLE;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N>
void vka::unique_handle<Handle, N, void>::swap(unique_handle& handle) noexcept
{
    Handle tmp_handle = this->m_handle;
    this->m_handle = handle.m_handle;
    handle.m_handle = tmp_handle;
}