#pragma once

#include "window.h"

constexpr vka::detail::unique_window::unique_window() noexcept :
    m_window(nullptr)
{}

constexpr vka::detail::unique_window::unique_window(GLFWwindow* handle) noexcept :
    m_window(handle)
{}

constexpr vka::detail::unique_window::unique_window(unique_window&& src) noexcept :
    m_window(src.m_window)
{
    src.m_window = nullptr;
}

constexpr vka::detail::unique_window::~unique_window()
{
    this->destroy_handle();
}

constexpr vka::detail::unique_window& vka::detail::unique_window::operator= (unique_window&& src) noexcept
{
    this->destroy_handle();
    this->m_window = src.m_window;
    src.m_window = nullptr;
    return *this;
}

constexpr vka::detail::unique_window::operator bool() const noexcept
{
    return this->m_window != nullptr;
}

constexpr GLFWwindow* vka::detail::unique_window::get() const noexcept
{
    return m_window;
}

constexpr void vka::detail::unique_window::destroy() noexcept
{
    this->destroy_handle();
    this->m_window = nullptr;
}

constexpr void vka::detail::unique_window::destroy_handle() const noexcept
{
    if (this->m_window != nullptr)
        glfwDestroyWindow(this->m_window);
}