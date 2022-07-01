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

vka::Buffer::Buffer(void) noexcept : Buffer(VK_NULL_HANDLE, VK_NULL_HANDLE) {}

vka::Buffer::Buffer(VkPhysicalDevice physical_device, VkDevice device) noexcept
{
    this->_physical_device = physical_device;
    this->_device = device;

    this->_buffer = VK_NULL_HANDLE;
    this->_memory = VK_NULL_HANDLE;
    
    this->_ci = this->_backup_ci = {};
    this->_ci.sType = this->_backup_ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    this->_ci.pNext = this->_backup_ci.pNext = nullptr;

    this->_properties = VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM;
    this->_mem_size = 0;
}

vka::Buffer::Buffer(Buffer&& src)
{
    *this = std::move(src);
}

vka::Buffer& vka::Buffer::operator= (Buffer&& src)
{
    this->clear();

    this->_device = src._device;
    this->_physical_device = src._physical_device;
    this->_ci = src._ci;
    this->_backup_ci = src._backup_ci;
    this->_properties = src._properties;
    this->_mem_size = src._mem_size;
    this->_buffer = src._buffer;
    this->_memory = src._memory;
    src._buffer = VK_NULL_HANDLE;
    src._memory = VK_NULL_HANDLE;
    src.clear();
    return *this;
}

vka::Buffer::~Buffer(void)
{
    this->clear();
}

void vka::Buffer::set_create_flags(VkBufferCreateFlags flags) noexcept
{
    this->_ci.flags = flags;
}

void vka::Buffer::set_create_size(VkDeviceSize size) noexcept
{
    this->_ci.size = size;
}

void vka::Buffer::set_create_usage(VkBufferUsageFlags usage) noexcept
{
    this->_ci.usage = usage;
}

void vka::Buffer::set_create_sharing_mode(VkSharingMode sharing_mode) noexcept
{
    this->_ci.sharingMode = sharing_mode;
}

void vka::Buffer::set_create_queue_families(const uint32_t* indices, uint32_t count) noexcept
{
    this->_ci.queueFamilyIndexCount    = count;
    this->_ci.pQueueFamilyIndices      = indices;
}

void vka::Buffer::set_create_info(const VkBufferCreateInfo& create_info) noexcept
{
    // this code can be reduced to this->_create_info = create_info
    // but this may destroy the default initalization and can cause troubles
    this->_ci.flags                    = create_info.flags;
    this->_ci.size                     = create_info.size;
    this->_ci.usage                    = create_info.usage;
    this->_ci.sharingMode              = create_info.sharingMode;
    this->_ci.queueFamilyIndexCount    = create_info.queueFamilyIndexCount;
    this->_ci.pQueueFamilyIndices      = create_info.pQueueFamilyIndices;
}

void vka::Buffer::set_memory_properties(VkMemoryPropertyFlags properties) noexcept
{
    this->_properties = properties;
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
    if(this->is_created())
        throw std::runtime_error("[vka::Buffer::create]: Buffer has already been created, use .clear() to recreate the buffer.");
    if(this->_physical_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Buffer::create]: Physical device of buffer is a VK_NULL_HANDLE!");
    if(this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Buffer::create]: Device of buffer is a VK_NULL_HANDLE!");

    this->_backup_ci = this->_ci;
    
    // create buffer from internal create info
    VkResult result = vkCreateBuffer(this->_device, &this->_backup_ci, nullptr, &this->_buffer);
    if(result != VK_SUCCESS) { this->clear(); return result; }
    
    // memory requirements specify the memory alignment, the actual buffer size and the memory type bits
    VkMemoryRequirements mem_req = {};
    vkGetBufferMemoryRequirements(this->_device, this->_buffer, &mem_req);
    this->_mem_size = mem_req.size;
    
    // allocate memory for buffer
    VkMemoryAllocateInfo mem_alloc_info = {};
    mem_alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mem_alloc_info.pNext = nullptr;
    mem_alloc_info.allocationSize = this->_mem_size;
    mem_alloc_info.memoryTypeIndex = utility::find_memory_type_index(this->_physical_device, mem_req.memoryTypeBits, this->_properties);
    
    result = vkAllocateMemory(this->_device, &mem_alloc_info, nullptr, &this->_memory);
    if(result != VK_SUCCESS) { this->clear(); return result; }
    
    // bind memory to buffer
    result = vkBindBufferMemory(this->_device, this->_buffer, this->_memory, 0);
    if(result != VK_SUCCESS) { this->clear(); return result; }

    return VK_SUCCESS;
}

void vka::Buffer::clear(void)
{
    if ((this->_memory != VK_NULL_HANDLE || this->_buffer != VK_NULL_HANDLE) && this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Buffer::clear]: Device of buffer is a VK_NULL_HANDLE!");

    if (this->_memory != VK_NULL_HANDLE) vkFreeMemory(this->_device, this->_memory, nullptr);
    if (this->_buffer != VK_NULL_HANDLE) vkDestroyBuffer(this->_device, this->_buffer, nullptr);

    this->_memory = VK_NULL_HANDLE;
    this->_buffer = VK_NULL_HANDLE;
    this->_mem_size = 0;

    this->_backup_ci = {};
    this->_backup_ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    this->_backup_ci.pNext = nullptr;
}

void* vka::Buffer::map(VkDeviceSize size, VkDeviceSize offset)
{
    if (this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Buffer::map]: Device of buffer is a VK_NULL_HANDLE!");
    if (this->_memory == VK_NULL_HANDLE)
        throw std::runtime_error("[vka::Buffer::map]: Memory to map is a VK_NULL_HANDLE, create buffer before any using map operations!");
    
    void* _map;
    VkResult result = vkMapMemory(this->_device, this->_memory, offset, size, 0, &_map);
    if (result != VK_SUCCESS)
        throw std::runtime_error("[vka::Buffer::map]: Vulkan memory mapping failed!");
    
    return _map;
}

void vka::Buffer::unmap(void)
{
    if (this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Buffer::unmap]: Device of buffer is a VK_NULL_HANDLE!");
    if (this->_memory == VK_NULL_HANDLE)
        throw std::runtime_error("[vka::Buffer::unmap]: Memory to unmap is a VK_NULL_HANDLE, create buffer before any using map operations!");
    
    vkUnmapMemory(this->_device, this->_memory);
}

const VkBufferCreateInfo& vka::Buffer::get_create_info(void) const noexcept
{
    return this->_backup_ci;
}

size_t vka::Buffer::size(void) const noexcept
{
    return this->_backup_ci.size;
}

size_t vka::Buffer::mem_size(void) const noexcept
{
    return this->_mem_size;
}

VkBuffer vka::Buffer::handle(void) const noexcept
{
    return this->_buffer;
}

bool vka::Buffer::is_created(void) const noexcept
{
    return (this->_mem_size > 0);
}

VkCommandBuffer vka::Buffer::enqueue_copy(VkDevice device, VkCommandPool pool, uint32_t count, const Buffer* src, Buffer* dst)
{
    // no copies should be made
    if (count == 0) return VK_NULL_HANDLE;

    if (device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Buffer::enqueue_copy]: Device is a VK_NULL_HANDLE!");
    if (pool == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Buffer::enqueue_copy]: Command pool VK_NULL_HANDLE!");

    // copy all the device-side buffer contents
    for (uint32_t i = 0; i < count; i++)
    {
        if (!src[i].is_created())
            throw std::runtime_error("[vka::Buffer::enqueue_copy]: All source buffers must have been created to be copyable.");
        if (!(src[i]._ci.usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT))
            throw std::invalid_argument("[vka::Buffer::enqueue_copy]: All source buffers must have VK_BUFFER_USAGE_TRANSFER_SRC_BIT set!");     
        if (!(dst[i]._ci.usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT))
            throw std::invalid_argument("[vka::Buffer::enqueue_copy]: All destination buffers must have VK_BUFFER_USAGE_TRANSFER_DST_BIT set!");
        if(dst[i]._properties == VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM)
            throw std::invalid_argument("[vka::Buffer::enqueue_copy]: Destination buffer has no memory properties set.");

        // general properties
        dst[i]._physical_device             = src[i]._physical_device;
        dst[i]._device                      = src[i]._device;

        // copy create info
        dst[i]._ci.flags                    = src[i]._backup_ci.flags;
        dst[i]._ci.size                     = src[i]._backup_ci.size;
        // destination buffer and source buffer can have different usage
        dst[i]._ci.sharingMode              = src[i]._backup_ci.sharingMode;
        dst[i]._ci.queueFamilyIndexCount    = src[i]._backup_ci.queueFamilyIndexCount;
        dst[i]._ci.pQueueFamilyIndices      = src[i]._backup_ci.pQueueFamilyIndices;
        // destination buffer and source buffer can have different memory properties

        // destination buffer must be cleared and recreated with a new create info
        dst[i].clear();
        if (dst[i].create() != VK_SUCCESS)
            throw std::runtime_error("[vka::Buffer::enqueue_copy]: Recreation of buffer within a copy operation failed!");
    }

    VkCommandBufferAllocateInfo ai = {};
    ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.pNext = nullptr;
    ai.commandPool = pool;
    ai.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    ai.commandBufferCount = 1;

    VkCommandBuffer cbo;
    if (vkAllocateCommandBuffers(device, &ai, &cbo) != VK_SUCCESS)
        throw std::runtime_error("[vka::Buffer::enqueue_copy]: Failed to create command buffer.");
    
    VkCommandBufferInheritanceInfo ii = {};
    ii.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    ii.pNext = nullptr;
    ii.renderPass = VK_NULL_HANDLE;
    ii.subpass = 0;
    ii.framebuffer = VK_NULL_HANDLE;
    ii.occlusionQueryEnable = VK_FALSE;
    ii.queryFlags = 0;
    ii.pipelineStatistics = 0;

    VkCommandBufferBeginInfo bi = {};
    bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bi.pNext = nullptr;
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    bi.pInheritanceInfo = &ii;  // is requiered because the command buffer recorded is secondary

    VkBufferCopy ci = {};
    ci.srcOffset = 0;
    ci.dstOffset = 0;

    // record copy commands
    if (vkBeginCommandBuffer(cbo, &bi) != VK_SUCCESS)
        throw std::runtime_error("[vka::Buffer::enqueue_copy]: Failed to begin recording the command buffer.");

    for(uint32_t i = 0; i < count; i++)
    {
        ci.size = src[i].size();
        vkCmdCopyBuffer(cbo, src[i].handle(), dst[i].handle(), 1, &ci);
    }

    if (vkEndCommandBuffer(cbo) != VK_SUCCESS)
        throw std::runtime_error("[vka::Buffer::enqueue_copy]: Failed to end recording the command buffer.");

    return cbo;
}