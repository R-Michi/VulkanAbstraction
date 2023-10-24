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
    : device(device), buffer(VK_NULL_HANDLE), memory(VK_NULL_HANDLE), b_size(0), mapped(false)
{}

vka::Buffer::Buffer(Buffer&& src) noexcept
    : device(src.device), buffer(src.buffer), memory(src.memory), b_size(src.b_size), mapped(src.mapped)
{
    src.buffer = VK_NULL_HANDLE;
    src.memory = VK_NULL_HANDLE;
    src.b_size = 0;
    src.mapped = false;
}

vka::Buffer& vka::Buffer::operator= (Buffer&& src) noexcept
{
    // destroyes the buffer, if it has been created, otherwise this function does nothing
    this->destroy_handles();
    this->device = src.device;
    this->buffer = src.buffer;
    this->memory = src.memory;
    this->b_size = src.b_size;
    this->mapped = src.mapped;
    src.buffer = VK_NULL_HANDLE;
    src.memory = VK_NULL_HANDLE;
    src.b_size = 0;
    src.mapped = false;
    return *this;
}

vka::Buffer::~Buffer(void)
{
    this->destroy_handles();
}

void vka::Buffer::init(VkDevice device) noexcept
{
    if (!this->is_valid())
        this->device = device;
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
        detail::error::check_result(vkCreateBuffer(this->device, &buffer_ci, nullptr, &this->buffer), BUFFER_CREATE_FAILED);
        this->b_size = buffer_ci.size;

        // query memory requierements
        VkMemoryRequirements requierements;
        vkGetBufferMemoryRequirements(this->device, this->buffer, &requierements);

        // allocate memory
        const VkMemoryAllocateInfo memory_ai = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = requierements.size,
            .memoryTypeIndex = memory::find_type_index(properties, requierements.memoryTypeBits, create_info.memoryPropertyFlags)
        };
        detail::error::check_result(vkAllocateMemory(this->device, &memory_ai, nullptr, &this->memory), ALLOC_MEMORY_FAILED);
        detail::error::check_result(vkBindBufferMemory(this->device, this->buffer, this->memory, 0), BIND_MEMORY_FAILED);
    }
}

void vka::Buffer::destroy(void) noexcept
{
    this->destroy_handles();
    this->buffer = VK_NULL_HANDLE;
    this->memory = VK_NULL_HANDLE;
    this->b_size = 0;
    this->mapped = false;
}
