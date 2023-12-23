/**
* @file     vka_descriptor_inline_impl.inl
* @brief    Descriptor implementation file of inline functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

/********************************************** DescriptorSetBindingList **********************************************/

template<uint32_t S>
inline uint32_t vka::DescriptorSetBindingList<S>::binding_count(uint32_t set) const noexcept
{
    return set >= S ? NSIZE32 : this->m_bindings[set].size();
}

template<uint32_t S>
constexpr uint32_t vka::DescriptorSetBindingList<S>::count(void) const noexcept
{
    return S;
}

template<uint32_t S>
inline const VkDescriptorSetLayoutBinding* vka::DescriptorSetBindingList<S>::bindings(uint32_t set) const noexcept
{
    return set >= S ? nullptr : this->m_bindings[set].data();
}

template<uint32_t S>
void vka::DescriptorSetBindingList<S>::push(uint32_t set, VkDescriptorType type, VkShaderStageFlags stages, uint32_t count, const VkSampler* immutable_samplers)
{
    if (set >= S) [[unlikely]]
        detail::error::throw_out_of_range("[vka::DescriptorSetBindingList::push]: Invalid descriptor set index.");

    const VkDescriptorSetLayoutBinding binding = {
        .binding = (uint32_t)this->m_bindings[set].size(),
        .descriptorType = type,
        .descriptorCount = count,
        .stageFlags = stages,
        .pImmutableSamplers = immutable_samplers
    };
    this->m_bindings[set].push_back(binding);
}

/**************************************************** make helpers ****************************************************/

inline VkDescriptorBufferInfo vka::descriptor::make_buffer_info(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range) noexcept
{
    return { buffer, offset, range };
}

inline VkDescriptorBufferInfo vka::descriptor::make_buffer_info(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize range) noexcept
{
    return { buffer.handle(), offset, range };
}

inline VkDescriptorImageInfo vka::descriptor::make_image_info(VkImageView view, VkImageLayout layout, VkSampler sampler) noexcept
{
    return { sampler, view, layout };
}

inline VkDescriptorImageInfo vka::descriptor::make_image_info(const Texture& texture, uint32_t view_index)
{
    return { texture.sampler(), texture.view(view_index), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
}

inline VkWriteDescriptorSetAccelerationStructureNV vka::descriptor::make_write_asnv(const VkAccelerationStructureNV* as, uint32_t count) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV,
        .pNext = nullptr,
        .accelerationStructureCount = count,
        .pAccelerationStructures = as
    };
}

inline VkWriteDescriptorSetAccelerationStructureKHR vka::descriptor::make_write_askhr(const VkAccelerationStructureKHR* as, uint32_t count) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR,
        .pNext = nullptr,
        .accelerationStructureCount = count,
        .pAccelerationStructures = as
    };
}

inline VkWriteDescriptorSetInlineUniformBlock vka::descriptor::make_inline_uniform_block(const void* data, uint32_t size) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK,
        .pNext = nullptr,
        .dataSize = size,
        .pData = data
    };
}

inline VkWriteDescriptorSet vka::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = count,
        .descriptorType = type,
        .pImageInfo = nullptr,
        .pBufferInfo = infos,
        .pTexelBufferView = nullptr
    };
}

inline VkWriteDescriptorSet vka::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = count,
        .descriptorType = type,
        .pImageInfo = infos,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}

inline VkWriteDescriptorSet vka::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = count,
        .descriptorType = type,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = views
    };
}

inline VkWriteDescriptorSet vka::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as_write) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = &as_write,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = as_write.accelerationStructureCount,
        .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}

inline VkWriteDescriptorSet vka::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as_write) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = &as_write,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = as_write.accelerationStructureCount,
        .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}

inline VkWriteDescriptorSet vka::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = &iub_write,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = iub_write.dataSize,
        .descriptorType = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}

/*********************************************** DescriptorManagerLayout **********************************************/

template<uint32_t S>
inline vka::DescriptorManagerLayout<S>::DescriptorManagerLayout(void) noexcept
    : m_device(VK_NULL_HANDLE), m_layouts{VK_NULL_HANDLE}
{}

template<uint32_t S>
inline vka::DescriptorManagerLayout<S>::DescriptorManagerLayout(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags)
    : m_device(device), m_layouts{VK_NULL_HANDLE}
{
    this->create(device, set_bindings, flags);
}

template<uint32_t S>
inline vka::DescriptorManagerLayout<S>::DescriptorManagerLayout(DescriptorManagerLayout&& src) noexcept
    : m_device(src.m_device), m_layouts(src.m_layouts)
{
    src.m_layouts.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
inline vka::DescriptorManagerLayout<S>& vka::DescriptorManagerLayout<S>::operator= (DescriptorManagerLayout&& src) noexcept
{
    // destroy_handles does nothing, if the layouts have not been created
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_layouts = src.m_layouts;
    src.m_layouts.fill(VK_NULL_HANDLE);
    return *this;
}

template<uint32_t S>
inline vka::DescriptorManagerLayout<S>::~DescriptorManagerLayout(void)
{
    this->destroy_handles();
}

template<uint32_t S>
void vka::DescriptorManagerLayout<S>::create(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags)
{
    if (!this->is_valid())
    {
        this->m_device = device;
        for (uint32_t i = 0; i < S; i++)    // number of sets must match the number of layouts, otherwise compiler error
        {
            const VkDescriptorSetLayoutCreateInfo ci = {
                .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                .pNext = nullptr,
                .flags = flags,
                .bindingCount = set_bindings.binding_count(i),
                .pBindings = set_bindings.bindings(i)
            };
            detail::error::check_result(vkCreateDescriptorSetLayout(device, &ci, nullptr, &this->m_layouts[i]),
                                        "[vka::DescriptorManagerLayout::create]: Failed to create descriptor set layout.");
        }
    }
}

template<uint32_t S>
VKA_NOINLINE void vka::DescriptorManagerLayout<S>::destroy_handles(void) noexcept
{
    for (uint32_t i = 0; i < S; i++)
    {
        if (this->m_layouts[i] != VK_NULL_HANDLE)
            vkDestroyDescriptorSetLayout(this->m_device, this->m_layouts[i], nullptr);
    }
}

template<uint32_t S>
inline void vka::DescriptorManagerLayout<S>::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_layouts.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
constexpr uint32_t vka::DescriptorManagerLayout<S>::count(void) const noexcept
{
    return S;
}

template<uint32_t S>
inline VkDescriptorSetLayout vka::DescriptorManagerLayout<S>::handle(size_t idx) const
{
    return this->m_layouts.at(idx);
}

template<uint32_t S>
inline VkDescriptorSetLayout vka::DescriptorManagerLayout<S>::operator[] (size_t idx) const noexcept
{
    return this->m_layouts[idx];
}

template<uint32_t S>
inline const VkDescriptorSetLayout* vka::DescriptorManagerLayout<S>::layouts(void) const noexcept
{
    return this->m_layouts.data();
}

template<uint32_t S>
inline bool vka::DescriptorManagerLayout<S>::is_valid(void) const noexcept
{
    // if the last one is valid -> all are valid
    return (this->m_layouts[S-1] != VK_NULL_HANDLE);
}

/************************************************** DescriptorManager *************************************************/

template<uint32_t S>
inline vka::DescriptorManager<S>::DescriptorManager(void) noexcept
    : m_device(VK_NULL_HANDLE), m_pool(VK_NULL_HANDLE), m_sets{VK_NULL_HANDLE}
{}

template<uint32_t S>
inline vka::DescriptorManager<S>::DescriptorManager(VkDevice device, VkDescriptorPool pool, const DescriptorManagerLayout<S>& layout)
    : m_device(device), m_pool(pool), m_sets{VK_NULL_HANDLE}
{
    this->create(device, pool, layout);
}

template<uint32_t S>
inline vka::DescriptorManager<S>::DescriptorManager(DescriptorManager&& src) noexcept
    : m_device(src.m_device), m_pool(src.m_pool), m_sets(src.m_sets)
{
    src.m_sets.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
inline vka::DescriptorManager<S>& vka::DescriptorManager<S>::operator= (DescriptorManager&& src) noexcept
{
    // destroy_handles does nothing, if the sets have not been allocated
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_pool = src.m_pool;
    this->m_sets = src.m_sets;
    src.m_sets.fill(VK_NULL_HANDLE);
    return *this;
}

template<uint32_t S>
inline vka::DescriptorManager<S>::~DescriptorManager(void)
{
    this->destroy_handles();
}

template<uint32_t S>
void vka::DescriptorManager<S>::create(VkDevice device, VkDescriptorPool pool, const DescriptorManagerLayout<S>& layout)
{
    if (!this->is_valid())
    {
        this->m_device = device;
        this->m_pool = pool;

        const VkDescriptorSetAllocateInfo ai = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = pool,
            .descriptorSetCount = S,
            .pSetLayouts = layout.layouts()
        };
        detail::error::check_result(vkAllocateDescriptorSets(device, &ai, this->m_sets.data()),
                                    "[vka::DescriptorManager::create]: Failed to allocate descriptor sets.");
    }
}

template<uint32_t S>
VKA_NOINLINE void vka::DescriptorManager<S>::destroy_handles(void) noexcept
{
    if (this->is_valid())
        vkFreeDescriptorSets(this->m_device, this->m_pool, S, this->m_sets.data());
}

template<uint32_t S>
inline void vka::DescriptorManager<S>::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_sets.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
inline void vka::DescriptorManager<S>::update(const VkWriteDescriptorSet* writes, uint32_t count) noexcept
{
    if (this->is_valid())
        vkUpdateDescriptorSets(this->m_device, count, writes, 0, nullptr);
}

template<uint32_t S>
inline void vka::DescriptorManager<S>::bind(VkCommandBuffer cbo, VkPipelineBindPoint bind_point, VkPipelineLayout pipe_layout) const noexcept
{
    if (this->is_valid())
        vkCmdBindDescriptorSets(cbo, bind_point, pipe_layout, 0, S, this->m_sets.data(), 0, nullptr);
}

template<uint32_t S>
constexpr uint32_t vka::DescriptorManager<S>::count(void) const noexcept
{
    return S;
}

template<uint32_t S>
inline VkDescriptorSet vka::DescriptorManager<S>::handle(size_t idx) const
{
    return this->m_sets.at(idx);
}

template<uint32_t S>
inline VkDescriptorSet vka::DescriptorManager<S>::operator[] (size_t idx) const noexcept
{
    return this->m_sets[idx];
}

template<uint32_t S>
inline const VkDescriptorSet* vka::DescriptorManager<S>::descriptor_sets(void) const noexcept
{
    return this->m_sets.data();
}

template<uint32_t S>
inline bool vka::DescriptorManager<S>::is_valid(void) const noexcept
{
    // either all are valid or none is valid
    return (this->m_sets[S-1] != VK_NULL_HANDLE);
}
