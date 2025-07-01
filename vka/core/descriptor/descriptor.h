/**
 * @brief Helper classes and functions for creating and updating descriptors.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

namespace vka
{
    // forward declarations
    template<uint32_t S>
    class DescriptorLayoutArray;

    template<uint32_t S>
    class DescriptorSetArray;

    template<uint32_t S, uint32_t N>
    class DescriptorUpdateOperation;

    /**
     * @brief This is a helper class to create bindings for descriptor-sets.
     * @tparam S Specifies the number of used descriptor sets.
     */
    template<uint32_t S>
    class DescriptorSetBindingList final
    {
    private:
        static constexpr char MSG_INVALID_INDEX[] = "[vka::DescriptorSetBindingList::push]: Invalid descriptor set index.";
        std::vector<VkDescriptorSetLayoutBinding> m_bindings[S];

    public:
        DescriptorSetBindingList() = default;
        ~DescriptorSetBindingList() = default;

        /**
         * @brief Adds a binding to the binding-list for a specified descriptor set.
         * @param set Specifies the descriptor set-index which also starts at 0.
         * @param type Specifies the descriptor type.
         * @param stages Specifies the shader stages where the current binding is used.
         * @param count Specifies the number of descriptors.
         * @param immutable_samplers Specifies an array of vulkan sampler handles which is optional and 'nullptr' by
         * default.
         * @details The binding number for every set is incremented by 1 and starts at 0 if this function is called.
         * Different descriptor sets have independent bindings and have their own index counter. Therefore, the number
         * of bindings for a given set corresponds to the number of push() calls for this specific set. The number of
         * descriptor sets the binding-list contains corresponds to the highest set-index + 1.
         * @note For more information see the documentation of VkDescriptorSetLayoutBinding. Additionally, the size of
         * the 'immutable_samplers'-array is not explicitly mentioned in the documentation of
         * VkDescriptorSetLayoutBinding. It might require as many VkSampler handles as the number of descriptors
         * specified for this binding (the value specified by 'count').
         */
        inline void push(uint32_t set, VkDescriptorType type, VkShaderStageFlags stages, uint32_t count = 1, const VkSampler* immutable_samplers = nullptr);

        /**
         * @param set Specifies the descriptor set index for which to return the binding count.
         * @return Returns the number of descriptors for the specified set.
         */
        inline uint32_t binding_count(uint32_t set) const noexcept;

        /**
         * @return Returns the number of descriptor sets.
         */
        static constexpr uint32_t count() noexcept;

        /**
         * @param set Specifies the descriptor set index for which to return the descriptor bindings.
         * @return Returns the descriptor bindings for the specified set.
         * @details If an invalid set is specified, nullptr is returned.
         */
        inline const VkDescriptorSetLayoutBinding* bindings(uint32_t set) const noexcept;

        /**
         * @brief Creates the descriptor-layout from the binding list.
         * @return Returns a valid descriptor-layout array.
         * @details If the creation of the descriptor-layout array failed, the constructor of it throws an exception.
         */
        inline DescriptorLayoutArray<S> create_layouts(VkDevice device, VkDescriptorSetLayoutCreateFlags flags = 0) const;
    };

    /**
     * @brief This class specifies an array of descriptor-set layouts.
     * @tparam S Specifies the number of descriptor-set layouts.
     */
    template<uint32_t S>
    class DescriptorLayoutArray final
    {
    private:
        static constexpr char MSG_CREATE_FAILED[] = "[vka::DescriptorManagerLayout::create]: Failed to create descriptor set layout.";

        VkDevice m_device;
        std::array<VkDescriptorSetLayout, S> m_layouts;

        /**
         * @brief Destroys all handles.
         * @details This function is not inlined on purpose because of the implicit try-catch block around the
         * C-functions that are called inside this function. Inlining this function might prevent other functions from
         * being inlined if this function is called. To allow more optimization in favor of the caller, this function is
         * prevented from being inlined. Noinline is chosen because this is a template function and cannot be
         * implemented in a translation unit.
         */
        void destroy_handles() noexcept;

        /**
         * @brief Creates the descriptor-layout-array.
         * @param device Specifies the device from which the layouts are created.
         * @param set_bindings Specifies the used descriptor sets and bindings.
         * @param flags Specifies additional flags which are equal over all created layouts.
         * @throw std::runtime_error If an error occurred while creating the descriptor-layouts.
         * @details After creation, the internal handles are valid if no error occurred.
         */
        void internal_create(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags);

    public:
        /// @brief Default initialization.
        inline DescriptorLayoutArray() noexcept;

        /**
         * @brief Creates the descriptor-layout-array
         * @param device Specifies the device from which the layouts are created.
         * @param set_bindings Specifies the used descriptor sets and bindings.
         * @param flags Optionally specifies additional flags which are equal over all created descriptor layouts.
         * @throw std::runtime_error If an error occurred while creating the descriptor layouts.
         * @details After creation, the internal handles are valid if no error occurred. This constructor has the same
         * functionality as the function create().
         */
        explicit inline DescriptorLayoutArray(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags = 0);

        // The DescriptorManagerLayout must be unique.
        DescriptorLayoutArray(const DescriptorLayoutArray&) = delete;
        DescriptorLayoutArray& operator= (const DescriptorLayoutArray&) = delete;

        /**
         * @brief Moves the ownership of a descriptor-layout array to a different object.
         * @param src Specifies the source object to move.
         * @details The source object image becomes invalidated. If the move destination has been created before and is
         * a valid object, it gets destroyed.
         */
        inline DescriptorLayoutArray(DescriptorLayoutArray&& src) noexcept;
        inline DescriptorLayoutArray& operator= (DescriptorLayoutArray&& src) noexcept;

        /// @brief Destroys all the vulkan handles.
        inline ~DescriptorLayoutArray();

        /**
         * @brief Creates the descriptor-layout-array
         * @param device Specifies the device from which the layouts are created.
         * @param set_bindings Specifies the used descriptor sets and bindings.
         * @param flags Optionally specifies additional flags which are equal over all created descriptor layouts.
         * @throw std::runtime_error If an error occurred while creating the descriptor layouts.
         * @details After creation, the internal handles are valid if no error occurred.
         */
        inline void create(VkDevice device, const DescriptorSetBindingList<S>& set_bindings, VkDescriptorSetLayoutCreateFlags flags = 0);

        /**
         * @brief Destroys all the internal vulkan handles.
         * @details After the handles are destroyed, they are reset to VK_NULL_HANDLE. Moreover, this function does not
         * destroy parent handles.
         */
        inline void destroy() noexcept;

        /// @return Returns the number of descriptor set layouts which equals to the template argument.
        static constexpr uint32_t count() noexcept;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @throw std::out_of_range If the index is invalid.
         * @details This function does perform a range check on the index and throws if the index is greater than or
         * equal to the number of descriptor-set-layouts. More formally throws, if 'idx' >= 'S' (S is the template
         * argument).
         */
        inline VkDescriptorSetLayout handle(size_t idx) const;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @details This function does not perform a range check on the index. Therefore, any access to an invalid index
         * will cause undefined behavior.
         */
        inline VkDescriptorSetLayout handle_u(uint32_t idx) const noexcept;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @details This operator does not perform a range check on the index. Therefore, any access to an invalid index
         * will cause undefined behavior.
         */
        inline VkDescriptorSetLayout operator[] (size_t idx) const noexcept;

        /// @return Returns a pointer to the descriptor-set-layouts.
        inline const VkDescriptorSetLayout* layouts() const noexcept;

        /**
         * @return Returns the device used by the descriptor-set-layouts.
         * @note After move the old device remains and is not reset to VK_NULL_HANDLE.
         */
        inline VkDevice device() const noexcept;

        /// @return Returns true if the DescriptorLayoutArray is a valid object and false otherwise.
        inline bool is_valid() const noexcept;

        /**
         * @brief Creates the descriptor-sets from the layout array.
         * @param pool Specifies the pool from which the descriptor-sets are allocated.
         * @return Returns a valid descriptor-set-array.
         * @details If the creation of the descriptor-set array failed, the constructor of it throws an exception.
         */
        inline DescriptorSetArray<S> create_sets(VkDescriptorPool pool) const;
    };

    /**
     * @brief This class specifies an array of descriptor-sets.
     * @tparam S Specifies the number of descriptor-sets.
     */
    template<uint32_t S>
    class DescriptorSetArray final
    {
    private:
        static constexpr char MSG_CREATE_FAILED[] = "[vka::DescriptorManager::create]: Failed to allocate descriptor sets.";
        static constexpr char MSG_UPDATE_OP[] = "[vka::DescriptorManager::op_update]: Cannot create update-operation, if the descriptor-set array is invalid.";

        VkDevice m_device;
        VkDescriptorPool m_pool;
        std::array<VkDescriptorSet, S> m_sets;

        /**
         * @brief Destroys all handles.
         * @details This function is not inlined on purpose because of the implicit try-catch block around the
         * C-functions that are called inside this function. Inlining this function might prevent other functions from
         * being inlined if this function is called. To allow more optimization in favor of the caller, this function is
         * prevented from being inlined. Noinline is chosen because this is a template function and cannot be
         * implemented in a translation unit.
         */
        void destroy_handles() noexcept;

        /**
         * @brief Creates the descriptor-sets.
         * @param pool Specifies the pool from which the sets are allocated.
         * @param layout Specifies the used descriptor-set-layouts.
         * @throw std::runtime_error If an error occurred while creating the descriptor-sets.
         * @details After creation, the internal handles are valid if no error occurred.
         */
        void internal_create(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout);

    public:
        /**
         * @brief Required flags for the descriptor pool.
         * @details As this class frees the allocated descriptor sets in the destructor, an additional
         * flag must be set in the descriptor-pool.
         */
        static constexpr VkDescriptorPoolCreateFlags POOL_FLAGS = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        ///@brief Default initialization.
        inline DescriptorSetArray() noexcept;

        /**
         * Creates the descriptor-sets.
         * @param pool Specifies the pool from which the sets are allocated.
         * @param layout Specifies the used descriptor-set-layouts.
         * @throw std::runtime_error If an error occurred while creating the descriptor-sets.
         * @details After creation, the internal handles are valid if no error occurred. This constructor
         * has the same functionality as the function create().
         */
        explicit inline DescriptorSetArray(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout);

        // The DescriptorManager must be unique.
        DescriptorSetArray(const DescriptorSetArray&) = delete;
        DescriptorSetArray& operator= (const DescriptorSetArray&) = delete;

        /**
         * @brief Moves the ownership of a descriptor-set array to a different object.
         * @param src Specifies the source object to move.
         * @details The source descriptor-set array becomes invalidated. If the move destination has been created before
         * and is a valid object, it gets destroyed.
         */
        inline DescriptorSetArray(DescriptorSetArray&& src) noexcept;
        inline DescriptorSetArray& operator= (DescriptorSetArray&& src) noexcept;

        /// @brief Destroys all created vulkan handles.
        inline ~DescriptorSetArray();

        /**
         * @brief Creates the descriptor-sets.
         * @param pool Specifies the pool from which the sets are allocated.
         * @param layout Specifies the used descriptor-set-layouts.
         * @throw std::runtime_error If an error occurred while creating the descriptor-sets.
         * @details After creation, the internal handles are valid if no error occurred.
         */
        inline void create(VkDescriptorPool pool, const DescriptorLayoutArray<S>& layout);

        /**
         * @brief Destroys all created vulkan handles.
         * @details After the handles are destroyed, they are reset to VK_NULL_HANDLE. Moreover, this function does not
         * destroy parent handles.
         */
        inline void destroy() noexcept;

        /**
         * @brief Binds all the descriptor-sets.
         * @param cbo Specifies the command buffer in which the bind command should be recorded.
         * @param bind_point Specifies the binding point of the descriptor-sets.
         * @param pipe_layout Specifies the pipeline layout that uses the descriptor sets.
         * @details If the descriptor manager is not a valid object, this function does nothing.
         */
        inline void bind(VkCommandBuffer cbo, VkPipelineBindPoint bind_point, VkPipelineLayout pipe_layout) const;

        /// @return Returns the number of descriptor sets.
        static constexpr uint32_t count() noexcept;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @throw std::out_of_range If the index is invalid.
         * @details This function does perform a range check on the index and throws if the index is greater than or
         * equal to the number of descriptor-sets. More formally throws, if 'idx' >= 'S' (S is the template argument).
         */
        inline VkDescriptorSet handle(size_t idx) const;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @details This function does not perform a range check on the index. Therefore, any access to an invalid index
         * will cause undefined behavior.
         */
        inline VkDescriptorSet handle_u(size_t idx) const;

        /**
         * @param idx Specifies the index of the handle to return.
         * @return Returns the handle at the specified index.
         * @details This operator does not perform a range check on the index. Therefore, any access to an invalid index
         * will cause undefined behavior.
         */
        inline VkDescriptorSet operator[] (size_t idx) const noexcept;

        /// @return Returns a pointer to the descriptor sets.
        inline const VkDescriptorSet* sets() const noexcept;

        /**
         * @return Returns the device used by the descriptor-sets.
         * @note After move the old device remains and is not reset to VK_NULL_HANDLE.
         */
        inline VkDevice device() const noexcept;

        /**
         * @return Returns the descriptor-pool used by the descriptor-sets.
         * @note After move the old descriptor-pool remains and is not reset to VK_NULL_HANDLE.
         */
        inline VkDescriptorPool pool() const noexcept;

        /// @return Returns true if the DescriptorSetArray is a valid object and false otherwise.
        inline bool is_valid() const noexcept;

        /**
         * @brief Creates an update operation for the descriptor-sets.
         * @tparam N Specifies the maximum number of descriptor-writes, or 0, if the number of descriptor-writes should
         * be dynamic.
         * @return Returns a descriptor update operation.
         * @throw std::runtime_error If the descriptor-set array is not a valid object.
         */
        template<uint32_t N>
        inline DescriptorUpdateOperation<S, N> op_update() const;
    };

    /**
     * @brief Helper class for a descriptor-set update operation.
     * @tparam S Specifies the number of descriptor sets. The initialization requires a descriptor-set
     * array which requires the number of descriptor sets.
     * @tparam N Specifies the maximum number of descriptor-writes. 0 is reserved to indicate a dynamic number of
     * descriptor-writes.
     * @note This class has no default constructor!
     */
    template<uint32_t S, uint32_t N>
    class DescriptorUpdateOperation final
    {
    private:
        static constexpr char MSG_INDEX[] = "[vka::DescriptorUpdater::write]: Too many writes.";

        const DescriptorSetArray<S>* m_sets;
        VkWriteDescriptorSet m_writes[N];
        uint32_t m_idx;

    public:
        /**
         * @brief Initializes a descriptor-update operation with a descriptor-set array.
         * @param sets Specifies the descriptor-set array on which the operation is performed.
         */
        explicit DescriptorUpdateOperation(const DescriptorSetArray<S>& sets) noexcept :
            m_sets(&sets),
            m_writes{},
            m_idx(0)
        {}

        // There is nothing to destroy.
        ~DescriptorUpdateOperation() = default;

        /**
         * @brief Creates a descriptor write for buffers.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param infos Specifies the update information for the descriptors. This is an array of info-structures, and
         * its length must correspond to the number of affected descriptors. More formally, the array-length must be
         * equal to the value of 'count'.
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
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param infos Specifies the update information for the descriptors. This is an array of info-structures, and
         * its length must correspond to the number of affected descriptors. More formally, the array-length must be
         * equal to the value of 'count'.
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
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param views Specifies the update information for the descriptors. This is an array of buffer views, and its
         * length must correspond to the number of affected descriptors. More formally, the array-length must be equal
         * to the value of 'count'.
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
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param as_write Specifies the update information for the descriptors. This is the extension structure for a
         * descriptor write for nvidia acceleration structures.
         * @throw std::out_of_range If the maximum number of descriptor-writes has been exceeded.
         * @note The number of affected descriptors (also referred to the parameter 'count') is already specified by the
         * extension structure. Additionally, the used descriptor type is VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV
         * because it is the only type that can be used with this extension structure.
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
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param as_write Specifies the update information for the descriptors. This is the extension structure for a
         * descriptor write for khronos acceleration structures.
         * @throw std::out_of_range If the maximum number of descriptor-writes has been exceeded.
         * @note The number of affected descriptors (also referred to the parameter 'count') is already specified by the
         * extension structure. Additionally, the used descriptor type is VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR
         * because it is the only type that can be used with this extension structure.
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
         * @param iub_write Specifies the write-data for the uniform block. This is the extension structure for a
         * descriptor write for an inline uniform block.
         * @throw std::out_of_range If the maximum number of descriptor-writes has been exceeded.
         * @note The data to write as well as the size of the data to write is already specified by the extension
         * structure. However, this function specifies the offset in the data buffer.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write)
        {
            detail::error::check_idx<N>(this->m_idx, MSG_INDEX);
            this->m_writes[this->m_idx++] = detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, iub_write);
        }

        /// @brief Executes the descriptor-update operation.
        void execute() const
        {
            vkUpdateDescriptorSets(this->m_sets->device(), this->m_idx, this->m_writes, 0, nullptr);
        }
    };

    /**
     * @brief Helper class for a descriptor-set update operation.
     * @tparam S Specifies the number of descriptor sets. The initialization requires a descriptor-set array which
     * requires the number of descriptor sets.
     * @note This class has no default constructor!
     * @details This template specialization uses a dynamic number of descriptor-writes.
     */
    template<uint32_t S>
    class DescriptorUpdateOperation<S, 0> final
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

        // There is nothing to destroy.
        ~DescriptorUpdateOperation() = default;

        /**
         * @brief Creates a descriptor write for buffers.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param infos Specifies the update information for the descriptors. This is an array of info-structures, and
         * its length must correspond to the number of affected descriptors. More formally, the array-length must be
         * equal to the value of 'count'.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, count, type, infos));
        }

        /**
         * @brief Creates a descriptor write for images.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param infos Specifies the update information for the descriptors. This is an array of info-structures, and
         * its length must correspond to the number of affected descriptors. More formally, the array-length must be
         * equal to the value of 'count'.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, count, type, infos));
        }

        /**
         * @brief Creates a descriptor write for (texel-) buffer views.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param count Specifies the number of affected descriptors within the binding.
         * @param type Specifies the descriptor type.
         * @param views Specifies the update information for the descriptors. This is an array of buffer views, and its
         * length must correspond to the number of affected descriptors. More formally, the array-length must be equal
         * to the value of 'count'.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, count, type, views));
        }

        /**
         * @brief Creates a descriptor write for nvidia acceleration structures.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param as_write Specifies the update information for the descriptors. This is the extension structure for a
         * descriptor write for nvidia acceleration structures.
         * @note The number of affected descriptors (also referred to the parameter 'count') is already specified by the
         * extension structure. Additionally, the used descriptor type is VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV
         * because it is the only type that can be used with this extension structure.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as_write)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, as_write));
        }

        /**
         * @brief Creates a descriptor write for khronos acceleration structures.
         * @param set Specifies the index of the descriptor-set within the used descriptor-set array.
         * @param binding Specifies the binding index within the descriptor-set.
         * @param offset Specifies the first affected descriptor if the target binding contains an array of descriptors.
         * @param as_write Specifies the update information for the descriptors. This is the extension structure for a
         * descriptor write for khronos acceleration structures.
         * @note The number of affected descriptors (also referred to the parameter 'count') is already specified by the
         * extension structure. Additionally, the used descriptor type is VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR
         * because it is the only type that can be used with this extension structure.
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
         * @param iub_write Specifies the write-data for the uniform block. This is the extension structure for a
         * descriptor write for an inline uniform block.
         * @note The data to write as well as the size of the data to write is already specified by the extension
         * structure. However, this function specifies the offset in the data buffer.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write)
        {
            this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->handle(set), binding, offset, iub_write));
        }

        /// @brief Executes the descriptor-update operation.
        void execute() const
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
         * @details Internally, the image layout VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL is used.
         */
        inline VkDescriptorImageInfo make_image_info(const Texture& texture, uint32_t view_index);

        /**
         * @brief Creates the extension structure for a descriptor-write for nvidia acceleration structures.
         * @param as Specifies an array of nvidia acceleration structures.
         * @param count Specifies the number of nvidia acceleration structures (number of acceleration structures in the
         * specified array).
         * @return Returns the extension structure for a descriptor-write for nvidia acceleration structures.
         */
        inline VkWriteDescriptorSetAccelerationStructureNV make_write_asnv(const VkAccelerationStructureNV* as, uint32_t count) noexcept;

        /**
         * @brief Creates the extension structure for a descriptor-write for khronos acceleration structures.
         * @param as Specifies an array of khronos acceleration structures.
         * @param count Specifies the number of khronos acceleration structures (number of acceleration structures in
         * the specified array).
         * @return Returns the extension structure for a descriptor-write for khronos acceleration structures.
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

#include "descriptor_inline.inl"
#include "descriptor_set_binding_list.inl"
#include "descriptor_layout_array.inl"
#include "descriptor_set_array.inl"
