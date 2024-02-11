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

void vka::Buffer::destroy_handles(void) noexcept
{
    if (this->m_mapped)
        vkUnmapMemory(this->m_device, this->m_memory);
    if (this->m_memory != VK_NULL_HANDLE)
        vkFreeMemory(this->m_device, this->m_memory, nullptr);
    if (this->m_buffer != VK_NULL_HANDLE)
        vkDestroyBuffer(this->m_device, this->m_buffer, nullptr);
}

void vka::Buffer::internal_create(const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info)
{
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
