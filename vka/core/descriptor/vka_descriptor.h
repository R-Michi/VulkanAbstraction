/**
* @file     vka_descriptor.h
* @brief    Descriptor definition file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../detail/descriptor/vka_descriptor.h"

namespace vka
{
    /*
     * This is a helper class to create bindings for descriptor sets. Each set of descriptors can have
     * multiple bindings of descriptors. The purpose of this class is to minimize the effort to create
     * bindings for multiple descriptor sets.
     */
    template<uint32_t S>
    class DescriptorSetBindingList
    {
    private:
        std::vector<VkDescriptorSetLayoutBinding> m_bindings[S];

    public:
        DescriptorSetBindingList(void) = default;
        ~DescriptorSetBindingList(void) = default;

        /*
         * Adds a binding to the binding-list for a specified descriptor set 'set'. The binding number
         * for every individual set is incremented by 1, and starts at 0, if this function is called.
         * Different descriptor sets have independent bindings and have its own index counter.
         * Therefore, the number of bindings for a given set corresponds to the number of push() calls
         * for this specific set. The number of descriptor sets the binding-list contains corresponds
         * to the highest set-index + 1. 'set' specifies the descriptor set-index which also starts at
         * 0. 'stages' specify the shader stages where the current binding is used. 'type' specifies the
         * descriptor type. 'count' specifies the number of descriptors of the given type. It is set to
         * 1 because it is often required to only have one descriptor per binding. 'immutable_samplers'
         * specifies an array of vulkan sampler handles which is optional and 'nullptr' by default.
         * NOTE: Fore more information see the documentation of VkDescriptorSetLayoutBinding.
         * Additionally, the size of the 'immutable_samplers'-array is not explicitly mentioned in the
         * documentation of VkDescriptorSetLayoutBinding. It might require as many VkSampler handles as
         * the number of descriptors specified for this binding (the value specified by 'count').
         */
        void push(uint32_t set, VkDescriptorType type, VkShaderStageFlags stages, uint32_t count = 1, const VkSampler* immutable_samplers = nullptr);

        /*
         * Returns the number of bindings for the descriptor set index specified by 'set'. If an invalid
         * set is specified, NSIZE32 is returned.
         */
        inline uint32_t binding_count(uint32_t set) const noexcept;

        // Returns the number of descriptor sets.
        constexpr uint32_t count(void) const noexcept;

        /*
         * Returns the descriptor bindings for a descriptor set index which is specified by 'set'.
         * If an invalid set is specified, nullptr is returned.
         */
        inline const VkDescriptorSetLayoutBinding* bindings(uint32_t set) const noexcept;
    };

    namespace descriptor
    {
        inline VkDescriptorBufferInfo make_buffer_info(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;
        inline VkDescriptorBufferInfo make_buffer_info(const Buffer& buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;

        inline VkDescriptorImageInfo make_image_info(VkImageView view, VkImageLayout layout, VkSampler sampler = VK_NULL_HANDLE) noexcept;
        inline VkDescriptorImageInfo make_image_info(const Texture& texture, uint32_t view_index);

        inline VkWriteDescriptorSetAccelerationStructureNV make_write_asnv(const VkAccelerationStructureNV* as, uint32_t count) noexcept;
        inline VkWriteDescriptorSetAccelerationStructureKHR make_write_askhr(const VkAccelerationStructureKHR* as, uint32_t count) noexcept;
        inline VkWriteDescriptorSetInlineUniformBlock make_inline_uniform_block(const void* data, uint32_t size) noexcept;

        inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos) noexcept;
        inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos) noexcept;
        inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views) noexcept;

        inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as_write) noexcept;
        inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as_write) noexcept;
        inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write) noexcept;
    }

    template<uint32_t S>
    class DescriptorManagerLayout
    {
    private:
        VkDevice m_device;
        std::array<VkDescriptorSetLayout, S> m_layouts;

        /*
         * Destroys all handles. This function is not inlined on purpose because of the implicit
         * try-catch block around the C-functions that are called inside this function. Inlining
         * this function might prevent other functions from being inlined, if this function is
         * called. To allow more optimization in favour of the caller, this function is prevented
         * from being inlined. Noinline is chosen because this is a template function and cannot
         * be implemented in a translation unit.
         */
        VKA_NOINLINE void destroy_handles(void) noexcept;

    public:
        // Default initialization, every member contains its default initialization.
        inline DescriptorManagerLayout(void) noexcept;

        /*
         * This constructor creates the descriptor-manager-layout and the internal handles are now
         * valid, if no error occured. If an error occured while creating, a std::runtime_error
         * exception is thrown with an appropriate message about the error. The descriptor-manager-
         * layout is created with a vulkan device handle which is specified by 'device' and a
         * descriptor set binding list which is specified by 'set_bindings'. This constructor has
         * the same functionality as the function 'create()'.
         */
        explicit inline DescriptorManagerLayout(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags = 0);

        // The DescriptorManagerLayout must be unique.
        DescriptorManagerLayout(const DescriptorManagerLayout&) = delete;
        DescriptorManagerLayout& operator= (const DescriptorManagerLayout&) = delete;

        /*
         * Moves another object of DescriptorManagerLayout into 'this'. 'this' now holds the ownership
         * of all the handles of the source object and the source object becomes invalidated. If 'this'
         * was created and is a valid object, 'this' is destroyed and replaced by the handles of the
         * moved object. The destructor on moved objects will not have any effect, unless it is
         * recreated.
         */
        inline DescriptorManagerLayout(DescriptorManagerLayout&& src) noexcept;
        inline DescriptorManagerLayout& operator= (DescriptorManagerLayout&& src) noexcept;

        // The destructor destroys all the vulkan handles.
        virtual inline ~DescriptorManagerLayout(void);

        /*
         * This function creates the descriptor-manager-layout and the internal handles are now valid,
         * if no error occured. If an error occured while creating, a std::runtime_error exception is
         * thrown with an appropriate message about the error. The descriptor-manager-layout is created
         * with a vulkan device handle which is specified by 'device' and a descriptor set binding list
         * which is specified by 'set_bindings'.
         */
        void create(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags = 0);

        /*
         * Destroys all the internal vulkan handles and sets them to VK_NULL_HANDLE except for parent
         * handles. After destroying, the object is reset to default state.
         */
        inline void destroy(void) noexcept;

        // Returns the number of descriptor set layouts.
        constexpr uint32_t count(void) const noexcept;

        /*
         * Returns the descriptor set layout at the specified index 'idx'. If the index is greater
         * than or equal to the number of owned descriptor set layouts, a std::out_of_range exception
         * is thrown.
         */
        inline VkDescriptorSetLayout handle(size_t idx) const;

        /*
         * Returns the descriptor set layout at the specified index 'idx'. This function does not
         * perform a range check.
         */
        inline VkDescriptorSetLayout operator[] (size_t idx) const noexcept;

        /*
         * Returns a pointer to all layouts.
         */
        inline const VkDescriptorSetLayout* layouts(void) const noexcept;

        // Returns true, if the DescriptorManagerLayout is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;
    };

    template<uint32_t S>
    class DescriptorManager
    {
    private:
        VkDevice m_device;
        VkDescriptorPool m_pool;
        std::array<VkDescriptorSet, S> m_sets;

        /*
         * Destroys all handles. This function is not inlined on purpose because of the implicit
         * try-catch block around the C-functions that are called inside this function. Inlining
         * this function might prevent other functions from being inlined, if this function is
         * called. To allow more optimization in favour of the caller, this function is prevented
         * from being inlined. Noinline is chosen because this is a template function and cannot
         * be implemented in a translation unit.
         */
        VKA_NOINLINE void destroy_handles(void) noexcept;

    public:
        static constexpr VkDescriptorPoolCreateFlags POOL_FLAGS = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        // Default initialization, every member contains its default initialization.
        inline DescriptorManager(void) noexcept;

        /*
         * This constructor creates the descriptor manager and the internal handles are now valid, if
         * no error occured. If an error occured while creating, a std::runtime_error exception is
         * thrown with an appropriate message about the error. The descriptor manager requires a device
         * and a descriptor pool to be created which are specifies by 'device' and 'pool' respectively.
         * 'layout' specifies the layouts that are used to create the descriptor sets with.
         *  This constructor has the same functionality as the function 'create()'.
         */
        explicit inline DescriptorManager(VkDevice device, VkDescriptorPool pool, const DescriptorManagerLayout<S>& layout);

        // The DescriptorManager must be unique.
        DescriptorManager(const DescriptorManager&) = delete;
        DescriptorManager& operator= (const DescriptorManager&) = delete;

        /*
         * Moves another object of DescriptorManager into 'this'. 'this' now holds the ownership of
         * all the handles of the source object and the source object becomes invalidated. If 'this'
         * was created and is a valid object, 'this' is destroyed and replaced by the handles of the
         * moved object. The destructor on moved objects will not have any effect, unless it is
         * recreated.
         */
        inline DescriptorManager(DescriptorManager&& src) noexcept;
        inline DescriptorManager& operator= (DescriptorManager&& src) noexcept;

        // The destructor destroys all the vulkan handles.
        virtual inline ~DescriptorManager(void);

        /*
         * This constructor creates the descriptor manager and the internal handles are now valid, if
         * no error occured. If an error occured while creating, a std::runtime_error exception is
         * thrown with an appropriate message about the error. The descriptor manager requires a device
         * and a descriptor pool to be created which are specifies by 'device' and 'pool' respectively.
         * 'layout' specifies the layouts that are used to create the descriptor sets with.
         */
        void create(VkDevice device, VkDescriptorPool pool, const DescriptorManagerLayout<S>& layout);

        /*
         * Destroys all the internal vulkan handles and sets them to VK_NULL_HANDLE except for parent
         * handles. After destroying, the object is reset to default state.
         */
        inline void destroy(void) noexcept;

        /*
         * Updates the owned descriptor sets. The descriptor sets are only updated, if the
         * DescriptorManager is a valid object. Otherwise, this function does nothing. The update/write
         * data is given by a DescriptorWriteList and is specified by 'write_list'. 'write_list' also
         * specifies the number of descriptor set writes. The number of descriptor writes corresponds
         * to the function 'DescriptorWriteList::count()'.
         */
        inline void update(const VkWriteDescriptorSet* writes, uint32_t count) noexcept;

        /*
         * Binds all the descriptor sets contained by the descriptor manager to the bind point specified
         * by 'bind_point'. A command buffer and a pipeline layout are required and are specified by
         * 'cbo' and 'pipe_layout'. If the descriptor manager is not a valid object, this function does
         * nothing.
         */
        inline void bind(VkCommandBuffer cbo, VkPipelineBindPoint bind_point, VkPipelineLayout pipe_layout) const noexcept;

        // Returns the number of descriptor sets.
        constexpr uint32_t count(void) const noexcept;

        /*
         * Returns the descriptor set at the specified index 'idx'. If the index is greater than or
         * equal to the number of owned descriptor sets, a std::out_of_range exception is thrown.
         */
        inline VkDescriptorSet handle(size_t idx) const;

        /*
         * Returns the descriptor set at the specified index 'idx'. This function does not perform a
         * range check.
         */
        inline VkDescriptorSet operator[] (size_t idx) const noexcept;

        /*
         * Returns a pointer to all descriptor sets.
         */
        inline const VkDescriptorSet* descriptor_sets(void) const noexcept;

        // Returns true, if the DescriptorManager is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;
    };
}

#include "vka_descriptor_inline_impl.inl"
