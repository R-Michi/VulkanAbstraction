/**
* @file     vka_buffer_inline_impl.inl
* @brief    Buffer implementation file of inline functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline void vka::Buffer::destroy_handles(void) noexcept
{
    if (this->mapped)
        vkUnmapMemory(this->device, this->memory);
    if (this->memory != VK_NULL_HANDLE)
        vkFreeMemory(this->device, this->memory, nullptr);
    if (this->buffer != VK_NULL_HANDLE)
        vkDestroyBuffer(this->device, this->buffer, nullptr);
}

inline VkDeviceSize vka::Buffer::size(void) const noexcept
{
    return this->memory_size;
}

inline VkBuffer vka::Buffer::handle(void) const noexcept
{
    return this->buffer;
}

inline bool vka::Buffer::is_valid(void) const noexcept
{
    // memory is the last handle created
    return (this->memory != VK_NULL_HANDLE);
}

inline void vka::Buffer::validate(void)
{
    if (this->device == VK_NULL_HANDLE) [[unlikely]]
        detail::error::throw_invalid_argument("[vka::Buffer::create]: Device is a VK_NULL_HANDLE.");
}

inline bool vka::Buffer::is_copy_invalid(const Buffer& src, const Buffer& dst, const VkBufferCopy* region) noexcept
{
    // every error case
    return (
        !src.is_valid() ||
        !dst.is_valid() ||
        (region == nullptr && dst.size() < src.size()) ||
        (region != nullptr && (
            dst.size() < (region->dstOffset + region->size) ||
            region->srcOffset >= src.size() ||
            region->dstOffset >= dst.size() ||
            (region->srcOffset + region->size) > src.size() ||
            (region->size == 0 && dst.size() < (region->dstOffset + src.size() - region->srcOffset))
        ))
    );
}

inline void vka::Buffer::is_copy_invalid(uint32_t count, const Buffer* src, const Buffer* dst, bool* results, const VkBufferCopy* regions) noexcept
{
    for (uint32_t i = 0; i < count; i++)
        results[i] = is_copy_invalid(src[i], dst[i], (regions == nullptr ? nullptr : regions + i));
}

inline void vka::Buffer::copy(VkCommandBuffer cbo, const Buffer& src) noexcept
{
    const VkBufferCopy region = { 0, 0, src.memory_size };
    vkCmdCopyBuffer(cbo, src.buffer, this->buffer, 1, &region);
}

inline void vka::Buffer::copy_region(VkCommandBuffer cbo, const Buffer& src, const VkBufferCopy& region) noexcept
{
    VkBufferCopy final_region = region;
    if (region.size == 0)
        final_region.size = src.size() - region.srcOffset;
    vkCmdCopyBuffer(cbo, src.buffer, this->buffer, 1, &final_region);
}

inline void* vka::Buffer::map(VkDeviceSize offset, VkDeviceSize size) noexcept
{
    if (!this->is_valid()) return nullptr;

    void* data;
    if (vkMapMemory(this->device, this->memory, offset, size, 0, &data) != VK_SUCCESS)
        return nullptr;
    this->mapped = true;
    return data;
}

inline void vka::Buffer::unmap(void) noexcept
{
    if (this->is_valid())
    {
        vkUnmapMemory(this->device, this->memory);
        this->mapped = false;
    }
}
