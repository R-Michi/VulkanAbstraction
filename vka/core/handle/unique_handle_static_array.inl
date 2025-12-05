#pragma once

#include "unique_handle.h"

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle[], N, Parent>::unique_handle() noexcept :
    m_parent(VK_NULL_HANDLE),
    m_handles{VK_NULL_HANDLE}
{}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle[], N, Parent>::unique_handle(Parent parent, const Handle* handles) noexcept :
    m_parent(parent)
{
    for (uint32_t i = 0; i < N; i++)
        this->m_handles[i] = handles[i];
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle[], N, Parent>::unique_handle(Parent parent, const std::array<Handle, N>& handles) noexcept :
    m_parent(parent),
    m_handles(handles)
{}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle[], N, Parent>::unique_handle(unique_handle&& src) noexcept :
    m_parent(src.m_parent),
    m_handles(src.m_handles)
{
    src.m_handles = {};
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle[], N, Parent>::~unique_handle()
{
    this->destroy_handles();
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle[], N, Parent>& vka::unique_handle<Handle[], N, Parent>::operator= (null_handle_t) noexcept
{
    this->destroy_handles();
    this->m_handles = {};
    return *this;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle[], N, Parent>& vka::unique_handle<Handle[], N, Parent>::operator= (unique_handle&& src) noexcept
{
    this->destroy_handles();
    this->m_parent = src.m_parent;
    this->m_handles = src.m_handles;
    src.m_handles = {};
    return *this;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr bool vka::unique_handle<Handle[], N, Parent>::operator==(const unique_handle& src) const noexcept
{
    return this->m_handles == src.m_handles;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr bool vka::unique_handle<Handle[], N, Parent>::operator!=(const unique_handle& src) const noexcept
{
    return !(*this == src);
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr Handle vka::unique_handle<Handle[], N, Parent>::operator[](uint32_t index) const noexcept(detail::handle::debug_noexcept)
{
#ifdef _DEBUG
    if (index >= N) [[unlikely]]
        throw std::out_of_range("unique_handle: Index out of range.");
#endif
    return this->m_handles[index];
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr vka::unique_handle<Handle[], N, Parent>::operator bool() const noexcept
{
    for (uint32_t i = 0; i < N; i++)
    {
        if (this->m_handles[i] != VK_NULL_HANDLE)
            return true;
    }
    return false;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle[], N, Parent>::destroy_handles() noexcept
{
    for (const Handle handle : this->m_handles)
    {
        if (handle != VK_NULL_HANDLE)
            detail::handle::destroy_f<Handle>(this->m_parent, handle, nullptr);
    }
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], N, Parent>::get() const noexcept
{
    return this->m_handles.data();
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr Parent vka::unique_handle<Handle[], N, Parent>::parent() const noexcept
{
    return this->m_parent;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr std::array<Handle, N> vka::unique_handle<Handle[], N, Parent>::release() noexcept
{
    std::array<Handle, N> handles = this->m_handles;
    this->m_handles = {};
    return handles;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle[], N, Parent>::release(Handle* handles) noexcept
{
    for (uint32_t i = 0; i < N; i++)
        handles[i] = this->m_handles[i];
    this->m_handles = {};
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle[], N, Parent>::reset(const Handle* handles) noexcept
{
    this->destroy_handles();
    for (uint32_t i = 0; i < N; i++)
        this->m_handles[i] = handles[i];
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle[], N, Parent>::reset(const std::array<Handle, N>& handles) noexcept
{
    this->destroy_handles();
    this->m_handles = handles;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle[], N, Parent>::reset(Parent parent, const Handle* handles) noexcept
{
    this->destroy_handles();
    this->m_parent = parent;
    for (uint32_t i = 0; i < N; i++)
        this->m_handles[i] = handles[i];
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle[], N, Parent>::reset(Parent parent, const std::array<Handle, N>& handles) noexcept
{
    this->destroy_handles();
    this->m_parent = parent;
    this->m_handles = handles;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr void vka::unique_handle<Handle[], N, Parent>::destroy() noexcept
{
    this->destroy_handles();
    this->m_handles = {};
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
void vka::unique_handle<Handle[], N, Parent>::swap(unique_handle& handle) noexcept
{
    Parent tmp_parent = this->m_parent;
    std::array<Handle, N> tmp_handles = this->m_handles;
    this->m_parent = handle.m_parent;
    this->m_handles = handle.m_handles;
    handle.m_parent = tmp_parent;
    handle.m_handles = tmp_handles;
}

template<vka::detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
constexpr uint32_t vka::unique_handle<Handle[], N, Parent>::size() noexcept
{
    return N;
}