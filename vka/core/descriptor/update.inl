#pragma once

#include "top.h"

constexpr vka::DescriptorUpdateOP::DescriptorUpdateOP(const DescriptorSets& sets) noexcept :
    m_sets(&sets)
{}

inline void vka::DescriptorUpdateOP::execute() const noexcept
{
    vkUpdateDescriptorSets(this->m_sets->parent(), this->m_writes.size(), this->m_writes.data(), 0, nullptr);
}

/// --------------------------------------------------------------------------------------------------------------------
/// --------------------------------------------------------------------------------------------------------------------
/// --------------------------------------------------------------------------------------------------------------------

constexpr VkDescriptorBufferInfo vka::descriptor::make_buffer_info(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range) noexcept
{
    return { buffer, offset, range };
}

constexpr VkDescriptorBufferInfo vka::descriptor::make_buffer_info(const Buffer& buffer, VkDeviceSize offset, VkDeviceSize range) noexcept
{
    return { buffer.handle(), offset, range };
}

constexpr VkDescriptorImageInfo vka::descriptor::make_image_info(VkImageView view, VkImageLayout layout, VkSampler sampler) noexcept
{
    return { sampler, view, layout };
}

constexpr VkDescriptorImageInfo vka::descriptor::make_image_info(const Texture& texture, uint32_t view_index) noexcept
{
    return { texture.sampler(), texture[view_index], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
}

constexpr VkWriteDescriptorSetAccelerationStructureNV vka::descriptor::make_write_asnv(const VkAccelerationStructureNV* as, uint32_t count) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV,
        .pNext = nullptr,
        .accelerationStructureCount = count,
        .pAccelerationStructures = as
    };
}

constexpr VkWriteDescriptorSetAccelerationStructureKHR vka::descriptor::make_write_askhr(const VkAccelerationStructureKHR* as, uint32_t count) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR,
        .pNext = nullptr,
        .accelerationStructureCount = count,
        .pAccelerationStructures = as
    };
}

constexpr VkWriteDescriptorSetInlineUniformBlock vka::descriptor::make_inline_uniform_block(const void* data, uint32_t size) noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK,
        .pNext = nullptr,
        .dataSize = size,
        .pData = data
    };
}
