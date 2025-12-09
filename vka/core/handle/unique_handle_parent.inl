#pragma once

#include "unique_handle.h"

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle, deleter, Parent>::unique_handle() noexcept :
    m_parent{},
    m_handle{}
{}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle, deleter, Parent>::unique_handle(Parent parent, Handle handle) noexcept :
    m_parent(parent),
    m_handle(handle)
{}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle, deleter, Parent>::unique_handle(unique_handle&& src) noexcept :
    m_parent(src.m_parent),
    m_handle(src.m_handle)
{
    src.reset_state();
}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle, deleter, Parent>::~unique_handle()
{
    this->destroy_handle();
}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle, deleter, Parent>& vka::unique_handle<Handle, deleter, Parent>::operator= (null_handle_t) noexcept
{
    this->destroy_handle();
    this->reset_state();
    return *this;
}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle, deleter, Parent>& vka::unique_handle<Handle, deleter, Parent>::operator= (unique_handle&& src) noexcept
{
    this->destroy_handle();
    this->m_parent = src.m_parent;
    this->m_handle = src.m_handle;
    src.reset_state();
    return *this;
}

template<typename Handle, auto deleter, typename Parent>
constexpr bool vka::unique_handle<Handle, deleter, Parent>::operator== (null_handle_t) const noexcept
{
    return this->m_handle == VK_NULL_HANDLE;
}

template<typename Handle, auto deleter, typename Parent>
constexpr bool vka::unique_handle<Handle, deleter, Parent>::operator== (const unique_handle& src) const noexcept
{
    return this->m_handle == src.m_handle;
}

template<typename Handle, auto deleter, typename Parent>
constexpr bool vka::unique_handle<Handle, deleter, Parent>::operator!= (null_handle_t) const noexcept
{
    return !(*this == VK_NULL_HANDLE);
}

template<typename Handle, auto deleter, typename Parent>
constexpr bool vka::unique_handle<Handle, deleter, Parent>::operator!= (const unique_handle& src) const noexcept
{
    return !(*this == src);
}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle, deleter, Parent>::operator bool() const noexcept
{
    return static_cast<bool>(this->m_handle);
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle, deleter, Parent>::destroy_handle() noexcept
{
    if (static_cast<bool>(this->m_handle))
        deleter(this->m_parent, this->m_handle, nullptr);
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle, deleter, Parent>::reset_state() noexcept
{
    this->m_handle = {}; // sets primitive handles to VK_NULL_HANDLE
}

template<typename Handle, auto deleter, typename Parent>
constexpr Handle vka::unique_handle<Handle, deleter, Parent>::get() const noexcept
{
    return this->m_handle;
}

template<typename Handle, auto deleter, typename Parent>
constexpr Parent vka::unique_handle<Handle, deleter, Parent>::parent() const noexcept
{
    return this->m_parent;
}

template<typename Handle, auto deleter, typename Parent>
constexpr Handle vka::unique_handle<Handle, deleter, Parent>::release() noexcept
{
    Handle handle = this->m_handle;
    this->reset_state();
    return handle;
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle, deleter, Parent>::reset(Handle handle) noexcept
{
    this->destroy_handle();
    this->m_handle = handle;
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle, deleter, Parent>::reset(Parent parent, Handle handle) noexcept
{
    this->destroy_handle();
    this->m_parent = parent;
    this->m_handle = handle;
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle, deleter, Parent>::destroy() noexcept
{
    this->destroy_handle();
    this->reset_state();
}

template<typename Handle, auto deleter, typename Parent>
void vka::unique_handle<Handle, deleter, Parent>::swap(unique_handle& handle) noexcept
{
    Handle tmp_handle = this->m_handle;
    Parent tmp_parent = this->m_parent;
    this->m_parent = handle.m_parent;
    this->m_handle = handle.m_handle;
    handle.m_parent = tmp_parent;
    handle.m_handle = tmp_handle;
}