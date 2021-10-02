/**
* @file     vka_buffer_impl.h
* @brief    Buffer implementation file.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "vka_buffer.h"
#include <stdexcept>

vka::Buffer::Buffer(void) noexcept
{
    this->_create_info = {};
    this->_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    this->_create_info.pNext = nullptr;
    this->_properties = 0;
    
    this->_buffer = VK_NULL_HANDLE;
    this->_memory = VK_NULL_HANDLE;
    this->_device = VK_NULL_HANDLE;
    this->_physical_device = VK_NULL_HANDLE;
    this->_cmd_pool = VK_NULL_HANDLE;
    this->_cmd_queue = VK_NULL_HANDLE;
    
    this->_mem_size = 0;
}

vka::Buffer::Buffer(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool, VkQueue queue) noexcept : Buffer()
{
    this->_physical_device = physical_device;
    this->_device = device;
    this->_cmd_pool = cmd_pool;
    this->_cmd_queue = queue;
}

vka::Buffer::Buffer(const Buffer& src) : Buffer()
{
    this->copy(src);
}

vka::Buffer& vka::Buffer::operator= (const Buffer& src)
{
    return this->copy(src);
}

vka::Buffer::Buffer(Buffer&& src) : Buffer()
{
    this->move((Buffer&&)src);
}

vka::Buffer& vka::Buffer::operator= (Buffer&& src)
{
    return this->move((Buffer&&)src);
}

vka::Buffer::~Buffer(void)
{
    this->clear();
}

void vka::Buffer::set_create_flags(VkBufferCreateFlags flags) noexcept
{
    this->_create_info.flags = flags;
}

void vka::Buffer::set_create_size(VkDeviceSize size) noexcept
{
    this->_create_info.size = size;
}

void vka::Buffer::set_create_usage(VkBufferUsageFlags usage) noexcept
{
    this->_create_info.usage = usage;
}

void vka::Buffer::set_create_sharing_mode(VkSharingMode sharing_mode) noexcept
{
    this->_create_info.sharingMode = sharing_mode;
}

void vka::Buffer::set_create_queue_families(const uint32_t* indices, uint32_t count) noexcept
{
    this->_create_info.queueFamilyIndexCount = count;
    this->_create_info.pQueueFamilyIndices = indices;
}

void vka::Buffer::set_create_info(const VkBufferCreateInfo& create_info) noexcept
{
    // this code can be reduced to this->_create_info = create_info
    // but this may destroy the default initalization and can cause troubles
    this->_create_info.flags = create_info.flags;
    this->_create_info.size = create_info.size;
    this->_create_info.usage = create_info.usage;
    this->_create_info.sharingMode = create_info.sharingMode;
    this->_create_info.queueFamilyIndexCount = create_info.queueFamilyIndexCount;
    this->_create_info.pQueueFamilyIndices = create_info.pQueueFamilyIndices;
}

void vka::Buffer::set_memory_properties(VkMemoryPropertyFlags properties) noexcept
{
    this->_properties = properties;
}

void vka::Buffer::set_queue(VkQueue queue) noexcept
{
    this->_cmd_queue = queue;
}

void vka::Buffer::set_command_pool(VkCommandPool cmd_pool) noexcept
{
    this->_cmd_pool = cmd_pool;
}

void vka::Buffer::set_physical_device(VkPhysicalDevice physical_device) noexcept
{
    this->_physical_device = physical_device;
}

void vka::Buffer::set_device(VkDevice device) noexcept
{
    this->_device = device;
}

VkResult vka::Buffer::create(void)
{
    if (this->_memory != VK_NULL_HANDLE || this->_buffer != VK_NULL_HANDLE)
        throw std::runtime_error("Can not create buffer withot deleting the old one!");
    if (this->_physical_device == VK_NULL_HANDLE)
        throw std::invalid_argument("Physical device of buffer is a VK_NULL_HANDLE! Requiered from buffer create.");
    if (this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("Device of buffer is a VK_NULL_HANDLE! Requiered from buffer create.");
    
    // create buffer from internal create info
    VkResult result = vkCreateBuffer(this->_device, &this->_create_info, nullptr, &this->_buffer);
    if (result != VK_SUCCESS) return result;
    
    // memory requirements specify the memory alignment, the actual buffer size and the memory type bits
    VkMemoryRequirements mem_req = {};
    vkGetBufferMemoryRequirements(this->_device, this->_buffer, &mem_req);
    
    // information for the memory allocation
    VkMemoryAllocateInfo mem_alloc_info = {};
    mem_alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc_info.pNext = nullptr;
    mem_alloc_info.allocationSize = mem_req.size;
    mem_alloc_info.memoryTypeIndex = utility::find_memory_type_index(this->_physical_device, mem_req.memoryTypeBits, this->_properties);
    
    // allocate memory for buffer
    result = vkAllocateMemory(this->_device, &mem_alloc_info, nullptr, &this->_memory);
    if (result != VK_SUCCESS) return result;
    
    // bind memory to buffer
    result = vkBindBufferMemory(this->_device, this->_buffer, this->_memory, 0);
    if (result != VK_SUCCESS) return result;
    
    this->_mem_size = mem_req.size;
    return VK_SUCCESS;
}

void* vka::Buffer::map(VkDeviceSize size, VkDeviceSize offset)
{
    if (this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("Device of buffer is a VK_NULL_HANDLE, requiered from memory mapping!");
    if (this->_memory == VK_NULL_HANDLE)
        throw std::runtime_error("Memory to map is a VK_NULL_HANDLE, create memory before any using map operations!");
    
    void* _map;
    VkResult result = vkMapMemory(this->_device, this->_memory, offset, size, 0, &_map);
    if (result != VK_SUCCESS)
        throw std::runtime_error("Vulkan memory mapping failed!");
    
    return _map;
}

void vka::Buffer::unmap(void)
{
    if (this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("Device of buffer is a VK_NULL_HANDLE, requiered from memory unmapping!");
    if (this->_memory == VK_NULL_HANDLE)
        throw std::runtime_error("Memory to map is a VK_NULL_HANDLE, create memory before any using map operations!");
    
    vkUnmapMemory(this->_device, this->_memory);
}

size_t vka::Buffer::size(void) const noexcept
{
    return this->_create_info.size;
}

size_t vka::Buffer::mem_size(void) const noexcept
{
    return this->_mem_size;
}

VkBuffer vka::Buffer::handle(void) const noexcept
{
    return this->_buffer;
}

VkDeviceMemory vka::Buffer::memory_handle(void) const noexcept
{
    return this->_memory;
}

vka::Buffer& vka::Buffer::copy(const Buffer& src)
{
    // physical device and logical device is used from source buffer
    this->_physical_device = src._physical_device;
    this->_device = src._device;
    
    // if the own ones are VK_NULL_HANDLE take them from the source (does not mean that they are not a VK_NULL_HANLDE either)
    if (this->_cmd_queue == VK_NULL_HANDLE)
        this->_cmd_queue = src._cmd_queue;
    if (this->_cmd_pool == VK_NULL_HANDLE)
        this->_cmd_pool = src._cmd_pool;
    
    // Copy the create info from source buffer without the usage flags because the destination buffer
    // can have other usage flags e.g. (staging buffer -> vertex buffer)
    // However, is the own (destination) usage flags are 0, then copy them from the source buffer.
    VkBufferUsageFlags old_usage;
    old_usage = this->_create_info.usage;
    this->_create_info = src._create_info;
    this->_create_info.usage = (old_usage == 0) ? src._create_info.usage : old_usage;
    
    // the same counts for the memory properties
    this->_properties = (this->_properties == 0) ? src._properties : this->_properties;
    
    if (this->_cmd_queue == VK_NULL_HANDLE)
        throw std::invalid_argument("Command queue of buffer is a VK_NULL_HANDLE! Requiered from buffer copy.");
    if (this->_cmd_pool == VK_NULL_HANDLE)
        throw std::invalid_argument("Command pool of buffer is a VK_NULL_HANDLE! Requiered from buffer copy.");
    if (this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("Device of buffer is a VK_NULL_HANDLE! Requiered from buffer copy.");
    
    // source buffer must have VK_BUFFER_USAGE_TRANSFER_SRC_BIT bit enabled otherwise the copy will fail
    if (!(src._create_info.usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT))
        throw std::runtime_error("Can not copy source buffer without VK_BUFFER_USAGE_TRANSFER_SRC_BIT set!");
    
    // and also the destination buffer must have VK_BUFFER_USAGE_TRANSFER_DST_BIT bit enabled
    this->_create_info.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    
    this->clear();                      // current buffer must be cleared
    VkResult result = this->create();   // and recreated with the new create info and memory property flags
    if (result != VK_SUCCESS)
        throw std::runtime_error("Recreation of buffer within a copy operation failed!");
    
    // a command buffer must be recorded because the copy operation requieres it
    VkCommandBufferAllocateInfo cpy_cmd_buffer_alloc_info = {};
    cpy_cmd_buffer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cpy_cmd_buffer_alloc_info.pNext = nullptr;
    cpy_cmd_buffer_alloc_info.commandPool = this->_cmd_pool;
    cpy_cmd_buffer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cpy_cmd_buffer_alloc_info.commandBufferCount = 1;
    
    VkCommandBuffer cpy_cmd_buffer = {};
    result = vkAllocateCommandBuffers(this->_device, &cpy_cmd_buffer_alloc_info, &cpy_cmd_buffer);
    if (result != VK_SUCCESS)
        throw std::runtime_error("Vulkan command buffer allocation failed while copying!");
    
    VkCommandBufferBeginInfo cmd_begin_info = {};
    cmd_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_begin_info.pNext = nullptr;
    cmd_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmd_begin_info.pInheritanceInfo = nullptr;
    
    // record command buffer with copy operation
    result = vkBeginCommandBuffer(cpy_cmd_buffer, &cmd_begin_info);
    if (result != VK_SUCCESS)
        throw std::runtime_error("Vulkan failed to begin copy process!");
    
    VkBufferCopy buffer_cpy = {};
    buffer_cpy.srcOffset = 0;
    buffer_cpy.dstOffset = 0;
    buffer_cpy.size = this->size();
    
    // copy...
    vkCmdCopyBuffer(cpy_cmd_buffer, src._buffer, this->_buffer, 1, &buffer_cpy);
    
    result = vkEndCommandBuffer(cpy_cmd_buffer);
    if (result != VK_SUCCESS)
        throw std::runtime_error("Vulkan failed to end copy process!");
    
    // submit command buffer to the command queue
    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = nullptr;
    submit_info.waitSemaphoreCount = 0;
    submit_info.pWaitSemaphores = nullptr;
    submit_info.pWaitDstStageMask = nullptr;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &cpy_cmd_buffer;
    submit_info.signalSemaphoreCount = 0;
    submit_info.pSignalSemaphores = nullptr;
    
    result = vkQueueSubmit(this->_cmd_queue, 1, &submit_info, VK_NULL_HANDLE);
    if (result != VK_NULL_HANDLE)
        throw std::runtime_error("Vulkan queue submition failed!");
    
    result = vkQueueWaitIdle(this->_cmd_queue);
    if (result != VK_NULL_HANDLE)
        throw std::runtime_error("Vulkan queue wait-idle failed!");
    
    // free command buffer, it is not used anymore
    vkFreeCommandBuffers(this->_device, this->_cmd_pool, 1, &cpy_cmd_buffer);
    
    return *this;
}

vka::Buffer& vka::Buffer::move(Buffer&& src)
{
    this->copy(src);    // copy soruce into destination
    src.clear();        // and clear source
    return *this;
}

void vka::Buffer::clear(void)
{
    if (this->_memory != VK_NULL_HANDLE && this->_buffer != VK_NULL_HANDLE)
    {
        if (this->_device == VK_NULL_HANDLE)
            throw std::invalid_argument("Device of buffer is a VK_NULL_HANDLE! Requiered from buffer clear.");
        
        vkFreeMemory(this->_device, this->_memory, nullptr);
        vkDestroyBuffer(this->_device, this->_buffer, nullptr);
        this->_memory = VK_NULL_HANDLE;
        this->_buffer = VK_NULL_HANDLE;
    }
}