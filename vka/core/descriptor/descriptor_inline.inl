/**
 * @brief Inline implementation for descriptor functions.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "descriptor_class.h"

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
vka::DescriptorInfoArray<VkAccelerationStructureNV, N>::DescriptorInfoArray() :
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
        .descriptorCount = detail::descriptor::min<N>(count, this->size()), // must also be set according to a vulkan standard
        .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}

/********************************* DescriptorInfoArray<VkAccelerationStructureKHR, N> *********************************/

template<uint32_t N>
vka::DescriptorInfoArray<VkAccelerationStructureKHR, N>::DescriptorInfoArray() :
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
        .descriptorCount = detail::descriptor::min<N>(count, this->size()), // must also be set according to a vulkan standard
        .descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
        .pImageInfo = nullptr,
        .pBufferInfo = nullptr,
        .pTexelBufferView = nullptr
    };
}

/***************************************** DescriptorInfoArray<const void*, 0> ****************************************/

vka::DescriptorInfoArray<const void*, 0>::DescriptorInfoArray() :
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
