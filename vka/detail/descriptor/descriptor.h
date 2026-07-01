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
    struct Parent
    {
        VkDevice device;
        VkDescriptorPool pool;
    };

    struct Handle
    {
        const VkDescriptorSet* sets;
        uint32_t count;

        explicit constexpr operator bool() const noexcept { return this->sets != nullptr; }
    };

    /// Frees the descriptor sets.
    inline void destroy(Parent parent, Handle handle, const VkAllocationCallbacks* allocator);

    /**
     * Creates the write-structure for a buffer.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos) noexcept;

    /**
     * Creates the write-structure for an image.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos) noexcept;

    /**
     * Creates the write-structure for a buffer view.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views) noexcept;

    /**
     * Creates the write-structure for a NV-acceleration-structure (NV = Nvidia).
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as_write) noexcept;

    /**
     * Creates the write-structure for a KHR-acceleration-structure.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as_write) noexcept;

    /**
     * Creates the write-structure for an inline uniform block.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write) noexcept;
}
