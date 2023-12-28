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

/************************************************ DescriptorLayoutArray ***********************************************/

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>::DescriptorLayoutArray(void) noexcept :
    m_device(VK_NULL_HANDLE), m_layouts{VK_NULL_HANDLE}
{}

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>::DescriptorLayoutArray(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags) :
    m_device(device), m_layouts{VK_NULL_HANDLE}
{
    this->internal_create(device, set_bindings, flags);
}

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>::DescriptorLayoutArray(DescriptorLayoutArray&& src) noexcept :
    m_device(src.m_device), m_layouts(src.m_layouts)
{
    src.m_layouts.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>& vka::DescriptorLayoutArray<S>::operator= (DescriptorLayoutArray&& src) noexcept
{
    // destroy_handles does nothing, if the layouts have not been created
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_layouts = src.m_layouts;
    src.m_layouts.fill(VK_NULL_HANDLE);
    return *this;
}

template<uint32_t S>
inline vka::DescriptorLayoutArray<S>::~DescriptorLayoutArray(void)
{
    this->destroy_handles();
}

template<uint32_t S>
void vka::DescriptorLayoutArray<S>::internal_create(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags)
{
    for (uint32_t i = 0; i < S; i++)    // number of sets must match the number of layouts, otherwise compiler error
    {
        const VkDescriptorSetLayoutCreateInfo ci = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = flags,
            .bindingCount = set_bindings.binding_count(i),
            .pBindings = set_bindings.bindings(i)
        };
        detail::error::check_result(vkCreateDescriptorSetLayout(device, &ci, nullptr, this->m_layouts.data() + i), MSG_CREATE_FAILED);
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
VKA_NOINLINE void vka::DescriptorLayoutArray<S>::destroy_handles(void) noexcept
{
    for (uint32_t i = 0; i < S; i++)
    {
        if (this->m_layouts[i] != VK_NULL_HANDLE)
            vkDestroyDescriptorSetLayout(this->m_device, this->m_layouts[i], nullptr);
    }
}

template<uint32_t S>
inline void vka::DescriptorLayoutArray<S>::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_device = VK_NULL_HANDLE;
    this->m_layouts.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
inline vka::DescriptorSetArray<S> vka::DescriptorLayoutArray<S>::create_sets(VkDescriptorPool pool)
{
    return DescriptorSetArray<S>(pool, *this);
}

template<uint32_t S>
constexpr uint32_t vka::DescriptorLayoutArray<S>::count(void) const noexcept
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
inline const VkDescriptorSetLayout* vka::DescriptorLayoutArray<S>::layouts(void) const noexcept
{
    return this->m_layouts.data();
}

template<uint32_t S>
inline VkDevice vka::DescriptorLayoutArray<S>::device(void) const noexcept
{
    return this->m_device;
}

template<uint32_t S>
inline bool vka::DescriptorLayoutArray<S>::is_valid(void) const noexcept
{
    // if the last one is valid -> all are valid
    return (this->m_layouts[S-1] != VK_NULL_HANDLE);
}

/************************************************** DescriptorSetArray ************************************************/

template<uint32_t S>
inline vka::DescriptorSetArray<S>::DescriptorSetArray(void) noexcept
    : m_device(VK_NULL_HANDLE), m_pool(VK_NULL_HANDLE), m_sets{VK_NULL_HANDLE}
{}

template<uint32_t S>
inline vka::DescriptorSetArray<S>::DescriptorSetArray(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout)
    : m_device(layout.device()), m_pool(pool), m_sets{VK_NULL_HANDLE}
{
    this->internal_create(pool, layout);
}

template<uint32_t S>
inline vka::DescriptorSetArray<S>::DescriptorSetArray(DescriptorSetArray&& src) noexcept
    : m_device(src.m_device), m_pool(src.m_pool), m_sets(src.m_sets)
{
    src.m_sets.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
inline vka::DescriptorSetArray<S>& vka::DescriptorSetArray<S>::operator= (DescriptorSetArray&& src) noexcept
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
inline vka::DescriptorSetArray<S>::~DescriptorSetArray(void)
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
    detail::error::check_result(vkAllocateDescriptorSets(layout.device(), &ai, this->m_sets.data()), MSG_CREATE_FAILED);
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
VKA_NOINLINE void vka::DescriptorSetArray<S>::destroy_handles(void) noexcept
{
    if (this->is_valid())
        vkFreeDescriptorSets(this->m_device, this->m_pool, S, this->m_sets.data());
}

template<uint32_t S>
inline void vka::DescriptorSetArray<S>::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_device = VK_NULL_HANDLE;
    this->m_pool = VK_NULL_HANDLE;
    this->m_sets.fill(VK_NULL_HANDLE);
}

template<uint32_t S>
template<uint32_t N>
inline vka::DescriptorSetArray<S>::UpdateOperation<N> vka::DescriptorSetArray<S>::op_update(void)
{
    if (!this->is_valid()) [[unlikely]]
        detail::error::throw_runtime_error(MSG_UPDATE_OP);
    return UpdateOperation<N>(*this);
}

template<uint32_t S>
inline void vka::DescriptorSetArray<S>::bind(VkCommandBuffer cbo, VkPipelineBindPoint bind_point, VkPipelineLayout pipe_layout) const
{
    if (this->is_valid())
        vkCmdBindDescriptorSets(cbo, bind_point, pipe_layout, 0, S, this->m_sets.data(), 0, nullptr);
}

template<uint32_t S>
constexpr uint32_t vka::DescriptorSetArray<S>::count(void) const noexcept
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
inline const VkDescriptorSet* vka::DescriptorSetArray<S>::sets(void) const noexcept
{
    return this->m_sets.data();
}

template<uint32_t S>
inline VkDevice vka::DescriptorSetArray<S>::device(void) const noexcept
{
    return this->m_device;
}

template<uint32_t S>
inline VkDescriptorPool vka::DescriptorSetArray<S>::pool(void) const noexcept
{
    return this->m_pool;
}

template<uint32_t S>
inline bool vka::DescriptorSetArray<S>::is_valid(void) const noexcept
{
    // either all are valid or none is valid
    return (this->m_sets[S-1] != VK_NULL_HANDLE);
}

#if 0
/*********************************** DescriptorInfoArray<VkDescriptorBufferInfo, N> ***********************************/

template<uint32_t N>
inline VkWriteDescriptorSet vka::DescriptorInfoArray<VkDescriptorBufferInfo, N>::make_set(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, uint32_t count) const noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = detail::descriptor::min<N>(count, this->size()),
        .descriptorType = type,
        .pImageInfo = nullptr,
        .pBufferInfo = this->data(),
        .pTexelBufferView = nullptr
    };
}

/************************************ DescriptorInfoArray<VkDescriptorImageInfo, N> ***********************************/

template<uint32_t N>
inline VkWriteDescriptorSet vka::DescriptorInfoArray<VkDescriptorImageInfo, N>::make_set(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, uint32_t count) const noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = detail::descriptor::min<N>(count, this->size()),
        .descriptorType = type,
        .pImageInfo = this->data(),
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}

/**************************************** DescriptorInfoArray<VkBufferView, N> ****************************************/

template<uint32_t N>
inline VkWriteDescriptorSet vka::DescriptorInfoArray<VkBufferView, N>::make_set(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, uint32_t count) const noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = nullptr,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = detail::descriptor::min<N>(count, this->size()),
        .descriptorType = type,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = this->data()
    };
}

/********************************** DescriptorInfoArray<VkAccelerationStructureNV, N> *********************************/

template<uint32_t N>
vka::DescriptorInfoArray<VkAccelerationStructureNV, N>::DescriptorInfoArray(void) :
    m_extension({ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV, .pNext = nullptr })
{}

template<uint32_t N>
inline VkWriteDescriptorSet vka::DescriptorInfoArray<VkAccelerationStructureNV, N>::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count)
{
    this->m_extension.accelerationStructureCount = detail::descriptor::min<N>(count, this->size());
    this->m_extension.pAccelerationStructures = this->data();

    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = &this->m_extension,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = detail::descriptor::min<N>(count, this->size()), // must also be set according to vulkan standard
        .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}

/********************************* DescriptorInfoArray<VkAccelerationStructureKHR, N> *********************************/

template<uint32_t N>
vka::DescriptorInfoArray<VkAccelerationStructureKHR, N>::DescriptorInfoArray(void) :
    m_extension({ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR, .pNext = nullptr })
{}

template<uint32_t N>
inline VkWriteDescriptorSet vka::DescriptorInfoArray<VkAccelerationStructureKHR, N>::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count)
{
    this->m_extension.accelerationStructureCount = detail::descriptor::min<N>(count, this->size());
    this->m_extension.pAccelerationStructures = this->data();

    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = &this->m_extension,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = offset,
        .descriptorCount = detail::descriptor::min<N>(count, this->size()), // must also be set according to vulkan standard
        .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}

/***************************************** DescriptorInfoArray<const void*, 0> ****************************************/

vka::DescriptorInfoArray<const void*, 0>::DescriptorInfoArray(void) :
    m_extension({ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK, .pNext = nullptr })
{}

vka::DescriptorInfoArray<const void*, 0>::DescriptorInfoArray(const void* data, uint32_t size) :
    detail::descriptor::DescriptorInfoArray<const void *, 0>(data, size), m_extension({})
{}

inline VkWriteDescriptorSet vka::DescriptorInfoArray<const void*, 0>::make_write(VkDescriptorSet set, uint32_t binding)
{
    this->m_extension.dataSize = this->size();
    this->m_extension.pData = this->data();

    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = &this->m_extension,
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = 0,   // TODO: define byte offset
        .descriptorCount = this->size(),
        .descriptorType = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}
#endif
