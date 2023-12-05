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

/************************************************* DescriptorWriteList ************************************************/

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkDescriptorBufferInfo* list)
{

}

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkDescriptorImageInfo* list)
{

}

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkAccelerationStructureNV* list)
{

}

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkAccelerationStructureKHR* list)
{

}

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const void* data)
{

}

template<uint32_t S>
template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<S>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const DescriptorWriteList::DescriptorInfoList<Info, S2>& list)
{

}

template<uint32_t S>
template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<S>::write_const(VkDescriptorSet set, uint32_t binding, uint32_t offset, const DescriptorWriteList::DescriptorInfoList<Info, S2>& list)
{

}

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write2(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkDescriptorBufferInfo* list) noexcept
{
}

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write2(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkDescriptorImageInfo* list) noexcept
{

}

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write2(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkAccelerationStructureNV* list) noexcept
{

}

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write2(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkAccelerationStructureKHR* list) noexcept
{

}

template<uint32_t S>
inline void vka::DescriptorWriteList<S>::write2(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const void* list) noexcept
{

}

template<uint32_t S>
template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<S>::write2(VkDescriptorSet set, uint32_t binding, uint32_t offset, const DescriptorWriteList::DescriptorInfoList<Info, S2>& list) noexcept
{

}

template<uint32_t S>
template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<S>::write2_const(VkDescriptorSet set, uint32_t binding, uint32_t offset, const DescriptorWriteList::DescriptorInfoList<Info, S2>& list) noexcept
{

}


inline void vka::DescriptorWriteList<0>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkDescriptorBufferInfo* list)
{

}

inline void vka::DescriptorWriteList<0>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkDescriptorImageInfo* list)
{

}

inline void vka::DescriptorWriteList<0>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkAccelerationStructureNV* list)
{

}

inline void vka::DescriptorWriteList<0>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const VkAccelerationStructureKHR* list)
{

}

inline void vka::DescriptorWriteList<0>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, const void* data)
{

}

template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<0>::write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const DescriptorWriteList::DescriptorInfoList<Info, S2>& list)
{

}

template<typename Info, uint32_t S2>
inline void vka::DescriptorWriteList<0>::write_const(VkDescriptorSet set, uint32_t binding, uint32_t offset, const DescriptorWriteList::DescriptorInfoList<Info, S2>& list)
{

}