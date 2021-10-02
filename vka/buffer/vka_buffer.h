/**
* @file     vka_buffer.h
* @brief    Buffer definition file.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../vka_global.h"

namespace vka
{
    class Buffer
    {
    private:
        VkDevice _device;
        VkPhysicalDevice _physical_device;
        VkCommandPool _cmd_pool;
        VkQueue	_cmd_queue;
        
        VkBufferCreateInfo _create_info;
        VkBuffer _buffer;
        VkMemoryPropertyFlags _properties;
        VkDeviceMemory _memory;
        
        size_t _mem_size;
    
    public:
        Buffer(void) noexcept;
        
        /**
        * @param[in] physical_device: physical device
        * @param[in] device: logical device
        * @param[in] cmd_pool: command pool used for copy operations
        * @param[in] queue: command queue of execution
        */
        explicit Buffer(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool = VK_NULL_HANDLE, VkQueue queue = VK_NULL_HANDLE) noexcept;
        
        /** @brief Copy constructor and copy operator. */
        Buffer(const Buffer& src);
        Buffer& operator= (const Buffer& src);
        
        /** @brief Move constructor and move operator. */
        Buffer(Buffer&& src);
        Buffer& operator= (Buffer&& src);
        
        virtual ~Buffer(void);
        
        /** @brief Setter for the buffer create info. */
        void set_create_flags(VkBufferCreateFlags flags) noexcept;
        void set_create_size(VkDeviceSize size) noexcept;
        void set_create_usage(VkBufferUsageFlags usage) noexcept;
        void set_create_sharing_mode(VkSharingMode sharing_mode) noexcept;
        void set_create_queue_families(const uint32_t* indices, uint32_t count) noexcept;
        void set_create_info(const VkBufferCreateInfo& create_info) noexcept;
        
        /** @brief Sets the memory properties of the buffer. */
        void set_memory_properties(VkMemoryPropertyFlags properties) noexcept;
        
        /** @param[in] physical_device: physical device */
        void set_physical_device(VkPhysicalDevice physical_device) noexcept;
        
        /** @param[in] device: logical device */
        void set_device(VkDevice device) noexcept;
        
        /** @param[in] cmd_pool: command pool used for copy operations */
        void set_queue(VkQueue queue) noexcept;
        
        /** @param[in] queue: command queue of execution */
        void set_command_pool(VkCommandPool cmd_pool) noexcept;
        
        /**
        * @brief Creates the buffer.
        * @return vulkan result
        */
        VkResult create(void);
        
        /**
        * @brief Maps the buffer for direct buffer access.
        * @param[in] size: size of the buffer to map
        * @param[in] offset: memory offset
        * @return pointer to the buffer memory
        */
        void* map(VkDeviceSize size, VkDeviceSize offset);
        
        /** @brief unampps the buffer */
        void unmap(void);
        
        /** @return buffer size in bytes */
        size_t size(void) const noexcept;
        
        /** @return allocated memory size of the buffer in bytes */
        size_t mem_size(void) const noexcept;
        
        /** @return VkBuffer handle */
        VkBuffer handle(void) const noexcept;

        /** @return VkDeviceMemory handle */
        VkDeviceMemory memory_handle(void) const noexcept;
        
        /**
        * @brief Copies a buffer. Buffer copy operation requieres command pool and command queue to be set.
        *   The buffer create info or the memory properties of the destination buffer can change how the buffer is copied.
        *   If the create info and the memory properties of the destination buffer contain their default values then the copy operation is a 1:1 copy.
        *   If the create info or the memory properties of the destination buffer have been modified then the buffer can also copy
        *   memory into another memory target (e.g. host memory -> device memory or cached memory -> host memory).
        *   This description counts for the copy operator. The copy constructor is always a 1:1 copy.
        * @param[in] src: source buffer
        * @return destination buffer of the copy operatrion ('this')
        */
        Buffer& copy(const Buffer& src);
        
        /**
        * @brief Move is a copy operation followed by a clear of the source buffer.
        *   After a move the source buffer is not aviable anymore.
        *   The same description counts for the move operator and the move constructor.
        * @param[in] src: source buffer
        * @return destination buffer of the move operatrion ('this')
        */
        Buffer& move(Buffer&& src);
        
        /** @brief Cleares the buffer. The buffer is no longer aviable after a clear. */
        void clear(void);
    };
}