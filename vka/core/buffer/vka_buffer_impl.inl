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
    : device(device), buffer(VK_NULL_HANDLE), memory(VK_NULL_HANDLE), memory_size(0)
{}

vka::Buffer::Buffer(Buffer&& src) noexcept
    : device(src.device), buffer(src.buffer), memory(src.memory), memory_size(src.memory_size)
{
    src.buffer = VK_NULL_HANDLE;
    src.memory = VK_NULL_HANDLE;
    src.memory_size = 0;
}

vka::Buffer& vka::Buffer::operator= (Buffer&& src) noexcept
{
    // destroyes the buffer, if it has been created, otherwise this function does nothing
    this->destroy_handles();
    this->device = src.device;
    this->buffer = src.buffer;
    this->memory = src.memory;
    this->memory_size = src.memory_size;
    src.buffer = VK_NULL_HANDLE;
    src.memory = VK_NULL_HANDLE;
    src.memory_size = 0;
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

VkResult vka::Buffer::create(const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info)
{
    if (this->is_valid()) return VK_SUCCESS;
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
    VkResult res = vkCreateBuffer(this->device, &buffer_ci, nullptr, &this->buffer);
    if (res != VK_SUCCESS) return res;

    // query memory requierements
    VkMemoryRequirements requierements;
    vkGetBufferMemoryRequirements(this->device, this->buffer, &requierements);
    this->memory_size = requierements.size;

    // allocate memory
    const VkMemoryAllocateInfo memory_ai = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = requierements.size,
        .memoryTypeIndex = utility::find_memory_type_index(properties, requierements.memoryTypeBits, create_info.memoryPropertyFlags)
    };
    res = vkAllocateMemory(this->device, &memory_ai, nullptr, &this->memory);
    if (res != VK_SUCCESS) return res;

    // bind memory to buffer
    return vkBindBufferMemory(this->device, this->buffer, this->memory, 0);
}

void vka::Buffer::destroy(void) noexcept
{
    this->destroy_handles();
    this->buffer = VK_NULL_HANDLE;
    this->memory = VK_NULL_HANDLE;
    this->memory_size = 0;
}

VkResult vka::Buffer::begin_copy(VkDevice device, VkCommandPool pool, VkCommandBuffer& cbo) noexcept
{
    const VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    VkCommandBuffer cbo2 = VK_NULL_HANDLE;  // prevent using reference argument, should I ever remove the inline property of this function
    const VkResult res = vkAllocateCommandBuffers(device, &alloc_info, &cbo2);
    cbo = cbo2;
    if (res != VK_SUCCESS) return res;

    constexpr VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = nullptr
    };
    return vkBeginCommandBuffer(cbo2, &begin_info);
}

VkResult vka::Buffer::end_copy(VkQueue queue, VkCommandBuffer cbo, VkFence fence) noexcept
{
    return end_and_submit(queue, cbo, fence);
}

VkResult vka::Buffer::end_wait_copy(VkQueue queue, VkCommandBuffer cbo, VkDevice device, VkFence fence, uint64_t timeout) noexcept
{
    const VkResult res = end_and_submit(queue, cbo, fence);
    if (res != VK_SUCCESS) return res;
    return (fence == VK_NULL_HANDLE) ? vkQueueWaitIdle(queue) : vkWaitForFences(device, 1, &fence, VK_TRUE, timeout);
}