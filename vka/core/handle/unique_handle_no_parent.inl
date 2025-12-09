#pragma once

#include "unique_handle.h"

template<typename Handle, auto deleter>
constexpr vka::unique_handle<Handle, deleter, void>::unique_handle() noexcept :
    m_handle{}
{}

template<typename Handle, auto deleter>
constexpr vka::unique_handle<Handle, deleter, void>::unique_handle(Handle handle) noexcept :
    m_handle(handle)
{}

template<typename Handle, auto deleter>
constexpr vka::unique_handle<Handle, deleter, void>::unique_handle(unique_handle&& src) noexcept :
    m_handle(src.m_handle)
{
    src.reset_state();
}

template<typename Handle, auto deleter>
constexpr vka::unique_handle<Handle, deleter, void>::~unique_handle()
{
    this->destroy_handle();
}

template<typename Handle, auto deleter>
constexpr vka::unique_handle<Handle, deleter, void>& vka::unique_handle<Handle, deleter, void>::operator= (null_handle_t) noexcept
{
    this->destroy_handle();
    this->reset_state();
    return *this;
}

template<typename Handle, auto deleter>
constexpr vka::unique_handle<Handle, deleter, void>& vka::unique_handle<Handle, deleter, void>::operator= (unique_handle&& src) noexcept
{
    this->destroy_handle();
    this->m_handle = src.m_handle;
    src.reset_state();
    return *this;
}

template<typename Handle, auto deleter>
constexpr bool vka::unique_handle<Handle, deleter, void>::operator== (null_handle_t) const noexcept
{
    return this->m_handle == VK_NULL_HANDLE;
}

template<typename Handle, auto deleter>
constexpr bool vka::unique_handle<Handle, deleter, void>::operator== (const unique_handle& src) const noexcept
{
    return this->m_handle == src.m_handle;
}

template<typename Handle, auto deleter>
constexpr bool vka::unique_handle<Handle, deleter, void>::operator!= (null_handle_t) const noexcept
{
    return !(*this == VK_NULL_HANDLE);
}

template<typename Handle, auto deleter>
constexpr bool vka::unique_handle<Handle, deleter, void>::operator!= (const unique_handle& src) const noexcept
{
    return !(*this == src);
}

template<typename Handle, auto deleter>
constexpr vka::unique_handle<Handle, deleter, void>::operator bool() const noexcept
{
    return static_cast<bool>(this->m_handle);
}

template<typename Handle, auto deleter>
constexpr void vka::unique_handle<Handle, deleter, void>::destroy_handle() noexcept
{
    if (static_cast<bool>(this->m_handle))
        deleter(this->m_handle, nullptr);
}

template<typename Handle, auto deleter>
constexpr void vka::unique_handle<Handle, deleter, void>::reset_state() noexcept
{
    this->m_handle = {}; // sets primitive handles to VK_NULL_HANDLE
}

template<typename Handle, auto deleter>
constexpr Handle vka::unique_handle<Handle, deleter, void>::get() const noexcept
{
    return this->m_handle;
}

template<typename Handle, auto deleter>
constexpr Handle vka::unique_handle<Handle, deleter, void>::release() noexcept
{
    Handle handle = this->m_handle;
    this->reset_state();
    return handle;
}

template<typename Handle, auto deleter>
constexpr void vka::unique_handle<Handle, deleter, void>::reset(Handle handle) noexcept
{
    this->destroy_handle();
    this->m_handle = handle;
}

template<typename Handle, auto deleter>
constexpr void vka::unique_handle<Handle, deleter, void>::destroy() noexcept
{
    this->destroy_handle();
    this->reset_state();
}

template<typename Handle, auto deleter>
void vka::unique_handle<Handle, deleter, void>::swap(unique_handle& handle) noexcept
{
    Handle tmp_handle = this->m_handle;
    this->m_handle = handle.m_handle;
    handle.m_handle = tmp_handle;
}