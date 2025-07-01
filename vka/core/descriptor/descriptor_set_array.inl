/**
 * @brief Implementation for the 'DescriptorSetArray' class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

// ReSharper disable CppRedundantInlineSpecifier
template<uint32_t S>
inline vka::DescriptorSetArray<S>::DescriptorSetArray() noexcept :
    m_device(VK_NULL_HANDLE),
    m_pool(VK_NULL_HANDLE),
    m_sets{VK_NULL_HANDLE}
{}

template<uint32_t S>
inline vka::DescriptorSetArray<S>::DescriptorSetArray(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout) :
    m_device(layout.device()),
    m_pool(pool),
    m_sets{VK_NULL_HANDLE}
{
    this->internal_create(pool, layout);
}

template<uint32_t S>
inline vka::DescriptorSetArray<S>::DescriptorSetArray(DescriptorSetArray&& src) noexcept :
    m_device(src.m_device),
    m_pool(src.m_pool),
    m_sets(src.m_sets)
{
    src.m_sets.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
inline vka::DescriptorSetArray<S>& vka::DescriptorSetArray<S>::operator= (DescriptorSetArray&& src) noexcept
{
    // destroy_handles does nothing if the sets have not been allocated
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_pool = src.m_pool;
    this->m_sets = src.m_sets;
    src.m_sets.fill(VK_NULL_HANDLE);
    return *this;
}

template<uint32_t S>
inline vka::DescriptorSetArray<S>::~DescriptorSetArray()
{
    this->destroy_handles();
}

template<uint32_t S>
void vka::DescriptorSetArray<S>::internal_create(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout)
{
    const VkDescriptorSetAllocateInfo ai = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = pool,
        .descriptorSetCount = S,
        .pSetLayouts = layout.layouts()
    };
    check_result(vkAllocateDescriptorSets(layout.device(), &ai, this->m_sets.data()), MSG_CREATE_FAILED);
}

template<uint32_t S>
inline void vka::DescriptorSetArray<S>::create(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout)
{
    if (!this->is_valid())
    {
        this->m_device = layout.device();
        this->m_pool = pool;
        this->internal_create(pool, layout);
    }
}

template<uint32_t S>
void vka::DescriptorSetArray<S>::destroy_handles() noexcept
{
    if (this->is_valid())
        vkFreeDescriptorSets(this->m_device, this->m_pool, S, this->m_sets.data());
}

template<uint32_t S>
inline void vka::DescriptorSetArray<S>::destroy() noexcept
{
    this->destroy_handles();
    this->m_device = VK_NULL_HANDLE;
    this->m_pool = VK_NULL_HANDLE;
    this->m_sets.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
inline void vka::DescriptorSetArray<S>::bind(VkCommandBuffer cbo, VkPipelineBindPoint bind_point, VkPipelineLayout pipe_layout) const
{
    if (this->is_valid())
        vkCmdBindDescriptorSets(cbo, bind_point, pipe_layout, 0, S, this->m_sets.data(), 0, nullptr);
}

template<uint32_t S>
constexpr uint32_t vka::DescriptorSetArray<S>::count() noexcept
{
    return S;
}

template<uint32_t S>
inline VkDescriptorSet vka::DescriptorSetArray<S>::handle(size_t idx) const
{
    return this->m_sets.at(idx);
}

template<uint32_t S>
inline VkDescriptorSet vka::DescriptorSetArray<S>::handle_u(size_t idx) const
{
    return this->m_sets[idx];
}

template<uint32_t S>
inline VkDescriptorSet vka::DescriptorSetArray<S>::operator[] (size_t idx) const noexcept
{
    return this->m_sets[idx];
}

template<uint32_t S>
inline const VkDescriptorSet* vka::DescriptorSetArray<S>::sets() const noexcept
{
    return this->m_sets.data();
}

template<uint32_t S>
inline VkDevice vka::DescriptorSetArray<S>::device() const noexcept
{
    return this->m_device;
}

template<uint32_t S>
inline VkDescriptorPool vka::DescriptorSetArray<S>::pool() const noexcept
{
    return this->m_pool;
}

template<uint32_t S>
inline bool vka::DescriptorSetArray<S>::is_valid() const noexcept
{
    // either all are valid or none is valid
    return (this->m_sets[S-1] != VK_NULL_HANDLE);
}

template<uint32_t S>
template<uint32_t N>
inline vka::DescriptorUpdateOperation<S, N> vka::DescriptorSetArray<S>::op_update() const
{
    if (!this->is_valid()) [[unlikely]]
        detail::error::throw_runtime_error(MSG_UPDATE_OP);
    return vka::DescriptorUpdateOperation<S, N>(*this);
}
