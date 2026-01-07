/**
 * @brief Helper classes for creating push constants.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

namespace vka
{
    // forward declaration
    template<uint32_t N>
    class PushConstants;

    class PushConstantView final
    {
    public:
        /**
         * Initializes the push constant range view.
         * @param range Specifies the range the view is initialized with.
         * @param data Specifies the push constant range memory that this view references.
         */
        inline PushConstantView(VkPushConstantRange range, void* data) noexcept;

        /// @return Returns the shader stages of this push constant range.
        inline VkShaderStageFlags stages() const noexcept;

        /// @return Returns the size in bytes of this push constant range.
        inline uint32_t size() const noexcept;

        /// @return Returns the offset of this range in push constant memory.
        inline uint32_t offset() const noexcept;

        /**
         * Grants direct access to this push constant range memory.
         * @return Returns the raw pointer to the push constant range memory.
         */
        inline void* data() noexcept;
        inline const void* data() const noexcept;

        /**
         * Writes sets to a push constant range.
         * @param offset Specifies the byte offset in the push constant range memory.
         * @param size Specifies the number of bytes that should be written to the push constant range.
         * @param data Specifies the data that should be written to the push constant range. The sets must be at least
         * 'size' bytes in length.
         * @details Let 'b' be the beginning address of this push constant range. Data is written in the following
         * interval: [b + offset, b + size - 1]. More informally, the first byte gets written at the position which is
         * specified by 'offset' and the last byte gets written at the position 'size - 1', assuming the first byte of
         * this push constant range has position 0.
         */
        inline void write(uint32_t offset, uint32_t size, const void* data);

        /**
         * Pushes the range to the GPU.
         * @param cbo Specifies the command buffer where the push command is recorded into.
         * @param layout Specifies the pipeline layout used for the push command.
         */
        inline void push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept;

        // default:
        ~PushConstantView() = default; // nothing to destroy

    private:
        static constexpr char MSG_INVALID_RANGE[] = "[vka::PushConstantView::write]: Attempt to write memory outside the push constant range";

        VkPushConstantRange m_range;
        void* m_data;
    };

    /**
     * Defines the layout and enables compile-time checking for the push constant ranges. After creating the push
     * constants, this object is no longer needed.
     * @tparam N Specifies the number of push constant ranges.
     */
    template<uint32_t N>
    class PushConstantLayout final
    {
    public:
        /// Initializes the layout with a default limit of 128 bytes. 128 bytes are guaranteed by the vulkan API.
        constexpr PushConstantLayout() noexcept;

        /**
         * Initializes the layout with a custom limit.
         * @param size Specifies the size of the push constant buffer. You can allocate more than 128 bytes. If you want
         * to allocate more, the actual limit can be found in the physical device limits.
         */
        explicit constexpr PushConstantLayout(uint32_t size) noexcept;

        /// @return Returns the number of push constant ranges.
        static constexpr uint32_t count() noexcept;

        /**
         * This is the size of memory a PushConstant object will allocate.
         * @return Returns the total size of all ranges combined.
         */
        constexpr uint32_t size() const noexcept;

        /// @return Returns the push constant ranges.
        constexpr const VkPushConstantRange* ranges() const noexcept;

        /// @returns Returns the std::array of push constant ranges.
        constexpr std::array<VkPushConstantRange, N> ranges_array() const noexcept;

        /**
         * Adds a push constant range to the layout.
         * @param size Specifies the size of the push constant range.
         * @param stages Specifies the shader stages where the push constant range is used.
         * @throw std::out_of_range If the number of added ranges exceeds the maximum number of push constant ranges
         * specifies by the template argument 'N'.
         * @throw std::runtime_error If the total size of all push constant ranges, including the new added one, exceeds
         * the maximum size of the push constant buffer.
         */
        constexpr void add(uint32_t size, VkShaderStageFlags stages);

        /**
         * Checks if all push constant ranges have been used. Unused push constant ranges will have a size of 0 and may
         * cause issues when creating a pipeline.
         * @throw std::runtime_error If any push constant range is unused.
         */
        constexpr void validate() const;

        /// Creates push constants from the layout.
        inline PushConstants<N> create_push_constants();

        // Default:
        ~PushConstantLayout() = default; // nothing to destroy

    private:
        static constexpr uint32_t MIN_SIZE = 128;   // minimum push constant size guaranteed by the vulkan API
        static constexpr char MSG_RANGE[] = "[vka::PushConstantLayout::add]: Exceeded maximum number of push constant ranges.";
        static constexpr char MSG_SIZE[] = "[vka::PushConstantLayout::add]: Limit for push constant size exceeded.";
        static constexpr char MSG_UNUSED[] = "[vka::PushConstantLayout::validate]: Some push constant ranges are unused.";

        std::array<VkPushConstantRange, N> m_ranges;
        uint32_t m_total_size;
        uint32_t m_max_size;
        uint32_t m_idx;

        /**
         * @brief Rounds the size to the next multiple of 4.
         * @param s Specifies the size to round.
         * @return Returns the rounded size.
         * @details The vulkan specification states that every push constant range must be a multiple of 4. More
         * formally, let truncate(x) be the function that removes the fraction part of a  number 'x'. The result 'r'
         * equals to 'r = truncate ((s - 1) / 4) * 4 + 4' or 'r = 0' if 's' is equal to 0.
         */
        static constexpr uint32_t round_size(uint32_t s) noexcept;
    };

    /**
     * Contains a CPU-side buffer for the push constants.
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
         * @param layout Specifies the layout used for the push constants.
         * @throw std::bad_alloc If memory allocation failed.
         */
        explicit inline PushConstants(const PushConstantLayout<N>& layout);

        /// @return Returns true if the object is valid.
        explicit constexpr operator bool() const noexcept;

        /**
         * No range check is performed on the index. Always read/write access.
         * @param idx Specifies the index of the push constant range to return.
         * @return Returns a view to the push constant range at the specified index.
         */
        inline PushConstantView operator[] (uint32_t idx) noexcept;

        /**
         * A range check on the index is performed. Always read/write access.
         * @param idx Specifies the index of the push constant range to return.
         * @return Returns a view to the push constant range at the specified index.
         * @throw std::out_of_range If the index is greater than the maximum number of ranges specified by the template
         * parameter 'N'.
         * @throw std::runtime_error If 'this' is not a valid PushConstants object.
         */
        inline PushConstantView at(uint32_t idx);

        /// @return Returns the number of push constant ranges.
        static constexpr uint32_t count() noexcept;

        /// Returns the allocated size which is the total size of all ranges combined.
        constexpr uint32_t size() const noexcept;

        /// @returns Returns the push constant ranges. Contains count() elements.
        constexpr const VkPushConstantRange* ranges() const noexcept;

        /**
         * Pushes all push constant ranges to the GPU.
         * @param cbo Specifies the command buffer where the push-commands are recorded into.
         * @param layout Specifies the pipeline layout which uses the push constants.
         */
        inline void push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept;

        // Default:
        ~PushConstants() = default; // nothing to destroy
        PushConstants(PushConstants&&) = default;
        PushConstants& operator= (PushConstants&&) = default;

        // Deleted:
        PushConstants(const PushConstants&) = delete;
        PushConstants& operator= (const PushConstants&) = delete;

    private:
        static constexpr char MSG_ACCESS[] = "[vka::PushConstants::at]: Accessing a push constant range from an invalid PushConstant object is forbidden.";

        std::array<VkPushConstantRange, N> m_ranges;
        detail::PushConstantBuffer m_buff;
    };
}
