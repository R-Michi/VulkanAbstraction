/**
 * @brief Implementation for the push-constant view class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "top.h"

constexpr vka::PushConstantView::PushConstantView(VkPushConstantRange range, void* data) noexcept :
    m_range(range),
    m_data(detail::common::add_vp(data, range.offset))
{}

constexpr VkShaderStageFlags vka::PushConstantView::stages() const noexcept
{
    return this->m_range.stageFlags;
}

constexpr uint32_t vka::PushConstantView::size() const noexcept
{
    return this->m_range.size;
}

constexpr uint32_t vka::PushConstantView::offset() const noexcept
{
    return this->m_range.offset;
}

constexpr void* vka::PushConstantView::data() noexcept
{
    return this->m_data;
}

constexpr const void* vka::PushConstantView::data() const noexcept
{
    return this->m_data;
}

// ReSharper disable once CppMemberFunctionMayBeConst
inline void vka::PushConstantView::write(uint32_t offset, uint32_t size, const void* data)
{
    detail::error::check_range(offset, size, this->m_range.size, MSG_INVALID_RANGE);
    memcpy(this->m_data, data, size);
}

inline void vka::PushConstantView::push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept
{
    vkCmdPushConstants(cbo, layout, this->m_range.stageFlags, this->m_range.offset, this->m_range.size, this->m_data);
}