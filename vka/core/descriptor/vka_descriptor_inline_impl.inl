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

inline uint32_t vka::DescriptorSetBindingList::binding_count(uint32_t set) const noexcept
{
    return set >= this->m_bindings.size() ? NSIZE32 : this->m_bindings[set].size();
}

inline uint32_t vka::DescriptorSetBindingList::count(void) const noexcept
{
    return this->m_bindings.size();
}

/*************************************************** BufferInfoList ***************************************************/

template<uint32_t S>
inline void vka::BufferInfoList<S>::push(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range)
{
    if (this->m_idx >= S) [[unlikely]]
        detail::error::throw_out_of_range("[vka::BufferInfoList]: Out of memory.");
    this->push2(buffer, offset, range);
}

template<uint32_t S>
inline void vka::BufferInfoList<S>::push(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize range)
{
    if (this->m_idx >= S) [[unlikely]]
        detail::error::throw_out_of_range("[vka::BufferInfoList]: Out of memory.");
    this->push2(buffer, offset, range);
}

template<uint32_t S>
inline void vka::BufferInfoList<S>::push2(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range) noexcept
{
    this->m_infos[this->m_idx++] = { buffer, offset, range };
}

template<uint32_t S>
inline void vka::BufferInfoList<S>::push2(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize range) noexcept
{
    this->m_infos[this->m_idx++] = { buffer.handle(), offset, range };
}

inline void vka::BufferInfoList<0>::push(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range)
{
    this->m_infos.push_back({ buffer, offset, range });
}

inline void vka::BufferInfoList<0>::push(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize range)
{
    this->m_infos.push_back({ buffer.handle(), offset, range });
}

/**************************************************** ImageInfoList ***************************************************/

template<uint32_t S>
inline void vka::ImageInfoList<S>::push(VkImageView view, VkImageLayout layout, VkSampler sampler)
{
    if (this->m_idx >= S) [[unlikely]]
        detail::error::throw_out_of_range("[vka::ImageInfoList]: Out of memory.");
    this->push2(view, layout, sampler);
}

template<uint32_t S>
inline void vka::ImageInfoList<S>::push(const Texture& texture, uint32_t view_index)
{
    if (this->m_idx >= S) [[unlikely]]
        detail::error::throw_out_of_range("[vka::ImageInfoList]: Out of memory.");
    this->m_infos[this->m_idx++] = { texture.sampler(), texture.view(view_index), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
}

template<uint32_t S>
inline void vka::ImageInfoList<S>::push2(VkImageView view, VkImageLayout layout, VkSampler sampler) noexcept
{
    this->m_infos[this->m_idx++] = { sampler, view, layout };
}

template<uint32_t S>
inline void vka::ImageInfoList<S>::push2(const Texture& texture, uint32_t view_index) noexcept
{
    this->m_infos[this->m_idx++] = { texture.sampler(), texture.viewu(view_index), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
}

inline void vka::ImageInfoList<0>::push(VkImageView view, VkImageLayout layout, VkSampler sampler)
{
    this->m_infos.push_back({ sampler, view, layout });
}

inline void vka::ImageInfoList<0>::push(const Texture& texture, uint32_t view_index)
{
    this->m_infos.push_back({ texture.sampler(), texture.view(view_index), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
}

/******************************************* AccelerationStructureInfoListNV ******************************************/

template<uint32_t S>
inline void vka::AccelerationStructureInfoListNV<S>::push(VkAccelerationStructureNV as)
{
    if (this->m_idx >= S) [[unlikely]]
        detail::error::throw_out_of_range("[vka::AccelerationStructureInfoListNV]: Out of memory.");
    this->push2(as);
}

template<uint32_t S>
inline void vka::AccelerationStructureInfoListNV<S>::push2(VkAccelerationStructureNV as) noexcept
{
    this->m_infos[this->m_idx++] = as;
}

inline void vka::AccelerationStructureInfoListNV<0>::push(VkAccelerationStructureNV as)
{
    this->m_infos.push_back(as);
}

/******************************************* AccelerationStructureInfoListKHR *****************************************/

template<uint32_t S>
inline void vka::AccelerationStructureInfoListKHR<S>::push(VkAccelerationStructureKHR as)
{
    if (this->m_idx >= S) [[unlikely]]
        detail::error::throw_out_of_range("[vka::AccelerationStructureInfoListKHR]: Out of memory.");
    this->push2(as);
}

template<uint32_t S>
inline void vka::AccelerationStructureInfoListKHR<S>::push2(VkAccelerationStructureKHR as) noexcept
{
    this->m_infos[this->m_idx++] = as;
}

inline void vka::AccelerationStructureInfoListKHR<0>::push(VkAccelerationStructureKHR as)
{
    this->m_infos.push_back(as);
}

/************************************************ InlineUniformBlockInfo **********************************************/

inline void vka::InlineUniformBlockInfo::set(const void* data) noexcept
{
    this->m_data = data;
}

/************************************************* DescriptorWriteList ************************************************/

template<uint32_t S>
constexpr vka::DescriptorWriteList<S>::DescriptorWriteList(void) noexcept :
    m_writes{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET },
    m_writes_ext{},
    m_idx(0)
{}

template<uint32_t S>
template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<S>::init_write(
    VkDescriptorSet                     set,
    uint32_t                            binding,
    uint32_t                            offset,
    uint32_t                            count,
    VkDescriptorType                    type,
    const DescriptorInfoList<Info, S2>& list
)
{
    using detail::descriptor::DescriptorInfoType;
    using detail::descriptor::get_descriptor_info_type;

    this->m_writes[this->m_idx].dstSet = set;
    this->m_writes[this->m_idx].dstBinding = binding;
    this->m_writes[this->m_idx].dstArrayElement = offset;
    this->m_writes[this->m_idx].descriptorCount = count;
    this->m_writes[this->m_idx].descriptorType = type;

    if constexpr (get_descriptor_info_type<Info>::type == DescriptorInfoType::BUFFER_INFO)
        this->m_writes[this->m_idx].pBufferInfo = list.data();
    if constexpr (get_descriptor_info_type<Info>::type == DescriptorInfoType::IMAGE_INFO)
        this->m_writes[this->m_idx].pImageInfo = list.data();
    if constexpr (get_descriptor_info_type<Info>::type == DescriptorInfoType::ACCELERATION_STRUCTURE_NV)
    {
        this->m_writes_ext[this->m_idx].asNV = {
            sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV,
            pNext = nullptr,
            accelerationStructureCount = count,
            pAccelerationStructures = list.data()
        };
        this->m_writes[this->m_idx].pNext = this->m_writes_ext + this->m_idx;
    }
}

template<uint32_t S>
template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<S>::write(
    VkDescriptorSet                     set,
    uint32_t                            binding,
    uint32_t                            offset,
    VkDescriptorType                    type,
    const DescriptorInfoList<Info, S2>& list
)
{

}

template<uint32_t S>
template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<S>::write_const(
    VkDescriptorSet                     set,
    uint32_t                            binding,
    uint32_t                            offset,
    VkDescriptorType                    type,
    const DescriptorInfoList<Info, S2>& list
)
{

}

template<uint32_t S>
template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<S>::write2(
    VkDescriptorSet                     set,
    uint32_t                            binding,
    uint32_t                            offset,
    VkDescriptorType                    type,
    const DescriptorInfoList<Info, S2>& list
) noexcept
{

}

template<uint32_t S>
template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<S>::write2_const(
    VkDescriptorSet set,
    uint32_t binding,
    uint32_t offset,
    VkDescriptorType type,
    const DescriptorInfoList<Info, S2>& list
) noexcept
{

}

