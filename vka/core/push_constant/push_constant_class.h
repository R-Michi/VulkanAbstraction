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
    private:
        static constexpr char MSG_INVALID_RANGE[] = "[vka::PushConstantView::write]: Attempt to write memory outside the push constant range";

        VkPushConstantRange m_range;
        void* m_data;

    public:
        /**
         * @brief Initializes the push constant range view.
         * @param range Specifies the range the view is initialized with.
         * @param data Specifies the push constant range memory that this view references.
         */
        inline PushConstantView(const VkPushConstantRange& range, void* data) noexcept;

        // Destructor is not used.
        ~PushConstantView() = default;

        /// @return Returns the shader stages of this push constant range.
        inline VkShaderStageFlags stages() const noexcept;

        /// @return Returns the size in bytes of this push constant range.
        inline uint32_t size() const noexcept;

        /// @return Returns the offset of this range in push constant memory.
        inline uint32_t offset() const noexcept;

        /**
         * @brief Grants direct access to this push constant range memory.
         * @return Returns the raw pointer to the push constant range memory.
         */
        inline void* data() noexcept;
        inline const void* data() const noexcept;

        /**
         * @brief Writes sets to a push constant range.
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
         * @brief Pushes the range to the GPU.
         * @param cbo Specifies the command buffer where the push command is recorded into.
         * @param layout Specifies the pipeline layout used for the push command.
         */
        inline void push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept;
    };

    template<uint32_t N>
    class PushConstantLayout final
    {
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

    public:
        /**
         * @bief Initializes the layout with a push constant buffer size. The limit is by default 128 bytes. 128 bytes
         * are guaranteed by the vulkan API. The actual limit of the device can be found in the physical device limits.
         * @param size Specifies the size of the push constant buffer.
         */
        explicit constexpr PushConstantLayout(uint32_t size = MIN_SIZE) noexcept;

        // nothing to destroy
        ~PushConstantLayout() = default;

        /**
         * @brief Initializes the layout with the actual maximum size for a push constant buffer.
         * @param size Specifies the maximum push-constant size queried from the physical device properties.
         * @details This can only be done if no ranges have been added yet. Otherwise, this function does nothing.
         */
        constexpr void init(uint32_t size) noexcept;

        /**
         * @brief Adds a push constant range to the layout.
         * @param size Specifies the size of the push constant range.
         * @param stages Specifies the shader stages where the push constant range is used.
         * @throw std::out_of_range If the number of added ranges exceeds the maximum number of push constant ranges
         * specifies by the template argument 'N'.
         * @throw std::runtime_error If the total size of all push constant ranges, including the new added one, exceeds
         * the maximum size of the push constant buffer.
         * @note The push constant buffer can contain multiple ranges.
         */
        constexpr void add(uint32_t size, VkShaderStageFlags stages);

        /**
         * @brief Creates a PushConstant object with 'this' layout.
         * @return Returns a valid push constant object.
         */
        inline PushConstants<N> create_push_constants();

        /// @return Returns the number of push constant ranges.
        static constexpr uint32_t count() noexcept;

        /**
         * @return Returns the total size of all ranges combined.
         * @note This is the size of memory a PushConstant object will allocate.
         */
        constexpr uint32_t size() const noexcept;

        /// @returns Returns the push constant ranges. The array contains 'count()' elements.
        constexpr const VkPushConstantRange* ranges() const noexcept;

        /**
         * @param idx Specifies the index (position) of the VkPushConstantRange to return.
         * @return Returns the vulkan VkPushConstantRange at the specified index (position).
         * @throw std::out_of_range If the index is greater than the maximum number of ranges specified by the template
         * parameter 'N'.
         * @details This function does perform a range check.
         */
        constexpr VkPushConstantRange at(uint32_t idx) const;

        /**
         * @param idx Specifies the index (position) of the VkPushConstantRange to return.
         * @return Returns the vulkan VkPushConstantRange at the specified index (position).
         * @details The index operator does not perform a range check. Therefore, accessing with an invalid index causes
         * undefined behavior.
         */
        constexpr VkPushConstantRange operator[] (uint32_t idx) const noexcept;

        /**
         * @brief Checks if all 'N' push constant ranges have been used.
         * @throw std::runtime_error If any push constant range is unused.
         * @detail This is an optional function that can be called to check if all push constant ranges have actually
         * been used. Unused push constant ranges will have a size of 0 and do not occupy any push constant buffer
         * memory. If an unused range is detected, the check fails and an exception is thrown.
         */
        constexpr void validate() const;
    };

    template<uint32_t N>
    class PushConstants final
    {
    private:
        static constexpr char MSG_ACCESS[] = "[vka::PushConstants::at]: Accessing a push constant range from an invalid PushConstant object is forbidden.";

        const PushConstantLayout<N>* m_layout;
        detail::PushConstantBuffer m_buff;

    public:
        /// @brief Default initialization. Does not allocate any memory.
        inline PushConstants() noexcept;

        /**
         * @brief Creates the PushConstant object with a specified layout.
         * @param layout Specifies the layout used for the push constants.
         * @throw std::bad_alloc If memory allocation failed.
         * @details This constructor allocates memory for all push constant ranges specified by the layout and has the
         * same functionality as the function 'create()'.
         */
        explicit inline PushConstants(const PushConstantLayout<N>& layout);

        PushConstants(const PushConstants&) = delete;
        PushConstants& operator= (const PushConstants&) = delete;

        /**
         * @brief Moves the ownership of a push-constant to a different object.
         * @param src Specifies the source object to move.
         * @details The source push-constant becomes invalidated. If the move destination has been created before and is
         * a valid object, it gets destroyed.
         */
        inline PushConstants(PushConstants&& src) noexcept;
        inline PushConstants& operator= (PushConstants&& src) noexcept;

        ~PushConstants() = default;

        /**
         * @brief Creates the PushConstant object with a specified layout.
         * @param layout Specifies the layout used for the push constants.
         * @throw std::bad_alloc If memory allocation failed.
         * @details This function allocates memory for all push constant ranges specified by the layout.
         */
        inline void create(const PushConstantLayout<N>& layout);

        /// @brief Deletes the allocated memory.
        inline void destroy() noexcept;

        /**
         * @brief Pushes all push constant ranges to the GPU (command buffer).
         * @param cbo Specifies the command buffer where the push-commands are recorded into.
         * @param layout Specifies the pipeline layout which uses the push constants.
         */
        inline void push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept;

        /**
         * @param idx Specifies the index (position) of the push constant range to return.
         * @return Returns a view on the push constant range at a specified index (position).
         * @throw std::out_of_range If the index is greater than the maximum number of ranges specified
         * by the template parameter 'N'.
         * @throw std::runtime_error If 'this' is not a valid PushConstants object. Access to an uninitialized / invalid
         * object may cause undefined behavior.
         * @details This function does perform a range check.
         */
        inline PushConstantView at(uint32_t idx) const;

        /**
         * @param idx Specifies the index (position) of the push constant range to return.
         * @return Returns a view on the push constant range at a specified index (position).
         * @details The index operator does not perform a range check. Therefore, accessing with an invalid index may
         * cause undefined behavior. Undefined behavior may also because if 'this' is an uninitialized / invalid object.
         */
        inline PushConstantView operator[] (uint32_t idx) const noexcept;

        /// @return Returns 'true' if 'this' is valid and 'false' otherwise.
        inline bool is_valid() const noexcept;
    };
}
