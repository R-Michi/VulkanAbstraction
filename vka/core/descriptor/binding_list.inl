#pragma once

#include "top.h"

constexpr vka::DescriptorBindingList::DescriptorBindingList() :
    m_bindings(1)
{}

constexpr uint32_t vka::DescriptorBindingList::count() const noexcept
{
    return this->m_bindings.size();
}

constexpr uint32_t vka::DescriptorBindingList::binding_count(uint32_t set) const noexcept
{
    return this->m_bindings[set].size();
}

constexpr const VkDescriptorSetLayoutBinding* vka::DescriptorBindingList::bindings(uint32_t set) const noexcept
{
    return this->m_bindings[set].data();
}

inline vka::DescriptorLayouts vka::DescriptorBindingList::create_layouts(VkDevice device, VkDescriptorSetLayoutCreateFlags flags) const
{
    return DescriptorLayouts(device, *this, flags);
}
