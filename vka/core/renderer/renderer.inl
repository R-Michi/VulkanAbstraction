#pragma once

#include "renderer.h"

constexpr vka::Renderer::Renderer() noexcept :
    m_window(nullptr),
    m_frame_index(0)
{}

constexpr vka::Renderer::operator bool() const noexcept
{
    return (bool)this->m_context;
}

constexpr void vka::Renderer::destroy() noexcept
{
    this->m_window = nullptr;
    this->m_context = VK_NULL_HANDLE;
    this->m_map_image2frame.clear();
    this->m_map_frame2image.clear();
    this->m_frame_index = 0;
}