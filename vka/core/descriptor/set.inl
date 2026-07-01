#pragma once

#include "top.h"

inline VkDescriptorSet vka::DescriptorSets::operator[] (uint32_t idx) const noexcept
{
    return this->m_sets.get().sets[idx];
}

constexpr vka::DescriptorSets::operator bool() const noexcept
{
    return (bool)this->m_sets;
}

constexpr uint32_t vka::DescriptorSets::count() const noexcept
{
    return this->m_sets.get().count;
}

constexpr VkDevice vka::DescriptorSets::parent() const noexcept
{
    return this->m_sets.parent().device;
}

constexpr VkDescriptorPool vka::DescriptorSets::pool() const noexcept
{
    return this->m_sets.parent().pool;
}

constexpr const VkDescriptorSet* vka::DescriptorSets::handles() const noexcept
{
    return this->m_sets.get().sets;
}

constexpr void vka::DescriptorSets::destroy() noexcept
{
    this->m_sets.destroy();
}

inline void vka::DescriptorSets::bind(VkCommandBuffer cbo, VkPipelineBindPoint bind_point, VkPipelineLayout pipeline_layout) const noexcept
{
    vkCmdBindDescriptorSets(cbo, bind_point, pipeline_layout, 0, this->m_sets.get().count, this->m_sets.get().sets, 0, nullptr);
}

constexpr vka::DescriptorUpdateOP vka::DescriptorSets::update_op() const noexcept
{
    return DescriptorUpdateOP(*this);
}
