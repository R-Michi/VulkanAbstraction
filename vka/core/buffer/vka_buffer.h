/**
* @file     vka_buffer.h
* @brief    Buffer definition file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka
{
    class Buffer
    {
    private:
        VkDevice _device;
        VkPhysicalDevice _physical_device;
        
        VkBuffer _buffer;
        VkDeviceMemory _memory;
        VkBufferCreateInfo _ci, _backup_ci;
        VkMemoryPropertyFlags _properties;
        
        size_t _mem_size;
    
    public:
        Buffer(void) noexcept;
        
        /**
        * @param[in] physical_device    physical device
        * @param[in] device             logical device
        */
        explicit Buffer(VkPhysicalDevice physical_device, VkDevice device) noexcept;
        
        /** @brief Copy constructor and copy operator. */
        Buffer(const Buffer& src) = delete;
        Buffer& operator= (const Buffer& src) = delete;
        
        /** 
         * @brief   Move constructor and move operator. 
         * @note    This does not replace a call to enqueue_copy()!
         *          Both the move constructor and move operator
         *          copy all the properties of the buffer and transfer
         *          ownership of the buffer object's handles.
         *          Therefore, the source buffer will become cleared.
         */
        Buffer(Buffer&& src);
        Buffer& operator= (Buffer&& src);
        
        virtual ~Buffer(void);
        
        /** @brief Setter for the parameter of the VkBufferCreateInfo.*/
        void set_create_flags(VkBufferCreateFlags flags) noexcept;
        void set_create_size(VkDeviceSize size) noexcept;
        void set_create_usage(VkBufferUsageFlags usage) noexcept;
        void set_create_sharing_mode(VkSharingMode sharing_mode) noexcept;
        void set_create_queue_families(const uint32_t* indices, uint32_t count) noexcept;
        void set_create_info(const VkBufferCreateInfo& create_info) noexcept;
        
        /** @brief Sets the memory properties of the buffer. */
        void set_memory_properties(VkMemoryPropertyFlags properties) noexcept;
        
        /** @param[in] physical_device physical device */
        void set_physical_device(VkPhysicalDevice physical_device) noexcept;
        
        /** @param[in] device logical device */
        void set_device(VkDevice device) noexcept;
        
        /**
        * @brief Creates the buffer.
        * @return vulkan result
        */
        VkResult create(void);

        /** @brief Cleares the buffer. The buffer is no longer aviable after a clear. */
        void clear(void);
        
        /**
        * @brief            Maps the buffer for direct buffer access.
        * @param[in] size   size of the buffer to map
        * @param[in] offset memory offset
        * @return           pointer to the buffer memory
        */
        void* map(VkDeviceSize size, VkDeviceSize offset);
        
        /** @brief unampps the buffer */
        void unmap(void);

        /** @return create info at the time point when the buffer was created */
        const VkBufferCreateInfo& get_create_info(void) const noexcept;

        /** @return size of the created buffer in bytes */
        size_t size(void) const noexcept;

        /** @return actually allocated memory size requiered for buffer in bytes*/
        size_t mem_size(void) const noexcept;
        
        /** @return VkBuffer handle */
        VkBuffer handle(void) const noexcept;

        /** @return Boolean wether the buffer has been created */
        bool is_created(void) const noexcept;

        /**
         * @brief               Copies multiple vka::Buffer objects from src (source) into dst (destination).
         * @param[in] device    logical device
         * @param[in] pool      command pool
         * @param[in] count     number of buffers to copy
         * @param[in] src       source buffers
         * @param[in] dst       destination buffers
         * @return              secondary command buffer object of recorded copy commands
         * @note                The function copies the host-side contents from the src buffer objects into the dst buffer objects.
         *                      The device-side contents - the actual memory of the buffer - will not be copied after 
         *                      executing this function, instead only a secondary command buffer is recorded containing
         *                      the copy commands. Those secondary command buffers MUST be executed manually.
         *                      Therefore be careful as the target buffer does not contain the source buffer's memory contents
         *                      after calling enqueue_copy.
         */
        static VkCommandBuffer enqueue_copy(VkDevice device, VkCommandPool pool, uint32_t count, const Buffer* src, Buffer* dst);
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_buffer_impl.inl"
#endif
