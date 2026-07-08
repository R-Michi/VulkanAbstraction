/**
 * @brief Helper classes for creating push constants.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
    template<uint32_t N>
    class PushConstants;

    /**
     * Defines a view to a push constant range. It contains a pointer to the memory as well as a
     * <c>VkPushConstantRange</c> defining the offset, size and shader stage flags of the push constant range.
     * - <b>shader stage flags</b> -- Specifies the shader stages in which the push constant range is active. The shader
     * stages are returned by <c>stages()</c>.
     * - <b>offset</b> --  Offset in bytes of the range relative to the whole push constant memory. The offset is
     * returned by <c>offset()</c>.
     * - <b>size</b> -- Size in bytes of the push constant range. The size is returned by <c>size()</c>.
     *
     * When writing data to a push constant range via <c>write()</c> the <c>offset</c> parameter specifies the memory
     * offset of this <b>push constant range</b>.
     *
     * <b>Default initialization:</b>\n
     * No default initialization.
     *
     * <b>Initialization:</b>\n
     * Is initialized with a <c>VkPushConstantRange</c> and a pointer pointing the memory of this push constant range.
     *
     * <b>Copy behaviour:</b>\n
     * The view acts like a trivial type and is therefore trivially copyable.
     *
     * <b>Moving behaviour:</b>\n
     * As this is a trivial type moving is equivalent to copying.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>writing</b> -- Invoked by <c>write()</c> writes data to the push constant range.
     * - <b>pushing</b> -- Invoked by <c>push()</c> pushes the range to the command buffer.
     */
    class PushConstantView final
    {
    public:
        /**
         * Initializes a view.
         * @param range Push constant range.
         * @param data Associated memory.
         */
        constexpr PushConstantView(VkPushConstantRange range, void* data) noexcept;

        /// @return Returns the shader stages of the push constant range.
        constexpr VkShaderStageFlags stages() const noexcept;

        /// @return Returns the size in bytes of the push constant range.
        constexpr uint32_t size() const noexcept;

        /// @return Returns the offset in bytes of the push constant range.
        constexpr uint32_t offset() const noexcept;

        /**
         * Grants direct access to the memory of the push constant range.
         * @return Returns the raw pointer to the memory of the push constant range.
         */
        constexpr void* data() noexcept;
        constexpr const void* data() const noexcept;

        /**
         * Writes data to the push constant range.
         * @param offset Offset in bytes relative to the push constant range.
         * @param size Number of bytes to write.
         * @param data Data to write. Must point to memory which is at least <c>size</c> bytes large.
         */
        inline void write(uint32_t offset, uint32_t size, const void* data);

        /**
         * Pushes the range to the command buffer.
         * @param cbo Command buffer in which the push command is recorded.
         * @param layout Pipeline layout used for the push command.
         */
        inline void push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept;

        // default:
        ~PushConstantView() = default; // nothing to destroy

    private:
        static constexpr char MSG_INVALID_RANGE[] = "[vka::PushConstantView]: Attempt to write memory outside the push constant range";

        VkPushConstantRange m_range;
        void* m_data;
    };

    /**
     * Defines the layout and enables compile-time checking for push constant ranges. After creating the push constants
     * from the layout, this object is no longer needed.
     *
     * <b>Default initialization:</b>\n
     * Initializes a push constant layout with a size limit of <c>128</c> bytes. <c>128</c> bytes are guaranteed by the
     * Vulkan API.
     *
     * <b>Initialization:</b>\n
     * Initializes the push constant layout with a custom size. It is completely fine, if your push constants are larger
     * than <c>128</c> bytes. However, you need to check how many bytes are supported on your platform.
     *
     * <b>Copy behaviour:</b>\n
     * The view acts like a trivial type and is therefore trivially copyable.
     *
     * <b>Moving behaviour:</b>\n
     * As this is a trivial type moving is equivalent to copying.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>adding</b> -- Adds a push constant range to the layout. If you perform this action in a constexpr context
     * like inside a <c>consteval</c> function, instead of throwing an exception you will receive a compiler error.
     * - <b>validating</b> -- Checks if all push constant ranges are used. If you perform this action in a constexpr
     * context like inside a <c>consteval</c> function, instead of throwing an exception you will receive a compiler
     * error.
     * - <b>creating push constants</b> -- Creates the push constants from the layout.
     *
     * @tparam N Number of push constant ranges.
     */
    template<uint32_t N>
    class PushConstantLayout final
    {
    public:
        /**
         * Initializes the layout with a default limit of <c>128</c> bytes. <c>128</c> bytes are guaranteed by the
         * vulkan API.
         */
        constexpr PushConstantLayout() noexcept;

        /**
         * Initializes the layout with a custom limit.
         * @param size Size in bytes of the push constant buffer. You can also allocate more than 128 bytes. The actual
         * limit can be found in the physical device limits.
         */
        explicit constexpr PushConstantLayout(uint32_t size) noexcept;

        /// @return Returns the number of push constant ranges.
        static constexpr uint32_t count() noexcept;

        /**
         * This is the size of memory in bytes a PushConstant object will allocate.
         * @return Returns the total size of all ranges combined.
         */
        constexpr uint32_t size() const noexcept;

        /// @return Returns the push constant ranges.
        constexpr const VkPushConstantRange* ranges() const noexcept;

        /**
         * Adds a push constant range to the layout. If this function is used in a constexpr context like inside a
         * <c>consteval</c> function, instead of throwing an exception you will receive a compiler error.
         * @param size Size in bytes of the push constant range.
         * @param stages Shader stages where the push constant range is used.
         * @throw std::out_of_range If the number of added ranges exceeds the maximum number of push constant ranges
         * defined by the template argument <c>N</c>.
         * @throw std::runtime_error If the total size of all push constant ranges combined, including the new added
         * one, exceeds the maximum size of the push constant buffer.
         */
        constexpr void add(uint32_t size, VkShaderStageFlags stages);

        /**
         * Checks if all push constant ranges have been used. Unused push constant ranges will have a size of <c>0</c>
         * and may cause issues when creating a pipeline. If this function is used in a constexpr context like inside a
         * <c>consteval</c> function, instead of throwing an exception you will receive a compiler error.
         * @throw std::runtime_error If any push constant range is unused.
         */
        constexpr void validate() const;

        /// Creates push constants from the layout.
        constexpr PushConstants<N> create_push_constants();

        // Default:
        ~PushConstantLayout() = default; // nothing to destroy

    private:
        static constexpr uint32_t MIN_SIZE = 128;   // minimum push constant size guaranteed by the vulkan API
        static constexpr char MSG_RANGE[] = "[vka::PushConstantLayout]: Exceeded maximum number of push constant ranges.";
        static constexpr char MSG_SIZE[] = "[vka::PushConstantLayout]: Limit for push constant size exceeded.";
        static constexpr char MSG_UNUSED[] = "[vka::PushConstantLayout]: Some push constant ranges are unused.";

        std::array<VkPushConstantRange, N> m_ranges;
        uint32_t m_total_size;
        uint32_t m_max_size;
        uint32_t m_idx;

        /**
         * Rounds the size to the next multiple of <c>4</c>. The vulkan specification states that every push constant
         * size must be a multiple of <c>4</c>. More formally, let <c>truncate(x)</c> be the function that removes the
         * fraction part of a number <c>x</c>. The result <c>r</c> equals to:\n
         * <c>r = truncate((s - 1) / 4) * 4 + 4</c> or <c>r = 0</c> if <c>s</c> is equal to <c>0</c>.
         * @param s Specifies the size to round.
         * @return Returns the rounded size.
         */
        static constexpr uint32_t round_size(uint32_t s) noexcept;
    };

    /**
     * Contains a CPU-side buffer containing the push constants.
     *
     * <b>Default initialization:</b>\n
     * Empty buffer. Does not allocate any memory.
     *
     * <b>Initialization:</b>\n
     * Allocates memory for a specified push constant layout.
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
     * If you want to destroy the buffer / deallocate its memory, move an empty object into it.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>pushing</b> -- Invoked by <c>push()</c> pushes all ranges to the command buffer.
     *
     * @tparam N Specifies the number of push constant ranges.
     */
    template<uint32_t N>
    class PushConstants final
    {
    public:
        /// Default initialization. Does not allocate any memory.
        constexpr PushConstants() noexcept;

        /**
         * Allocates memory for the push constant buffer.
         * @param layout Layout with which the push constants are allocated.
         * @throw std::bad_alloc If memory allocation failed.
         */
        explicit constexpr PushConstants(const PushConstantLayout<N>& layout);

        /// @return Returns <c>true</c> if the object is valid.
        explicit constexpr operator bool() const noexcept;

        /**
         * No range check is performed on the index. Always <c>read/write</c> access.
         * @param idx Index of the push constant range.
         * @return Returns a view to the push constant range at <c>idx</c>.
         */
        constexpr PushConstantView operator[] (uint32_t idx) noexcept;

        /**
         * A range check on the index is performed. Always <c>read/write</c> access.
         * @param idx Index of the push constant range.
         * @return Returns a view to the push constant range at <c>idx</c>.
         * @throw std::out_of_range If the index is greater than the maximum number of ranges defined by the template
         * parameter <c>N</c>.
         * @throw std::runtime_error If the <c>PushConstants</c> object is invalid.
         */
        constexpr PushConstantView at(uint32_t idx);

        /// @return Returns the number of push constant ranges.
        static constexpr uint32_t count() noexcept;

        /// Returns the allocated size in bytes which is the total size of all ranges combined.
        constexpr uint32_t size() const noexcept;

        /// @returns Returns the push constant ranges. Contains <c>count()</c> elements.
        constexpr const VkPushConstantRange* ranges() const noexcept;

        /**
         * Pushes all push constant ranges to the GPU.
         * @param cbo Command buffer where the push-commands are recorded into.
         * @param layout Pipeline layout which uses the push constants.
         */
        constexpr void push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept;

        // Default:
        PushConstants(PushConstants&&) = default;
        ~PushConstants() = default;
        PushConstants& operator= (PushConstants&&) = default;

        // Deleted:
        PushConstants(const PushConstants&) = delete;
        PushConstants& operator= (const PushConstants&) = delete;

    private:
        static constexpr char MSG_ACCESS[] = "[vka::PushConstants]: Accessing a push constant range from an invalid PushConstant object is forbidden.";

        std::array<VkPushConstantRange, N> m_ranges;
        detail::PushConstantBuffer m_buff;
    };
}
