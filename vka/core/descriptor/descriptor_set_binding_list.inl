/**
 * @brief Implementation for the 'DescriptorSetBindingList' class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "descriptor_class.h"

template<uint32_t S>
inline uint32_t vka::DescriptorSetBindingList<S>::binding_count(uint32_t set) const noexcept
{
    return set >= S ? NCOUNT : this->m_bindings[set].size();
}

template<uint32_t S>
constexpr uint32_t vka::DescriptorSetBindingList<S>::count() noexcept
{
    return S;
}

template<uint32_t S>
inline const VkDescriptorSetLayoutBinding* vka::DescriptorSetBindingList<S>::bindings(uint32_t set) const noexcept
{
    return set >= S ? nullptr : this->m_bindings[set].data();
}

template<uint32_t S>
inline void vka::DescriptorSetBindingList<S>::push(uint32_t set, VkDescriptorType type, VkShaderStageFlags stages, uint32_t count, const VkSampler* immutable_samplers)
{
    if (set >= S) [[unlikely]]
        detail::error::throw_out_of_range(MSG_INVALID_INDEX);

    const VkDescriptorSetLayoutBinding binding = {
        .binding = (uint32_t)this->m_bindings[set].size(),
        .descriptorType = type,
        .descriptorCount = count,
        .stageFlags = stages,
        .pImmutableSamplers = immutable_samplers
    };
    this->m_bindings[set].push_back(binding);
}

template<uint32_t S>
inline vka::DescriptorLayoutArray<S> vka::DescriptorSetBindingList<S>::create_layouts(VkDevice device, VkDescriptorSetLayoutCreateFlags flags) const
{
    return DecriptorLayoutArray<S>(device, *this, flags);
}
