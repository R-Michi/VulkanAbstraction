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
    // forward declarations
    template<uint32_t S>
    class DescriptorSetArray;

    template<uint32_t S, uint32_t N>
    class DescriptorUpdateOperation;

    /**
     * @brief This is a helper class to create bindings for descriptor-sets.
     * @tparam S Specifies the number of used descriptor sets.
     */
    template<uint32_t S>
    class DescriptorSetBindingList
    {
    private:
        static constexpr const char* MSG_INVALID_INDEX = "[vka::DescriptorSetBindingList::push]: Invalid descriptor set index.";
        std::vector<VkDescriptorSetLayoutBinding> m_bindings[S];

    public:
        DescriptorSetBindingList(void) = default;
        ~DescriptorSetBindingList(void) = default;

        /**
         * @brief Adds a binding to the binding-list for a specified descriptor set.
         * @param set Specifies the descriptor set-index which also starts at 0.
         * @param type Specifies the descriptor type.
         * @param stages Specifies the shader stages where the current binding is used.
         * @param count Specifies the number of descriptors of the specified type.
         * @param immutable_samplers Specifies an array of vulkan sampler handles which is optional and
         * 'nullptr' by default.
         * @details The binding number for every individual set is incremented by 1, and starts at 0,
         * if this function is called. Different descriptor sets have independent bindings and have
         * its own index counter. Therefore, the number of bindings for a given set corresponds to the
         * number of push() calls for this specific set. The number of descriptor sets the binding-list
         * contains corresponds to the highest set-index + 1.
         * @note Fore more information see the documentation of VkDescriptorSetLayoutBinding.
         * Additionally, the size of the 'immutable_samplers'-array is not explicitly mentioned in the
         * documentation of VkDescriptorSetLayoutBinding. It might require as many VkSampler handles as
         * the number of descriptors specified for this binding (the value specified by 'count').
         */
        void push(uint32_t set, VkDescriptorType type, VkShaderStageFlags stages, uint32_t count = 1, const VkSampler* immutable_samplers = nullptr);

        /**
         * @param set Specifies the descriptor set index for which to return the binding count.
         * @return Returns the number of descriptors for the specified set.
         */
        inline uint32_t binding_count(uint32_t set) const noexcept;

        /**
         * @return Returns the number of descriptor sets.
         */
        constexpr uint32_t count(void) const noexcept;

        /**
         * @param set Specifies the descriptor set index for which to return the descriptor bindings.
         * @return Returns the descriptor bindings for the specified set.
         * @details If an invalid set is specified, nullptr is returned.
         */
        inline const VkDescriptorSetLayoutBinding* bindings(uint32_t set) const noexcept;
    };

    /**
     * @brief This class specifies an array of descriptor-set layouts.
     * @tparam S Specifies the number of descriptor-set layouts.
     */
    template<uint32_t S>
    class DescriptorLayoutArray
    {
    private:
        static constexpr const char* MSG_CREATE_FAILED = "[vka::DescriptorManagerLayout::create]: Failed to create descriptor set layout.";

        VkDevice m_device;
        std::array<VkDescriptorSetLayout, S> m_layouts;

        /**
         * @brief Destroys all handles.
         * @details This function is not inlined on purpose because of the implicit try-catch block
         * around the C-functions that are called inside this function. Inlining this function might
         * prevent other functions from being inlined, if this function is called. To allow more
         * optimization in favour of the caller, this function is prevented from being inlined.
         * Noinline is chosen because this is a template function and cannot be implemented in a
         * translation unit.
         */
        VKA_NOINLINE void destroy_handles(void) noexcept;

        /**
         * @brief Creates the descriptor-layout-array.
         * @param device Specifies the device from which the layouts are created.
         * @param set_bindings Specifies the used descriptor sets and bindings.
         * @param flags Specifies additional flags which are equal over all created layouts.
         * @throw std::runtime_error If an error occured while creating the descriptor-layouts.
         * @details After create, the internal handles are valid, if no error occured.
         */
        void internal_create(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags);

    public:
        /// @brief Default initialization.
        inline DescriptorLayoutArray(void) noexcept;

        /**
         * @brief Creates the descriptor-layout-array
         * @param device Specifies the device from which the layouts are created.
         * @param set_bindings Specifies the used descriptor sets and bindings.
         * @param flags Optionally specifies additional flags which are equal over all created
         * descriptor layouts.
         * @throw std::runtime_error If an error occured while creating the descriptor layouts.
         * @details After create, the internal handles are valid, if no error occured. This constructor
         * has the same functionality as the function create().
         */
        explicit inline DescriptorLayoutArray(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags = 0);

        // The DescriptorManagerLayout must be unique.
        DescriptorLayoutArray(const DescriptorLayoutArray&) = delete;
        DescriptorLayoutArray& operator= (const DescriptorLayoutArray&) = delete;

        /**
         * @brief Moves another object of DescriptorLayoutArray into 'this'.
         * @param src Specifies the source object to move.
         * @details 'this' now holds the ownership of all the handles of the source object and the
         * source object becomes invalidated. If 'this' was created and is a valid object, 'this'
         * is destroyed and replaced by the handles of the moved object. The destructor on moved
         * objects will not have any effect, unless it was recreated.
         */
        inline DescriptorLayoutArray(DescriptorLayoutArray&& src) noexcept;
        inline DescriptorLayoutArray& operator= (DescriptorLayoutArray&& src) noexcept;

        /// @brief Destroys all the vulkan handles.
        virtual inline ~DescriptorLayoutArray(void);

        /**
         * @brief Creates the descriptor-layout-array
         * @param device Specifies the device from which the layouts are created.
         * @param set_bindings Specifies the used descriptor sets and bindings.
         * @param flags Optionally specifies additional flags which are equal over all created
         * descriptor layouts.
         * @throw std::runtime_error If an error occured while creating the descriptor layouts.
         * @details After create, the internal handles are valid, if no error occured.
         */
        inline void create(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags = 0);

        /**
         * @brief Destroys all the internal vulkan handles.
         * @details After the handles are destroyed they are reset to VK_NULL_HANDLE. Moreover, parent
         * handles are not destroyed by this function.
         */
        inline void destroy(void) noexcept;

        /**
         * @brief Creates the descriptor-sets from the layout array.
         * @param pool Specifies the pool from which the descriptor-sets are allocated.
         * @return Returns a valid descriptor-set-array.
         * @details If the creation of the descriptor-set array failed, the constructor of it throws
         * an exception. However, if this function returns, the descriptor-set array is valid.
         */
        inline DescriptorSetArray<S> create_sets(VkDescriptorPool pool);

        /// @return Returns the number of descriptor set layouts which equals to the template argument.
        constexpr uint32_t count(void) const noexcept;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @throw std::out_of_range If the index is invalid.
         * @details This function does perform a range check on the index and throws if the index
         * is greater than or equal to the number of descriptor-set-layouts. More formally throws,
         * if 'idx' >= 'S' (S is the template argument).
         */
        inline VkDescriptorSetLayout handle(size_t idx) const;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @details This function does not perform a range check on the index. Therefore any access
         * at an invalid index will cause undefined behaviour.
         */
        inline VkDescriptorSetLayout handle_u(uint32_t idx) const noexcept;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @details This operator does not perform a range check on the index. Therefore any access
         * at an invalid index will cause undefined behaviour.
         */
        inline VkDescriptorSetLayout operator[] (size_t idx) const noexcept;

        /// @return Returns a pointer to the descriptor-set-layouts.
        inline const VkDescriptorSetLayout* layouts(void) const noexcept;

        /**
         * @return Returns the device used by the descriptor-set-layouts.
         * @note After move the old device remains and is not reset to VK_NULL_HANDLE.
         */
        inline VkDevice device(void) const noexcept;

        /// @return Returns true, if the DescriptorLayoutArray is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;
    };

    /**
     * @brief This class specifies an array of descriptor-sets.
     * @tparam S Specifies the number of descriptor-sets.
     */
    template<uint32_t S>
    class DescriptorSetArray
    {
    private:
        static constexpr const char* MSG_CREATE_FAILED = "[vka::DescriptorManager::create]: Failed to allocate descriptor sets.";
        static constexpr const char* MSG_UPDATE_OP = "[vka::DescriptorManager::op_update]: Cannot create update-operation, if the descriptor-set array is invalid.";

        VkDevice m_device;
        VkDescriptorPool m_pool;
        std::array<VkDescriptorSet, S> m_sets;

        /**
         * @brief Destroys all handles.
         * @details This function is not inlined on purpose because of the implicit try-catch block
         * around the C-functions that are called inside this function. Inlining this function might
         * prevent other functions from being inlined, if this function is called. To allow more
         * optimization in favour of the caller, this function is prevented from being inlined.
         * Noinline is chosen because this is a template function and cannot be implemented in a
         * translation unit.
         */
        VKA_NOINLINE void destroy_handles(void) noexcept;

        /**
         * @brief Creates the descriptor-sets.
         * @param pool Specifies the pool from which the sets are allocated.
         * @param layout Specifies the used descriptor-set-layouts.
         * @throw std::runtime_error If an error occured while creating the descriptor-sets.
         * @details After create, the internal handles are valid, if no error occured.
         */
        void internal_create(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout);

    public:
        template<uint32_t N>
        using UpdateOperation = DescriptorUpdateOperation<S, N>;

        /**
         * @brief Required flags for the descriptor pool.
         * @details As this class frees the allocated descriptor sets in the destructor, an additional
         * flag must be set in the descriptor-pool.
         */
        static constexpr VkDescriptorPoolCreateFlags POOL_FLAGS = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        ///@brief Default initialization.
        inline DescriptorSetArray(void) noexcept;

        /**
         * Creates the descriptor-sets.
         * @param pool Specifies the pool from which the sets are allocated.
         * @param layout Specifies the used descriptor-set-layouts.
         * @throw std::runtime_error If an error occured while creating the descriptor-sets.
         * @details After create, the internal handles are valid, if no error occured. This constructor
         * has the same functionality as the function create().
         */
        explicit inline DescriptorSetArray(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout);

        // The DescriptorManager must be unique.
        DescriptorSetArray(const DescriptorSetArray&) = delete;
        DescriptorSetArray& operator= (const DescriptorSetArray&) = delete;

        /**
         * @brief Moves another object of DescriptorSetArray into 'this'.
         * @param src Specifies the source object to move.
         * @details 'this' now holds the ownership of all the handles of the source object and the
         * source object becomes invalidated. If 'this' was created and is a valid object, 'this'
         * is destroyed and replaced by the handles of the moved object. The destructor on moved
         * objects will not have any effect, unless it was recreated.
         */
        inline DescriptorSetArray(DescriptorSetArray&& src) noexcept;
        inline DescriptorSetArray& operator= (DescriptorSetArray&& src) noexcept;

        /// @brief Destroys all created vulkan handles.
        virtual inline ~DescriptorSetArray(void);

        /**
         * @brief Creates the descriptor-sets.
         * @param pool Specifies the pool from which the sets are allocated.
         * @param layout Specifies the used descriptor-set-layouts.
         * @throw std::runtime_error If an error occured while creating the descriptor-sets.
         * @details After create, the internal handles are valid, if no error occured.
         */
        inline void create(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout);

        /**
         * @brief Destroys all created vulkan handles.
         * @details After the handles are destroyed they are reset to VK_NULL_HANDLE. Moreover, parent
         * handles are not destroyed by this function.
         */
        inline void destroy(void) noexcept;

        /**
         * @brief Creates an update operation for the descriptor-sets.
         * @tparam N Specifies the maximum number of descriptor-writes, or 0, if the number of
         * descriptor writes should be dynamic.
         * @return Returns a descriptor update operation.
         * @throw std::runtime_error If the descriptor-set array is not a valid object.
         */
        template<uint32_t N>
        inline UpdateOperation<N> op_update(void);

        /**
         * @brief Binds all the descriptor-sets.
         * @param cbo Specifies the command buffer in which the bind command should be recorded.
         * @param bind_point Specifies the binding point of the descriptor-sets.
         * @param pipe_layout Specifies the pipeline layout that uses the descriptor sets.
         * @details If the descriptor manager is not a valid object, this function does nothing.
         */
        inline void bind(VkCommandBuffer cbo, VkPipelineBindPoint bind_point, VkPipelineLayout pipe_layout) const;

        /// @return Returns the number of descriptor sets.
        constexpr uint32_t count(void) const noexcept;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @throw std::out_of_range If the index is invalid.
         * @details This function does perform a range check on the index and throws if the index
         * is greater than or equal to the number of descriptor-sets. More formally throws, if
         * 'idx' >= 'S' (S is the template argument).
         */
        inline VkDescriptorSet handle(size_t idx) const;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @details This function does not perform a range check on the index. Therefore any access
         * at an invalid index will cause undefined behaviour.
         */
        inline VkDescriptorSet handle_u(size_t idx) const;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @details This operator does not perform a range check on the index. Therefore any access
         * at an invalid index will cause undefined behaviour.
         */
        inline VkDescriptorSet operator[] (size_t idx) const noexcept;

        /// @return Returns a pointer to the descriptor sets.
        inline const VkDescriptorSet* sets(void) const noexcept;

        /**
         * @return Returns the device used by the descriptor-sets.
         * @note After move the old device remains and is not reset to VK_NULL_HANDLE.
         */
        inline VkDevice device(void) const noexcept;

        /**
         * @return Returns the descriptor-pool used by the descriptor-sets.
         * @note After move the old descriptor-pool remains and is not reset to VK_NULL_HANDLE.
         */
        inline VkDescriptorPool pool(void) const noexcept;

        /// @return Returns true, if the DescriptorSetArray is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;
    };

    /**
     * @brief Helper class for a descriptor-set update operation.
     * @tparam S Specifies the number of descriptor sets. The initialization requires a descriptor-set
     * array which requires the number of descriptor sets.
     * @tparam N Specifies the maximum number of descriptor writes. 0 is reserved to indicate a dynamic
     * number of descriptor writes.
     */
    template<uint32_t S, uint32_t N>
    class DescriptorUpdateOperation
    {
    private:
        static constexpr const char* MSG_INDEX = "[vka::DescriptorUpdater::write]: Too many writes.";

        const DescriptorSetArray<S>* m_sets;
        VkWriteDescriptorSet m_writes[N];
        uint32_t m_idx;

    public:

        /**
         * @brief Initializes a descriptor-update operation with a descriptor-set array.
         * @param sets Specifies the descriptor-set array on which the operation is performed.
         */
        explicit DescriptorUpdateOperation(const DescriptorSetArray<S>& sets) noexcept :
            m_sets(&sets), m_writes{}, m_idx(0)
        {}

        /**
         * @brief Creates a descriptor write for buffers.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param infos Specifies the update information for the descriptors. This is an array of
         * info-structures and its length must correspond to the number of affected descriptors.
         * More formally, the array-length must be equal to the value of 'count'.
         * @throw std::out_of_range If the maximum number of descriptor-writes has been exceeded.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos)
        {
            detail::error::check_idx<N>(this->m_idx, MSG_INDEX);
            this->m_writes[this->m_idx++] = detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, count, type, infos);
        }

        /**
         * @brief Creates a descriptor write for images.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param infos Specifies the update information for the descriptors. This is an array of
         * info-structures and its length must correspond to the number of affected descriptors.
         * More formally, the array-length must be equal to the value of 'count'.
         * @throw std::out_of_range If the maximum number of descriptor-writes has been exceeded.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos)
        {
            detail::error::check_idx<N>(this->m_idx, MSG_INDEX);
            this->m_writes[this->m_idx++] = detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, count, type, infos);
        }

        /**
         * @brief Creates a descriptor write for (texel-) buffer views.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param views Specifies the update information for the descriptors. This is an array of
         * buffer views and its length must correspond to the number of affected descriptors.
         * More formally, the array-length must be equal to the value of 'count'.
         * @throw std::out_of_range If the maximum number of descriptor-writes has been exceeded.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views)
        {
            detail::error::check_idx<N>(this->m_idx, MSG_INDEX);
            this->m_writes[this->m_idx++] = detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, count, type, views);
        }

        /**
         * @brief Creates a descriptor write for nvidia acceleration structures.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param as_write Specifies the update information for the descriptors. This is the extension
         * structure for a descriptor write for nvidia acceleration structures.
         * @throw std::out_of_range If the maximum number of descriptor-writes has been exceeded.
         * @note The number of affected descriptors (also referred to the parameter 'count') is already
         * specified by the extension structure. Additionally the used descriptor type is
         * VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV because it is the only type that can be used
         * with this extension structure.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as_write)
        {
            detail::error::check_idx<N>(this->m_idx, MSG_INDEX);
            this->m_writes[this->m_idx++] = detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, as_write);
        }

        /**
         * @brief Creates a descriptor write for khronos acceleration structures.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param as_write Specifies the update information for the descriptors. This is the extension
         * structure for a descriptor write for khronos acceleration structures.
         * @throw std::out_of_range If the maximum number of descriptor-writes has been exceeded.
         * @note The number of affected descriptors (also referred to the parameter 'count') is already
         * specified by the extension structure. Additionally the used descriptor type is
         * VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR because it is the only type that can be used
         * with this extension structure.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as_write)
        {
            detail::error::check_idx<N>(this->m_idx, MSG_INDEX);
            this->m_writes[this->m_idx++] = detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, as_write);
        }

        /**
         * @brief Creates a descriptor write for an inline uniform block.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the offset in bytes in the data buffer for the uniform block.
         * @param iub_write Specifies the write data for the uniform block. This is the extension
         * structure for a descriptor write for an inline uniform block.
         * @throw std::out_of_range If the maximum number of descriptor-writes has been exceeded.
         * @note The data to write as well as the size of the data to write is already specified by the
         * extension structure. However, the offset in the data buffer is specified by this function.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write)
        {
            detail::error::check_idx<N>(this->m_idx, MSG_INDEX);
            this->m_writes[this->m_idx++] = detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, iub_write);
        }

        /// @brief Executes the descriptor-update operation.
        void execute(void) const
        {
            vkUpdateDescriptorSets(this->m_sets->device(), this->m_idx, this->m_writes, 0, nullptr);
        }
    };

    /**
     * @brief Helper class for a descriptor-set update operation.
     * @tparam S Specifies the number of descriptor sets. The initialization requires a descriptor-set
     * array which requires the number of descriptor sets.
     * @details This template specialization uses a dynamic number of descriptor writes.
     */
    template<uint32_t S>
    class DescriptorUpdateOperation<S, 0>
    {
    private:
        const DescriptorSetArray<S>* m_sets;
        std::vector<VkWriteDescriptorSet> m_writes;

    public:

        /**
         * @brief Initializes a descriptor-update operation with a descriptor-set array.
         * @param sets Specifies the descriptor-set array on which the operation is performed.
         */
        explicit DescriptorUpdateOperation(const DescriptorSetArray<S>& sets) noexcept :
            m_sets(&sets)
        {}

        /**
         * @brief Creates a descriptor write for buffers.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param infos Specifies the update information for the descriptors. This is an array of
         * info-structures and its length must correspond to the number of affected descriptors.
         * More formally, the array-length must be equal to the value of 'count'.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, count, type, infos));
        }

        /**
         * @brief Creates a descriptor write for images.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param infos Specifies the update information for the descriptors. This is an array of
         * info-structures and its length must correspond to the number of affected descriptors.
         * More formally, the array-length must be equal to the value of 'count'.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, count, type, infos));
        }

        /**
         * @brief Creates a descriptor write for (texel-) buffer views.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param views Specifies the update information for the descriptors. This is an array of
         * buffer views and its length must correspond to the number of affected descriptors.
         * More formally, the array-length must be equal to the value of 'count'.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, count, type, views));
        }

        /**
         * @brief Creates a descriptor write for nvidia acceleration structures.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param as_write Specifies the update information for the descriptors. This is the extension
         * structure for a descriptor write for nvidia acceleration structures.
         * @note The number of affected descriptors (also referred to the parameter 'count') is already
         * specified by the extension structure. Additionally the used descriptor type is
         * VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV because it is the only type that can be used
         * with this extension structure.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as_write)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, as_write));
        }

        /**
         * @brief Creates a descriptor write for khronos acceleration structures.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor, if the target binding contains an
         * array of descriptors.
         * @param as_write Specifies the update information for the descriptors. This is the extension
         * structure for a descriptor write for khronos acceleration structures.
         * @note The number of affected descriptors (also referred to the parameter 'count') is already
         * specified by the extension structure. Additionally the used descriptor type is
         * VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR because it is the only type that can be used
         * with this extension structure.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as_write)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, as_write));
        }

        /**
         * @brief Creates a descriptor write for an inline uniform block.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the offset in bytes in the data buffer for the uniform block.
         * @param iub_write Specifies the write data for the uniform block. This is the extension
         * structure for a descriptor write for an inline uniform block.
         * @note The data to write as well as the size of the data to write is already specified by the
         * extension structure. However, the offset in the data buffer is specified by this function.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, iub_write));
        }

        /// @brief Executes the descriptor-update operation.
        void execute(void) const
        {
            vkUpdateDescriptorSets(this->m_sets->device(), this->m_writes.size(), this->m_writes.data(), 0, nullptr);
        }
    };

    namespace descriptor
    {
        /**
         * @brief Creates a vulkan VkDescriptorBufferInfo structure.
         * @param buffer Specifies the vulkan buffer which should be included in the buffer info.
         * @param offset Specifies the offset in bytes within the buffer.
         * @param range Specifies the affected size in bytes of the buffer starting at 'offset'.
         * @return Returns the vulkan VkDescriptorBufferInfo created from those parameters.
         */
        inline VkDescriptorBufferInfo make_buffer_info(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;

        /**
         * @brief Creates a vulkan VkDescriptorBufferInfo structure from a vka::Buffer.
         * @param buffer Specifies the buffer.
         * @param offset Specifies the offset in bytes within the buffer.
         * @param range Specifies the affected size in bytes of the buffer starting at 'offset'.
         * @return Returns the vulkan VkDescriptorBufferInfo structure created from the buffer.
         */
        inline VkDescriptorBufferInfo make_buffer_info(const Buffer& buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;

        /**
         * @brief Creates a vulkan VkDescriptorImageInfo structure.
         * @param view Specifies the vulkan image view which should be included in the image info.
         * @param layout Specifies the layout of the image (-region) referenced by the view.
         * @param sampler Optionally specifies a sampler which is used together with the image view.
         * @return Returns the vulkan VkDescriptorImageInfo created from those parameters.
         */
        inline VkDescriptorImageInfo make_image_info(VkImageView view, VkImageLayout layout, VkSampler sampler = VK_NULL_HANDLE) noexcept;

        /**
         * @brief Creates a vulkan VkDescriptorImageInfo structure from a vka::Texture.
         * @param texture Specifies the texture.
         * @param view_index Specifies a valid image view index from the texture.
         * @return Returns the vulkan VkDescriptorImageInfo structure created from the texture.
         * @throw std::out_of_range If the image view index is invalid.
         * @details Internally the image layout VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL is used.
         */
        inline VkDescriptorImageInfo make_image_info(const Texture& texture, uint32_t view_index);

        /**
         * @brief Creates the extension structure for a descriptor-write for nvidia acceleration
         * structures.
         * @param as Specifies an array of nvidia acceleration structures.
         * @param count Specifies the number of nvidia acceleration structures (number of acceleration
         * structures in the specified array).
         * @return Returns the extension structure for a descriptor-write for nvidia acceleration
         * structures.
         */
        inline VkWriteDescriptorSetAccelerationStructureNV make_write_asnv(const VkAccelerationStructureNV* as, uint32_t count) noexcept;

        /**
         * @brief Creates the extension structure for a descriptor-write for khronos acceleration
         * structures.
         * @param as Specifies an array of khronos acceleration structures.
         * @param count Specifies the number of khronos acceleration structures (number of acceleration
         * structures in the specified array).
         * @return Returns the extension structure for a descriptor-write for khronos acceleration
         * structures.
         */
        inline VkWriteDescriptorSetAccelerationStructureKHR make_write_askhr(const VkAccelerationStructureKHR* as, uint32_t count) noexcept;

        /**
         * @brief Creates the extension structure for a descriptor-write for an inline uniform block.
         * @param data Specifies the data to write into the uniform block.
         * @param size Specifies the size of the data.
         * @return Returns the extension structure for a descriptor-write for an inline uniform block.
         */
        inline VkWriteDescriptorSetInlineUniformBlock make_inline_uniform_block(const void* data, uint32_t size) noexcept;
    }
}

#if 0
template<typename Info, uint32_t N>
class DescriptorInfoArray : public detail::descriptor::DescriptorInfoArray<Info, N>
{
    static_assert("[vka::DescriptorInfoArray]: Invalid info type.");
};

template<uint32_t N>
class DescriptorInfoArray<VkDescriptorBufferInfo, N> : public detail::descriptor::DescriptorInfoArray<VkDescriptorBufferInfo, N>
{
public:
    DescriptorInfoArray(void) = default;
    ~DescriptorInfoArray(void) = default;

    inline VkWriteDescriptorSet make_set(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, uint32_t count = N) const noexcept;
};

template<uint32_t N>
class DescriptorInfoArray<VkDescriptorImageInfo, N> : public detail::descriptor::DescriptorInfoArray<VkDescriptorImageInfo, N>
{
public:
    DescriptorInfoArray(void) = default;
    ~DescriptorInfoArray(void) = default;

    inline VkWriteDescriptorSet make_set(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, uint32_t count = N) const noexcept;
};

template<uint32_t N>
class DescriptorInfoArray<VkBufferView, N> : public detail::descriptor::DescriptorInfoArray<VkBufferView, N>
{
public:
    DescriptorInfoArray(void) = default;
    ~DescriptorInfoArray(void) = default;

    inline VkWriteDescriptorSet make_set(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, uint32_t count = N) const noexcept;
};


template<uint32_t N>
class DescriptorInfoArray<VkAccelerationStructureNV, N> : public detail::descriptor::DescriptorInfoArray<VkAccelerationStructureNV, N>
{
private:
    VkWriteDescriptorSetAccelerationStructureNV m_extension;

public:
    DescriptorInfoArray(void);
    ~DescriptorInfoArray(void) = default;

    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count = N);
};

template<uint32_t N>
class DescriptorInfoArray<VkAccelerationStructureKHR, N> : public detail::descriptor::DescriptorInfoArray<VkAccelerationStructureKHR, N>
{
private:
    VkWriteDescriptorSetAccelerationStructureKHR m_extension;

public:
    DescriptorInfoArray(void);
    ~DescriptorInfoArray(void) = default;

    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count = N);
};

template<>
class DescriptorInfoArray<const void*, 0> : public detail::descriptor::DescriptorInfoArray<const void*, 0>
{
private:
    VkWriteDescriptorSetInlineUniformBlock m_extension;

public:
    DescriptorInfoArray(void);
    DescriptorInfoArray(const void* data, uint32_t size);
    ~DescriptorInfoArray(void) = default;

    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding);
};
#endif

#include "vka_descriptor_inline_impl.inl"
