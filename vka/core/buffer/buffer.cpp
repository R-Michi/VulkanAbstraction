/**
 * @brief Implementation for the buffer class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vulkan/vulkan.h>
#include <vka/vka.h>

vka::Buffer::Buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info) :
    m_buffer(device , create_buffer(device, properties, create_info)),
    m_size(create_info.bufferSize),
    m_map_status(false)
{
    check_result(vkBindBufferMemory(device, this->m_buffer.get().buffer, this->m_buffer.get().memory, 0), BIND_MEMORY_FAILED);
}

vka::Buffer::BufferMemory vka::Buffer::create_buffer(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const BufferCreateInfo& create_info)
{
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
    VkResult result = vkCreateBuffer(device, &buffer_ci, nullptr, &buffer);
    if (is_error(result)) [[unlikely]]
        detail::error::throw_runtime_error(BUFFER_CREATE_FAILED);

    // query memory requirements
    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(device, buffer, &requirements);

    const VkMemoryAllocateInfo memory_ai = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = create_info.pMemoryNext,
        .allocationSize = requirements.size,
        .memoryTypeIndex = memory::find_type_index(properties, requirements.memoryTypeBits, create_info.memoryPropertyFlags)
    };
    VkDeviceMemory memory;
    result = vkAllocateMemory(device, &memory_ai, nullptr, &memory);
    if (is_error(result)) [[unlikely]]
    {
        // One handle has been created successfully which must be destroyed.
        vkDestroyBuffer(device, buffer, nullptr);
        detail::error::throw_runtime_error(ALLOC_MEMORY_FAILED);
    }
    return { buffer, memory };
}
