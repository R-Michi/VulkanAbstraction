#pragma once

#include "top.h"

inline VkDescriptorSetLayout vka::DescriptorLayouts::operator[] (uint32_t idx) const noexcept
{
    return this->m_layouts[idx];
}

constexpr vka::DescriptorLayouts::operator bool() const noexcept
{
    return (bool)this->m_layouts;
}

constexpr uint32_t vka::DescriptorLayouts::count() const noexcept
{
    return this->m_layouts.count();
}

constexpr VkDevice vka::DescriptorLayouts::parent() const noexcept
{
    return this->m_layouts.parent();
}

constexpr const VkDescriptorSetLayout* vka::DescriptorLayouts::handles() const noexcept
{
    return this->m_layouts.get();
}

constexpr void vka::DescriptorLayouts::destroy() noexcept
{
    this->m_layouts.destroy();
}

inline vka::DescriptorSets vka::DescriptorLayouts::create_sets(VkDescriptorPool pool) const
{
    return DescriptorSets(pool, *this);
}
