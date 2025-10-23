/**
 * @brief Includes implementation details for descriptors.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::detail::descriptor
{
    /**
     * @brief Creates the write-structure for a buffer.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos) noexcept;

    /**
     * @brief Creates the write-structure for an image / texture.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos) noexcept;

    /**
     * @brief Creates the write-structure for a buffer view.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views) noexcept;

    /**
     * @brief Creates the write-structure for a NV-acceleration-structure (NV = Nvidia).
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as_write) noexcept;

    /**
     * @brief Creates the write-structure for a KHR-acceleration-structure.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as_write) noexcept;

    /**
     * @brief Creates the write-structure for an inline uniform block.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write) noexcept;
}
