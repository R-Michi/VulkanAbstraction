/**
* @file     vka_descriptor.h
* @brief    Descriptor manager definition file.
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
    class DescriptorManager
    {
    private:
        // handles
        VkDescriptorPool _pool;
        std::vector<VkDescriptorSetLayout> set_layouts;
        std::vector<VkDescriptorSet> sets;

        // descriptor structs
        std::vector<VkDescriptorPoolSize> pool_sizes;
        std::vector< std::vector<VkDescriptorSetLayoutBinding> > set_layout_bindings;
        std::vector<VkWriteDescriptorSet> descriptor_writes;

        // requiered
        VkDevice device;

        VkResult init_descriptor_layouts(void);
        VkResult init_descriptor_pool(void);
        VkResult init_descriptor_sets(void);

        void add_to_pool_sizes(VkDescriptorType type, uint32_t count);
        void init_descriptor_write(VkWriteDescriptorSet& write, uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t n_elements);

    public:
        /**
        * @brief device is a VK_NULL_HANDLE per default.
        */
        DescriptorManager(void);

        /**
        * @param[in] device: logical device
        * @param[in] count: number of descriptor sets to allocate
        */
        DescriptorManager(VkDevice device, uint32_t count);
        virtual ~DescriptorManager(void);

        /**
        * @brief Set the logical device that is used by the descriptor manager.
        * @param[in] device: logical device
        */
        void set_device(VkDevice device);

        /**
        * @brief Set the number of descriptor sets to allocate.
        * @param[in] count: number of descriptor sets
        * @throw std::runtime_error, if this method is called after 'DescriptorManager::init()'
        */
        void set_descriptor_set_count(uint32_t count);

        /** @return number of allocated descriptor sets */
        uint32_t descriptor_set_count(void);

        /**
        * @brief Adds a binding to a descriptor set.
        * 
        * @param[in] set: descriptor set ID
        * @param[in] binding: descriptor set binding ID
        * @param[in] type: type of the descriptors within the binding
        * @param[in] count: number of descriptors of the binding
        * @param[in] stage: shader stage where the descriptor binding is present
        * 
        * @throw std::runtime_error, if this method is called after 'DescriptorManager::init()'
        * @throw std::out_of_range, if descriptor set ID greather than or equal to the
        *   allocated descriptor set count
        */
        void add_binding(uint32_t set, uint32_t binding, VkDescriptorType type, uint32_t count, VkShaderStageFlags stage);

        /**
        * @brief Sets buffer descriptors.
        * 
        * @param[in] set: descriptor set ID
        * @param[in] binding: descriptor set binding ID
        * @param[in] begin_element: index of the first affected descriptor
        * @param[in] element_count: number of descriptors that are affected
        *   NOTE: '@param begin_element' + '@param element_count' MUST NOT exeed the descriptor capacity
        *   of this binding, this will cause validation errors.
        * @param[in] buffer_infos: buffer info structs for every descriptor that is affected
        * 
        * @throw std::out_of_range, if descriptor set ID greather than or equal to the
        *   allocated descriptor set count
        * @throw std::runtime_error, if descriptor binding ID of the given could not be found
        */
        void write_buffer_info(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t element_count, const VkDescriptorBufferInfo* buffer_infos);

        /**
        * @brief Sets image descriptors.
        *
        * @param[in] set: descriptor set ID
        * @param[in] binding: descriptor set binding ID
        * @param[in] begin_element: index of the first affected descriptor
        * @param[in] element_count: number of descriptors that are affected
        *   NOTE: '@param begin_element' + '@param element_count' MUST NOT exeed the descriptor capacity
        *   of this binding, this will cause validation errors.
        * @param[in] buffer_infos: buffer info structs for every descriptor that is affected
        *
        * @throw std::out_of_range, if descriptor set ID greather than or equal to the
        *   allocated descriptor set count
        * @throw std::runtime_error, if descriptor binding ID of the given could not be found
        */
        void write_image_info(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t element_count, const VkDescriptorImageInfo* image_infos);

        /**
        * @brief Sets texel buffer view descriptors.
        *
        * @param[in] set: descriptor set ID
        * @param[in] binding: descriptor set binding ID
        * @param[in] begin_element: index of the first affected descriptor
        * @param[in] element_count: number of descriptors that are affected
        *   NOTE: '@param begin_element' + '@param element_count' MUST NOT exeed the descriptor capacity
        *   of this binding, this will cause validation errors.
        * @param[in] buffer_infos: buffer info structs for every descriptor that is affected
        *
        * @throw std::out_of_range, if descriptor set ID greather than or equal to the
        *   allocated descriptor set count
        * @throw std::runtime_error, if descriptor binding ID of the given could not be found
        */
        void write_texel_buffer_view(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t element_count, const VkBufferView* buffer_views);

        /**
        * @brief Initializes the descriptor manager.
        *   After initialization, bindings can no longer be added and the amount of sets
        *   can no longer be canged.
        * 
        * @thorw std::runtime_error, if this method is called twice or more
        * @thorw std::invalid_argument, if device is a VK_NULL_HANDLE
        */
        VkResult init(void);

        /**
        * @brief Updates descriptor sets.
        * 
        * @throw std::runtime_error, if descriptor manager has not been initialized
        * @throw std::invalid_argument, if device is a VK_NULL_HANDLE
        */
        void update(void);

        /** 
        * @brief Cleares the descriptor manager. 
        * @thorw std::invalid_argument, if device is a VK_NULL_HANDLE
        */
        void clear(void);

        /** @return descriptor pool handle */
        VkDescriptorPool pool(void) const noexcept;

        /** @return vector of all descriptor set layout handles */
        const std::vector<VkDescriptorSetLayout>& layouts(void) const noexcept;

        /** @return vector of all descriptor set handles */
        const std::vector<VkDescriptorSet>& descriptor_sets(void) const noexcept;
    };
}
