/**
 * @brief Implementation for the buffer class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vka/vka.h>

vka::Buffer::Buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info) :
    m_buffer(create_buffer(device, properties, create_info)),
    m_size(create_info.bufferSize),
    m_map(nullptr)
{}

vka::unique_handle<vka::Buffer::Handle> vka::Buffer::create_buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info)
{
    // create buffer
    const VkBufferCreateInfo buffer_ci = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = create_info.pBufferNext,
        .flags = create_info.bufferFlags,
        .size = create_info.bufferSize,
        .usage = create_info.bufferUsage,
        .sharingMode = create_info.bufferSharingMode,
        .queueFamilyIndexCount = create_info.bufferQueueFamilyIndexCount,
        .pQueueFamilyIndices = create_info.bufferQueueFamilyIndices
    };
    VkBuffer buffer;
    check_result(vkCreateBuffer(device, &buffer_ci, nullptr, &buffer), BUFFER_CREATE_FAILED);
    unique_handle buffer_guard(device, buffer);


    // query memory requirements
    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(device, buffer, &requirements);

    // allocate memory
    const VkMemoryAllocateInfo memory_ai = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = create_info.pMemoryNext,
        .allocationSize = requirements.size,
        .memoryTypeIndex = memory::find_type_index(properties, requirements.memoryTypeBits, create_info.memoryType)
    };
    VkDeviceMemory memory;
    check_result(vkAllocateMemory(device, &memory_ai, nullptr, &memory), ALLOC_MEMORY_FAILED);
    unique_handle memory_guard(device, memory);
    check_result(vkBindBufferMemory(device, buffer, memory, 0), BIND_MEMORY_FAILED);

    const Handle handle = { buffer_guard.release(), memory_guard.release() };
    return unique_handle(device, handle);
}
