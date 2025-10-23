/**
 * @brief Inline implementation for the 'DescriptorLayoutArray' class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "descriptor_class.h"

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>::DescriptorLayoutArray() noexcept :
    m_device(VK_NULL_HANDLE),
    m_layouts{VK_NULL_HANDLE}
{}

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>::DescriptorLayoutArray(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags) :
    m_device(device),
    m_layouts{VK_NULL_HANDLE}
{
    this->internal_create(device, set_bindings, flags);
}

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>::DescriptorLayoutArray(DescriptorLayoutArray&& src) noexcept :
    m_device(src.m_device),
    m_layouts(src.m_layouts)
{
    src.m_layouts.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>& vka::DescriptorLayoutArray<S>::operator= (DescriptorLayoutArray&& src) noexcept
{
    // destroy_handles does nothing if the layouts have not been created
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_layouts = src.m_layouts;
    src.m_layouts.fill(VK_NULL_HANDLE);
    return *this;
}

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>::~DescriptorLayoutArray()
{
    this->destroy_handles();
}

template<uint32_t S>
void vka::DescriptorLayoutArray<S>::internal_create(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags)
{
    for (uint32_t i = 0; i < S; i++) // the number of sets must match the number of layouts, otherwise a compiler error
    {
        const VkDescriptorSetLayoutCreateInfo ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = flags,
            .bindingCount = set_bindings.binding_count(i),
            .pBindings = set_bindings.bindings(i)
        };
        check_result(vkCreateDescriptorSetLayout(device, &ci, nullptr, this->m_layouts.data() + i), MSG_CREATE_FAILED);
    }
}

template<uint32_t S>
inline void vka::DescriptorLayoutArray<S>::create(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags)
{
    if (!this->is_valid())
    {
        this->m_device = device;
        this->internal_create(device, set_bindings, flags);
    }
}

template<uint32_t S>
void vka::DescriptorLayoutArray<S>::destroy_handles() noexcept
{
    for (uint32_t i = 0; i < S; i++)
    {
        if (this->m_layouts[i] != VK_NULL_HANDLE)
            vkDestroyDescriptorSetLayout(this->m_device, this->m_layouts[i], nullptr);
    }
}

template<uint32_t S>
inline void vka::DescriptorLayoutArray<S>::destroy() noexcept
{
    this->destroy_handles();
    this->m_device = VK_NULL_HANDLE;
    this->m_layouts.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
constexpr uint32_t vka::DescriptorLayoutArray<S>::count() noexcept
{
    return S;
}

template<uint32_t S>
inline VkDescriptorSetLayout vka::DescriptorLayoutArray<S>::handle(size_t idx) const
{
    return this->m_layouts.at(idx);
}

template<uint32_t S>
inline VkDescriptorSetLayout vka::DescriptorLayoutArray<S>::handle_u(uint32_t idx) const noexcept
{
    return this->m_layouts[idx];
}

template<uint32_t S>
inline VkDescriptorSetLayout vka::DescriptorLayoutArray<S>::operator[] (size_t idx) const noexcept
{
    return this->m_layouts[idx];
}

template<uint32_t S>
inline const VkDescriptorSetLayout* vka::DescriptorLayoutArray<S>::layouts() const noexcept
{
    return this->m_layouts.data();
}

template<uint32_t S>
inline VkDevice vka::DescriptorLayoutArray<S>::device() const noexcept
{
    return this->m_device;
}

template<uint32_t S>
inline bool vka::DescriptorLayoutArray<S>::is_valid() const noexcept
{
    // if the last one is valid -> all are valid
    return (this->m_layouts[S-1] != VK_NULL_HANDLE);
}

template<uint32_t S>
inline vka::DescriptorSetArray<S> vka::DescriptorLayoutArray<S>::create_sets(VkDescriptorPool pool) const
{
    return DescriptorSetArray<S>(pool, *this);
}
