/**
 * @brief Inline implementation for the buffer class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

inline vka::Buffer::Buffer() noexcept :
    m_device(VK_NULL_HANDLE),
    m_buffer(VK_NULL_HANDLE),
    m_memory(VK_NULL_HANDLE),
    m_size(0),
    m_mapped(false)
{}

inline vka::Buffer::Buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info) :
    m_device(device),
    m_buffer(VK_NULL_HANDLE),
    m_memory(VK_NULL_HANDLE),
    m_size(0),
    m_mapped(false)
{
    this->internal_create(properties, create_info);
}

inline vka::Buffer::Buffer(Buffer&& src) noexcept :
    m_device(src.m_device),
    m_buffer(src.m_buffer),
    m_memory(src.m_memory),
    m_size(src.m_size),
    m_mapped(src.m_mapped)
{
    src.m_buffer = VK_NULL_HANDLE;
    src.m_memory = VK_NULL_HANDLE;
    src.m_size = 0;
    src.m_mapped = false;
}

inline vka::Buffer& vka::Buffer::operator= (Buffer&& src) noexcept
{
    // destroys the buffer, if it has been created, otherwise this function does nothing
    this->destroy_handles();

    // copy from source
    this->m_device = src.m_device;
    this->m_buffer = src.m_buffer;
    this->m_memory = src.m_memory;
    this->m_size = src.m_size;
    this->m_mapped = src.m_mapped;

    // reset source
    src.m_buffer = VK_NULL_HANDLE;
    src.m_memory = VK_NULL_HANDLE;
    src.m_size = 0;
    src.m_mapped = false;

    return *this;
}

inline vka::Buffer::~Buffer()
{
    this->destroy_handles();
}

inline void vka::Buffer::create(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info)
{
    if (!this->is_valid())
    {
        this->m_device = device;
        this->internal_create(properties, create_info);
    }
}

inline void vka::Buffer::destroy() noexcept
{
    this->destroy_handles();

    // reset values
    this->m_buffer = VK_NULL_HANDLE;
    this->m_memory = VK_NULL_HANDLE;
    this->m_size = 0;
    this->m_mapped = false;
}

inline void* vka::Buffer::map(VkDeviceSize offset, VkDeviceSize size) noexcept
{
    void* data;
    check_result(vkMapMemory(this->m_device, this->m_memory, offset, size, 0, &data), MAP_MEMORY_FAILED); // does also check if the buffer is invalid
    this->m_mapped = true;
    return data;
}

inline void vka::Buffer::unmap() noexcept
{
    if (this->m_mapped)
    {
        vkUnmapMemory(this->m_device, this->m_memory);
        this->m_mapped = false;
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
inline void vka::Buffer::copy(VkCommandBuffer cbo, const Buffer& src) noexcept
{
    if (this->is_valid() && src.is_valid())
    {
        const VkBufferCopy region = { 0, 0, src.m_size };
        vkCmdCopyBuffer(cbo, src.m_buffer, this->m_buffer, 1, &region);
    }
}

// ReSharper disable once CppMemberFunctionMayBeConst
inline void vka::Buffer::copy_region(VkCommandBuffer cbo, const Buffer& src, const VkBufferCopy& region) noexcept
{
    if (this->is_valid() && src.is_valid())
    {
        VkBufferCopy final_region = region;
        if (region.size == 0)
            final_region.size = src.size() - region.srcOffset;
        vkCmdCopyBuffer(cbo, src.m_buffer, this->m_buffer, 1, &final_region);
    }
}

inline VkDeviceSize vka::Buffer::size() const noexcept
{
    return this->m_size;
}

inline VkBuffer vka::Buffer::handle() const noexcept
{
    return this->m_buffer;
}

inline bool vka::Buffer::is_valid() const noexcept
{
    // memory is the last handle created
    return (this->m_memory != VK_NULL_HANDLE);
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
