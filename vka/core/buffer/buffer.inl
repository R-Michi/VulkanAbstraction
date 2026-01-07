/**
 * @brief Inline implementation for the buffer class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "buffer.h"

constexpr vka::Buffer::Buffer() noexcept :
    m_size(0),
    m_map_status(false)
{}

inline vka::Buffer::Buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info) :
    m_buffer(device , create_buffer(device, properties, create_info)),
    m_size(create_info.bufferSize),
    m_map_status(false)
{}

constexpr vka::Buffer::Buffer(Buffer&& src) noexcept :
    m_buffer(std::move(src.m_buffer)),
    m_size(src.m_size),
    m_map_status(src.m_map_status)
{
    src.m_map_status = false;
}

constexpr vka::Buffer::~Buffer()
{
    this->unmap_memory();
}

constexpr vka::Buffer& vka::Buffer::operator= (Buffer&& src) noexcept
{
    this->unmap_memory();
    this->m_buffer = std::move(src.m_buffer);
    this->m_size = src.m_size;
    this->m_map_status = src.m_map_status;
    src.m_map_status = false;
    return *this;
}

constexpr vka::Buffer::operator bool() const noexcept
{
    return (bool)this->m_buffer;
}

constexpr VkDeviceSize vka::Buffer::size() const noexcept
{
    return this->m_size;
}

constexpr VkBuffer vka::Buffer::handle() const noexcept
{
    return this->m_buffer.get().buffer;
}

constexpr VkDevice vka::Buffer::parent() const noexcept
{
    return this->m_buffer.parent();
}

inline VkDeviceAddress vka::Buffer::device_address() const noexcept
{
    const VkBufferDeviceAddressInfo info = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
        .pNext = nullptr,
        .buffer = this->m_buffer.get().buffer
    };
    return vkGetBufferDeviceAddress(this->m_buffer.parent(), &info);
}

inline void* vka::Buffer::map()
{
    return this->map(0, this->m_size);
}

inline void* vka::Buffer::map(VkDeviceSize offset, VkDeviceSize size)
{
    void* data;
    check_result(
        vkMapMemory(this->m_buffer.parent(), this->m_buffer.get().memory, offset, size, 0, &data),
        MAP_MEMORY_FAILED
    );
    this->m_map_status = true;
    return data;
}

constexpr void vka::Buffer::unmap_memory() const noexcept
{
    if (this->m_map_status)
        vkUnmapMemory(this->m_buffer.parent(), this->m_buffer.get().memory);
}

inline void vka::Buffer::unmap() noexcept
{
    this->unmap_memory();
    this->m_map_status = false;
}

// ReSharper disable once CppMemberFunctionMayBeConst
inline void vka::Buffer::copy(VkCommandBuffer cbo, const Buffer& src) noexcept
{
    const VkBufferCopy region = { 0, 0, src.m_size };
    vkCmdCopyBuffer(cbo, src.m_buffer.get().buffer, this->m_buffer.get().buffer, 1, &region);
}

// ReSharper disable once CppMemberFunctionMayBeConst
inline void vka::Buffer::copy_region(VkCommandBuffer cbo, const Buffer& src, const VkBufferCopy& region) noexcept
{
    VkBufferCopy final_region = region;
    if (region.size == 0)
        final_region.size = src.size() - region.srcOffset;
    vkCmdCopyBuffer(cbo, src.m_buffer.get().buffer, this->m_buffer.get().buffer, 1, &final_region);
}
