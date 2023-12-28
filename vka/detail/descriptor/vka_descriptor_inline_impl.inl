/**
* @file     vka_descriptor_inline_impl.inl
* @brief    Detailed implementation of descriptor-manager classes and functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

/**************************************************** make helpers ****************************************************/

inline VkWriteDescriptorSet vka::detail::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos) noexcept
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

inline VkWriteDescriptorSet vka::detail::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos) noexcept
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

inline VkWriteDescriptorSet vka::detail::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views) noexcept
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

inline VkWriteDescriptorSet vka::detail::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as_write) noexcept
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

inline VkWriteDescriptorSet vka::detail::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as_write) noexcept
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

inline VkWriteDescriptorSet vka::detail::descriptor::make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write) noexcept
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
