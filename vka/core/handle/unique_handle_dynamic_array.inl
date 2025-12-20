#pragma once

#include "unique_handle.h"

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], deleter, Parent>::unique_handle() noexcept :
    m_parent{},
    m_handles(nullptr),
    m_count(0)
{}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], deleter, Parent>::unique_handle(Parent parent, Handle* handles, uint32_t count) noexcept :
    m_parent(parent),
    m_handles(handles),
    m_count(count)
{}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], deleter, Parent>::unique_handle(unique_handle&& src) noexcept :
    m_parent(src.m_parent),
    m_handles(src.m_handles),
    m_count(src.m_count)
{
    src.reset_state();
}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], deleter, Parent>::~unique_handle()
{
    this->destroy_handles();
}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], deleter, Parent>& vka::unique_handle<Handle[], deleter, Parent>::operator= (null_handle_t) noexcept
{
    this->destroy_handles();
    this->reset_state();
    return *this;
}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], deleter, Parent>& vka::unique_handle<Handle[], deleter, Parent>::operator= (unique_handle&& src) noexcept
{
    this->destroy_handles();
    this->m_parent = src.m_parent;
    this->m_handles = src.m_handles;
    this->m_count = src.m_count;
    src.reset_state();
    return *this;
}

template<typename Handle, auto deleter, typename Parent>
constexpr bool vka::unique_handle<Handle[], deleter, Parent>::operator== (const unique_handle& src) const noexcept
{
    if (this->m_count != src.m_count)
        return false;

    for (uint32_t i = 0; i < this->m_count; i++)
    {
        if (this->m_handles[i] != src.m_handles[i])
            return false;
    }
    return true;
}

template<typename Handle, auto deleter, typename Parent>
constexpr bool vka::unique_handle<Handle[], deleter, Parent>::operator!= (const unique_handle& src) const noexcept
{
    return !(*this == src);
}

template<typename Handle, auto deleter, typename Parent>
constexpr Handle vka::unique_handle<Handle[], deleter, Parent>::operator[] (uint32_t index) const noexcept(detail::handle::debug_noexcept)
{
#ifdef _DEBUG
    if (index >= this->m_count) [[unlikely]]
        throw std::out_of_range("unique_handle: Index out of range.");
#endif
    return this->m_handles[index];
}

template<typename Handle, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], deleter, Parent>::operator bool() const noexcept
{
    return this->m_handles != nullptr;
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], deleter, Parent>::destroy_handles() noexcept
{
    // Precondition: m_handles == nullptr implies m_count == 0
    // Precondition: m_count != 0 implies m_handles != nullptr
    for (uint32_t i = 0; i < this->m_count; i++)
    {
        if (static_cast<bool>(this->m_handles[i]))
            deleter(this->m_parent, this->m_handles[i], nullptr);
    }
    delete[] this->m_handles;
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], deleter, Parent>::reset_state() noexcept
{
    this->m_handles = nullptr;
    this->m_count = 0;
}

template<typename Handle, auto deleter, typename Parent>
constexpr Handle* vka::unique_handle<Handle[], deleter, Parent>::get() noexcept
{
    return this->m_handles;
}

template<typename Handle, auto deleter, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], deleter, Parent>::get() const noexcept
{
    return this->m_handles;
}

template<typename Handle, auto deleter, typename Parent>
constexpr Parent vka::unique_handle<Handle[], deleter, Parent>::parent() const noexcept
{
    return this->m_parent;
}

template<typename Handle, auto deleter, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], deleter, Parent>::release() noexcept
{
    const Handle* const cur_handles = this->m_handles;
    this->reset_state();
    return cur_handles;
}

template<typename Handle, auto deleter, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], deleter, Parent>::release_reset(Handle* handles, uint32_t count)
{
    const Handle* const cur_handles = this->m_handles;
    this->m_handles = handles;
    this->m_count = count;
    return cur_handles;
}

template<typename Handle, auto deleter, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], deleter, Parent>::release_reset(Parent parent, Handle* handles, uint32_t count)
{
    const Handle* const cur_handles = this->m_handles;
    this->m_parent = parent;
    this->m_handles = handles;
    this->m_count = count;
    return cur_handles;
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], deleter, Parent>::reset(Handle* handles, uint32_t count)
{
    this->destroy_handles();
    this->m_handles = handles;
    this->m_count = count;
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], deleter, Parent>::reset(Parent parent, Handle* handles, uint32_t count)
{
    this->destroy_handles();
    this->m_parent = parent;
    this->m_handles = handles;
    this->m_count = count;
}

template<typename Handle, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], deleter, Parent>::destroy() noexcept
{
    this->destroy_handles();
    this->reset_state();
}

template<typename Handle, auto deleter, typename Parent>
void vka::unique_handle<Handle[], deleter, Parent>::swap(unique_handle& handle) noexcept
{
    Parent tmp_parent = this->m_parent;
    const Handle* tmp_handles = this->m_handles;
    uint32_t tmp_count = this->m_count;
    this->m_parent = handle.m_parent;
    this->m_handles = handle.m_handles;
    this->m_count = handle.m_count;
    handle.m_parent = tmp_parent;
    handle.m_handles = tmp_handles;
    handle.m_count = tmp_count;
}

template<typename Handle, auto deleter, typename Parent>
constexpr uint32_t vka::unique_handle<Handle[], deleter, Parent>::size() const noexcept
{
    return this->m_count;
}

template<typename Handle, auto deleter, typename Parent>
constexpr Handle* vka::unique_handle<Handle[], deleter, Parent>::begin() noexcept
{
    return this->m_handles;
}

template<typename Handle, auto deleter, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], deleter, Parent>::begin() const noexcept
{
    return this->m_handles;
}

template<typename Handle, auto deleter, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], deleter, Parent>::end() const noexcept
{
    return this->m_handles + this->m_count;
}
