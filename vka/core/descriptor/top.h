/**
 * @brief Helper classes and functions for creating and updating descriptors.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
    class DescriptorLayouts;
    class DescriptorSets;
    class DescriptorUpdateOP;

    /**
     * Helper class to define descriptor sets and bindings.
     *
     * <b>Default initialization:</b>\n
     * Creates an empty list.
     *
     * <b>Copy behaviour:</b>\n
     * Trivially copyable.
     *
     * <b>Moving behaviour:</b>\n
     * When calling the move constructor or operator, the moved object is invalidated and performing any operation on it
     * is unsafe. This may lead to undefined behaviour or even a crash. If an already valid object is replaced by a
     * move, the current object is destroyed.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>crating layouts</b> -- Invoked by <c>create_layouts()</c> creates the descriptor set layouts from the list.
     */
    class DescriptorBindingList final
    {
    public:
        /// Initializes the binding list.
        constexpr DescriptorBindingList();

        /**
         * Adds a binding to the current descriptor set. With every call to this function the binding index for the
         * current set is incremented by <c>1</c> starting at <c>0</c>.
         * @param type Descriptor type.
         * @param stages Shader stages where the current binding is used.
         * @param count Number of descriptors referenced by this binding.
         * @param immutable_samplers Array of vulkan sampler handles which is optional and <c>nullptr</c> by default.
         */
        void push(VkDescriptorType type, VkShaderStageFlags stages, uint32_t count = 1, const VkSampler* immutable_samplers = nullptr);

        /// Increments the descriptor set index by <c>1</c> starting at <c>0</c>.
        void next_set();

        /// @return Returns the number of descriptor sets.
        constexpr uint32_t count() const noexcept;

        /**
         * No range check is performed on the index.
         * @param set Descriptor set index.
         * @return Returns the number of bindings of the specified descriptor set.
         * @pre <c>set</c> is a valid index in the range <c>[0, count()-1]</c>.
         */
        constexpr uint32_t binding_count(uint32_t set) const noexcept;

        /**
         * No range check is performed on the index.
         * @param set Descriptor set index.
         * @return Returns the bindings of the specified descriptor set.
         * @pre <c>set</c> is a valid index in the range <c>[0, count()-1]</c>.
         */
        constexpr const VkDescriptorSetLayoutBinding* bindings(uint32_t set) const noexcept;

        /**
         * Creates descriptor layouts from the binding list.
         * @param device Device with which the descriptor layouts are created.
         * @param flags Optional create flags for all descriptor layouts.
         * @return Returns the descriptor layouts created from the binding list.
         */
        inline DescriptorLayouts create_layouts(VkDevice device, VkDescriptorSetLayoutCreateFlags flags = 0) const;

        // default:
        ~DescriptorBindingList() = default;

    private:
        std::vector<std::vector<VkDescriptorSetLayoutBinding>> m_bindings;
    };

    /**
     * Abstraction to simplify the creation of descriptor layouts. Contains an array of vulkan
     * <c>VkDescriptorSetLayout</c> handles.
     *
     * <b>Default initialization:</b>\n
     * The default constructor creates <b>empty</b> descriptor layouts. This empty object is invalid and cannot perform
     * any actions (see below for a brief list of actions). Any member function or operator returning a vulkan handle
     * returns <c>VK_NULL_HANDLE</c> or <c>nullptr</c>. Calling <c>count()</c> returns <c>0</c>. Calling
     * <c>destroy()</c> does nothing.
     *
     * <b>Initialization:</b>\n
     * The initialization constructor creates valid descriptor layouts that can perform any action, if no exception was
     * thrown.
     *
     * <b>Copy behaviour:</b>\n
     * The copy constructor and operator are deleted.
     *
     * <b>Moving behaviour:</b>\n
     * When calling the move constructor or operator, the moved object is invalidated and performing any operation on it
     * is unsafe. This may lead to undefined behaviour or even a crash. If an already valid object is replaced by a
     * move, the current object is destroyed.
     *
     * <b>Destroy behaviour:</b>\n
     * Destroys all vulkan handles and sets everything back to default values. After destroying the object contains
     * <b>empty</b> descriptor layouts.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>creating sets</b> -- Invoked by <c>create_sets()</c> creates the descriptor set from the layouts.
     */
    class DescriptorLayouts final
    {
    public:
        /**
         * Creates descriptor layouts from a binding list. The descriptor layouts are valid if no exception was thrown.
         * @param device Device with which the descriptor layouts are created.
         * @param bindings Binding list from which to create descriptor layouts.
         * @param flags Optional create flags for all descriptor layouts.
         */
        explicit DescriptorLayouts(VkDevice device, const DescriptorBindingList& bindings, VkDescriptorSetLayoutCreateFlags flags = 0);

        /**
         * No range check is performed.
         * @return Returns the vulkan <c>VkDescriptorSetLayout</c> handle at the specified index.
         */
        inline VkDescriptorSetLayout operator[] (uint32_t idx) const noexcept;

        /// @return Returns whether the descriptor layouts are valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the number of descriptor layouts.
        constexpr uint32_t count() const noexcept;

        /// @return Returns the parent handle.
        constexpr VkDevice parent() const noexcept;

        /// @return Returns the vulkan <c>VkDescriptorSetLayout</c> handles.
        constexpr const VkDescriptorSetLayout* handles() const noexcept;

        /// Destroys the descriptor layouts. After destroying the descriptor layouts are empty and therefore invalid.
        constexpr void destroy() noexcept;

        /**
         * Creates descriptor sets from the descriptor layouts.
         * @param pool Pool from which the descriptor-sets are allocated.
         * @return Returns the descriptor sets created from the layouts.
         */
        inline DescriptorSets create_sets(VkDescriptorPool pool) const;

        // default:
        DescriptorLayouts() = default;
        DescriptorLayouts(DescriptorLayouts&&) = default;
        ~DescriptorLayouts() = default;
        DescriptorLayouts& operator=(DescriptorLayouts&&) = default;

    private:
        static constexpr const char* MSG_CREATE_FAILED = "[vka::DescriptorLayouts]: Failed to create descriptor set layout.";

        unique_handle<VkDescriptorSetLayout[]> m_layouts;

        /// Creates the descriptor set layouts.
        static unique_handle<VkDescriptorSetLayout[]> create_layouts(VkDevice device, const DescriptorBindingList& bindings, VkDescriptorSetLayoutCreateFlags flags);
    };

    /**
     * Abstraction to simplify the creation of descriptor sets. Contains an array of vulkan <c>VkDescriptorSet</c>
     * handles.
     *
     * <b>Default initialization:</b>\n
     * The default constructor creates <b>empty</b> descriptor sets. This empty object is invalid and cannot perform any
     * actions (see below for a brief list of actions). Any member function or operator returning a vulkan handle
     * returns <c>VK_NULL_HANDLE</c> or <c>nullptr</c>. Calling <c>count()</c> returns <c>0</c>. Calling
     * <c>destroy()</c> does nothing.
     *
     * <b>Initialization:</b>\n
     * The initialization constructor creates valid descriptor sets that can perform any action, if no exception was
     * thrown.
     *
     * <b>Copy behaviour:</b>\n
     * The copy constructor and operator are deleted.
     *
     * <b>Moving behaviour:</b>\n
     * When calling the move constructor or operator, the moved object is invalidated and performing any operation on it
     * is unsafe. This may lead to undefined behaviour or even a crash. If an already valid object is replaced by a
     * move, the current object is destroyed.
     *
     * <b>Destroy behaviour:</b>\n
     * Destroys all vulkan handles and sets everything back to default values. After destroying the object contains
     * <b>empty</b> descriptor sets.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>binding</b> -- Invoked by <c>bind()</c> binds the descriptor sets to a pipeline.
     * - <b>updating</b> -- Invoked by <c>update_op()</c> creates an operation which is used to update the descriptor
     * sets.
     */
    class DescriptorSets final
    {
        using Parent = detail::descriptor::Parent;
        using Handle = detail::descriptor::Handle;

    public:
        /// Required flags for the descriptor pool.
        static constexpr VkDescriptorPoolCreateFlags POOL_FLAGS = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        /**
         * For each descriptor layout in <c>layouts</c> one descriptor set is created. The descriptor sets are valid if
         * no exception was thrown.
         * @param pool Pool from which the descriptor sets are allocated.
         * @param layouts Layouts from which the descriptor sets are created.
         */
        explicit DescriptorSets(VkDescriptorPool pool, const DescriptorLayouts& layouts);

        /**
         * No range check is performed.
         * @return Returns the vulkan <c>VkDescriptorSet</c> handle at the specified index.
         */
        inline VkDescriptorSet operator[] (uint32_t idx) const noexcept;

        /// @return Returns whether the descriptor sets are valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the number of descriptor sets.
        constexpr uint32_t count() const noexcept;

        /// @return Returns the parent handle.
        constexpr VkDevice parent() const noexcept;

        /// @return Returns the pool from which the descriptor sets are allocated.
        constexpr VkDescriptorPool pool() const noexcept;

        /// @return Returns the vulkan <c>VkDescriptorSet</c> handles.
        constexpr const VkDescriptorSet* handles() const noexcept;

        /// Destroys the descriptor sets. After destroying the descriptor sets are empty and therefore invalid.
        constexpr void destroy() noexcept;

        /**
         * Binds descriptor sets to a pipeline layout.
         * @param cbo Command buffer in which the bind command is recorded.
         * @param bind_point Pipeline bind point.
         * @param pipeline_layout Pipeline layout to which the descriptor sets are bound.
         */
        inline void bind(VkCommandBuffer cbo, VkPipelineBindPoint bind_point, VkPipelineLayout pipeline_layout) const noexcept;

        /// @return Returns an update operation for the descriptor sets.
        constexpr DescriptorUpdateOP update_op() const noexcept;

        // default:
        DescriptorSets() = default;
        DescriptorSets(DescriptorSets&&) = default;
        ~DescriptorSets() = default;
        DescriptorSets& operator= (DescriptorSets&&) = default;

    private:
        static constexpr const char* MSG_CREATE_FAILED = "[vka::DescriptorManager]: Failed to allocate descriptor sets.";

        unique_handle<Handle> m_sets;

        /// Creates the descriptor sets.
        static unique_handle<Handle> create_sets(VkDescriptorPool pool, const DescriptorLayouts& layouts);
    };

    /**
     * Operation object used to update descriptor sets.
     *
     * <b>Default initialization:</b>\n
     * Has no default initialization.
     *
     * <b>Initialization:</b>\n
     * Creates an update operation object.
     *
     * <b>Copy behaviour:</b>\n
     * Trivially copyable.
     *
     * <b>Moving behaviour:</b>\n
     * When calling the move constructor or operator, the moved object is invalidated and performing any operation on it
     * is unsafe. This may lead to undefined behaviour or even a crash. If an already valid object is replaced by a
     * move, the current object is destroyed.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>write</b> -- Invoked by <c>write()</c> adds a descriptor write.
     * - <b>update</b> -- Invoked by <c>execute()</c> updates the descriptor sets.
     */
    class DescriptorUpdateOP final
    {
    public:
        /**
         * Initializes an update operation for descriptor sets.
         * @param sets Descriptor sets to update.
         */
        explicit constexpr DescriptorUpdateOP(const DescriptorSets& sets) noexcept;

        /**
         * Creates a descriptor write for buffers. Range of affected descriptors:\n
         * <c>[offset, offset + count - 1]</c>.
         * @param set Index of the descriptor set to update.
         * @param binding Index of the binding to update.
         * @param offset Descriptor offset.
         * @param count Number of affected descriptors.
         * @param type Descriptor type.
         * @param infos Array of buffer infos. Must contain at least <c>count</c> elements.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos);

        /**
         * Creates a descriptor write for images. Range of affected descriptors:\n
         * <c>[offset, offset + count - 1]</c>.
         * @param set Index of the descriptor set to update.
         * @param binding Index of the binding to update.
         * @param offset Descriptor offset.
         * @param count Number of affected descriptors.
         * @param type Descriptor type.
         * @param infos Array of image infos. Must contain at least <c>count</c> elements.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos);

        /**
         * Creates a descriptor write for buffer views. Range of affected descriptors:\n
         * <c>[offset, offset + count - 1]</c>.
         * @param set Index of the descriptor set to update.
         * @param binding Index of the binding to update.
         * @param offset Descriptor offset.
         * @param count Number of affected descriptors.
         * @param type Descriptor type.
         * @param views Array of buffer views. Must contain at least <c>count</c> elements.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views);

        /**
         * Creates a descriptor write for a <c>NV</c> acceleration structure.
         * @param set Index of the descriptor set to update.
         * @param binding Index of the binding to update.
         * @param offset Descriptor offset.
         * @param as <c>NV</c> acceleration structure info.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as);

        /**
         * Creates a descriptor write for a <c>KHR</c> acceleration structure.
         * @param set Index of the descriptor set to update.
         * @param binding Index of the binding to update.
         * @param offset Descriptor offset.
         * @param as <c>KHR</c> acceleration structure info.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as);

        /**
         * Creates a descriptor write for an inline uniform block.
         * @param set Index of the descriptor set to update.
         * @param binding Index of the binding to update.
         * @param offset Descriptor offset.
         * @param block Inline uniform block info.
         */
        void write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& block);

        /// Executes the descriptor update.
        inline void execute() const noexcept;

        // default:
        ~DescriptorUpdateOP() = default;

    private:
        const DescriptorSets* m_sets;
        std::vector<VkWriteDescriptorSet> m_writes;
    };

    namespace descriptor
    {
        /**
         * Creates a vulkan <c>VkDescriptorBufferInfo</c> structure.
         * @param buffer Buffer to bind.
         * @param offset Optional offset in bytes.
         * @param range Optional size in bytes. If left to <c>VK_WHOLE_SIZE</c>, the whole buffer memory will be bound
         * to the descriptor starting at <c>offset</c>.
         * @return Returns the corresponding vulkan <c>VkDescriptorBufferInfo</c> structure.
         */
        constexpr VkDescriptorBufferInfo make_buffer_info(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;

        /**
         * Creates a vulkan <c>VkDescriptorBufferInfo</c> structure from a <c>vka::Buffer</c>.
         * @param buffer Buffer to bind.
         * @param offset Optional offset in bytes.
         * @param range Optional size in bytes. If left to <c>VK_WHOLE_SIZE</c>, the whole buffer memory will be bound
         * to the descriptor starting at <c>offset</c>.
         * @return Returns the corresponding vulkan <c>VkDescriptorBufferInfo</c> structure.
         */
        constexpr VkDescriptorBufferInfo make_buffer_info(const Buffer& buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;

        /**
         * Creates a vulkan <c>VkDescriptorImageInfo</c> structure.
         * @param view Image view to bind.
         * @param layout Layout of the image region referenced by <c>view</c>.
         * @param sampler Optionally specifies a sampler which is used together with the image view.
         * @return Returns the corresponding vulkan <c>VkDescriptorImageInfo</c> structure.
         */
        constexpr VkDescriptorImageInfo make_image_info(VkImageView view, VkImageLayout layout, VkSampler sampler = VK_NULL_HANDLE) noexcept;

        /**
         * Creates a vulkan <c>VkDescriptorImageInfo</c> structure from a <c>vka::Texture</c>. Does not perform a range
         * check on the view index.
         * @param texture Texture from which one view will be bound.
         * @param view_index View index of the texture to bind.
         * @return Returns the corresponding vulkan <c>VkDescriptorImageInfo</c> structure.
         */
        constexpr VkDescriptorImageInfo make_image_info(const Texture& texture, uint32_t view_index) noexcept;

        /**
         * Creates a vulkan <c>VkWriteDescriptorSetAccelerationStructureNV</c> structure from a <c>NV</c> acceleration
         * structure.
         * @param as <c>NV</c> acceleration structures to bind.
         * @param count Number of acceleration structures to bind.
         * @return Returns the corresponding vulkan <c>VkWriteDescriptorSetAccelerationStructureNV</c> structure.
         * @note <c>count</c> descriptors are affected by this write info.
         */
        constexpr VkWriteDescriptorSetAccelerationStructureNV make_write_asnv(const VkAccelerationStructureNV* as, uint32_t count) noexcept;

        /**
         * Creates a vulkan <c>VkWriteDescriptorSetAccelerationStructureKHR</c> structure from a <c>KHR</c> acceleration
         * structure.
         * @param as <c>KHR</c> acceleration structures to bind.
         * @param count Number of acceleration structures to bind.
         * @return Returns the corresponding vulkan <c>VkWriteDescriptorSetAccelerationStructureKHR</c> structure.
         * @note <c>count</c> descriptors are affected by this write info.
         */
        constexpr VkWriteDescriptorSetAccelerationStructureKHR make_write_askhr(const VkAccelerationStructureKHR* as, uint32_t count) noexcept;

        /**
         * Creates a vulkan <c>VkWriteDescriptorSetInlineUniformBlock</c> structure.
         * @param data Data of the inline uniform block to bind.
         * @param size Size in bytes of the data.
         * @return Returns the corresponding vulkan <c>VkWriteDescriptorSetInlineUniformBlock</c> structure.
         */
        constexpr VkWriteDescriptorSetInlineUniformBlock make_inline_uniform_block(const void* data, uint32_t size) noexcept;
    }
}
