/**
* @file     vka_buffer_impl.inl
* @brief    Buffer implementation file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

vka::Buffer::Buffer(VkDevice device) noexcept
    : m_device(device), m_buffer(VK_NULL_HANDLE), m_memory(VK_NULL_HANDLE), m_size(0), m_mapped(false)
{}

vka::Buffer::Buffer(Buffer&& src) noexcept
    : m_device(src.m_device), m_buffer(src.m_buffer), m_memory(src.m_memory), m_size(src.m_size), m_mapped(src.m_mapped)
{
    src.m_buffer = VK_NULL_HANDLE;
    src.m_memory = VK_NULL_HANDLE;
    src.m_size = 0;
    src.m_mapped = false;
}

vka::Buffer& vka::Buffer::operator= (Buffer&& src) noexcept
{
    // destroys the buffer, if it has been created, otherwise this function does nothing
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_buffer = src.m_buffer;
    this->m_memory = src.m_memory;
    this->m_size = src.m_size;
    this->m_mapped = src.m_mapped;
    src.m_buffer = VK_NULL_HANDLE;
    src.m_memory = VK_NULL_HANDLE;
    src.m_size = 0;
    src.m_mapped = false;
    return *this;
}

vka::Buffer::~Buffer(void)
{
    this->destroy_handles();
}

void vka::Buffer::init(VkDevice device) noexcept
{
    if (!this->is_valid())
        this->m_device = device;
}

void vka::Buffer::create(const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info)
{
    if (!this->is_valid())
    {
        this->validate();

        // create buffer handle
        const VkBufferCreateInfo buffer_ci = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = create_info.bufferFlags,
            .size = create_info.bufferSize,
            .usage = create_info.bufferUsage,
            .sharingMode = create_info.bufferSharingMode,
            .queueFamilyIndexCount = create_info.bufferQueueFamilyIndexCount,
            .pQueueFamilyIndices = create_info.bufferQueueFamilyIndices
        };
        detail::error::check_result(vkCreateBuffer(this->m_device, &buffer_ci, nullptr, &this->m_buffer), BUFFER_CREATE_FAILED);
        this->m_size = buffer_ci.size;

        // query memory requirements
        VkMemoryRequirements requirements;
        vkGetBufferMemoryRequirements(this->m_device, this->m_buffer, &requirements);

        // allocate memory
        const VkMemoryAllocateInfo memory_ai = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = requirements.size,
            .memoryTypeIndex = memory::find_type_index(properties, requirements.memoryTypeBits, create_info.memoryPropertyFlags)
        };
        detail::error::check_result(vkAllocateMemory(this->m_device, &memory_ai, nullptr, &this->m_memory), ALLOC_MEMORY_FAILED);
        detail::error::check_result(vkBindBufferMemory(this->m_device, this->m_buffer, this->m_memory, 0), BIND_MEMORY_FAILED);
    }
}

void vka::Buffer::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_buffer = VK_NULL_HANDLE;
    this->m_memory = VK_NULL_HANDLE;
    this->m_size = 0;
    this->m_mapped = false;
}
