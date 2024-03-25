/**
* @file     vka_push_constant.h
* @brief    Push constant definition file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../detail/push_constant/vka_push_constant.h"

namespace vka
{
    // forward declaration
    template<uint32_t N>
    class PushConstants;

    class PushConstantView
    {
    private:
        static constexpr const char* MSG_INVALID_RANGE = "[vka::PushConstantView::write]: Attempt to write memory outside the push constant range";

        VkPushConstantRange m_range;
        void* m_data;

    public:
        /**
         * @brief Initializes the push constant range view.
         * @param range Specifies the range the view is initializes with.
         * @param data Specifies the push constant range memory that this view references.
         */
        constexpr PushConstantView(const VkPushConstantRange& range, void* data) noexcept;

        // Destructor not used.
        ~PushConstantView(void) = default;

        /// @return Returns the shader stages of this push constant range.
        constexpr VkShaderStageFlags stages(void) const noexcept;

        /// @return Returns the size in bytes of this push constant range.
        constexpr uint32_t size(void) const noexcept;

        /// @return Returns the offset of this range in push constant memory.
        constexpr uint32_t offset(void) const noexcept;

        /**
         * @brief Grants direct access to this push constant range memory.
         * @return Returns the raw pointer to the push constant range memory.
         */
        constexpr void* data(void) noexcept;

        /**
         * @brief Writes sets to a push constant range.
         * @param offset Specifies the byte offset in the push constant range memory.
         * @param size Specifies the number of bytes that should be written to the push constant range.
         * @param data Specifies the data that should be written to the push constant range. The sets
         * must be at least 'size' bytes in length.
         * @details Let 'b' be the begin address of this push constant range. Data is written in the
         * following interval: [b + offset, b + size - 1]. More informally, the first byte gets written
         * at the position which is specified by 'offset' and the last byte gets written at the position
         * 'size - 1', assuming the first byte of this push constant range has position 0.
         */
        inline void write(uint32_t offset, uint32_t size, const void* data);

        /**
         * @brief Pushes the range to the GPU.
         * @param cbo Specifies the command buffer where the push command is recorded into.
         * @param layout Specifies the pipeline layout that is used for the push command.
         */
        inline void push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept;
    };

    template<uint32_t N>
    class PushConstantLayout
    {
    private:
        static constexpr uint32_t MIN_SIZE = 128;   // minimum push constant size guaranteed by the vulkan API
        static constexpr const char* MSG_RANGE = "[vka::PushConstantLayout::add]: Exceeded maximum number of push constant ranges.";
        static constexpr const char* MSG_SIZE = "[vka::PushConstantLayout::add]: Limit for push constant size exceeded.";
        static constexpr const char* MSG_UNUSED = "[vka::PushConstantLayout::validate]: Some push constant ranges are unused.";

        std::array<VkPushConstantRange, N> m_ranges;
        uint32_t m_total_size;
        uint32_t m_max_size;
        uint32_t m_idx;

        /**
         * @brief Rounds the size to the next multiple of 4.
         * @param s Specifies the size to round.
         * @return Returns the rounded size.
         * @details The vulkan specification states that every push constant range must be a multiple
         * of 4. More formally, let truncate(x) be the function that removes the fraction part of a
         * number 'x'. The result 'r' equals to 'r = truncate((s - 1) / 4) * 4 + 4' or 'r = 0', if s
         * is equal to 0.
         */
        constexpr uint32_t round_size(uint32_t s) const noexcept;

    public:

        /**
         * @brief Default initialization.
         * @details The default maximum size of the push constant buffer is 128 bytes. The vulkan
         * specification guarantees a minimum push constant buffer size of 128 bytes.
         */
        constexpr PushConstantLayout(void) noexcept;

        /**
         * @bief Initializes the layout with the actual push constant buffer size limit which is
         * contained by the physical device properties.
         * @param properties Specifies the physical device properties which contains the maximum
         * size for the push constant buffer.
         */
        explicit inline PushConstantLayout(const VkPhysicalDeviceProperties& properties) noexcept;

        // It does not make much sense to copy a push constant layout.
        PushConstantLayout(const PushConstantLayout&) = delete;
        PushConstantLayout& operator= (const PushConstantLayout&) = delete;

        /**
         * @brief Moves another object of PushConstantLayout into 'this'.
         * @param src Specifies the source object to move into 'this'.
         * @details 'this' now holds the ownership of all the push constant ranges of the source
         * object. The source object becomes invalidated and contains its default initialization.
         * If 'this' already contains push constant ranges, the get overwritten by the ranges of
         * the source object.
         */
        constexpr PushConstantLayout(PushConstantLayout&& src) noexcept;
        constexpr PushConstantLayout& operator= (PushConstantLayout&& src) noexcept;

        ~PushConstantLayout(void) = default;

        /**
         * @brief Initializes the layout with the actual maximum size for a push constant buffer.
         * @param properties Specifies the physical device properties which contains the maximum size
         * for the push constant buffer.
         * @details This can only be done, if no ranges have been added yet. Otherwise, this function
         * does nothing.
         */
        inline void init(const VkPhysicalDeviceProperties& properties) noexcept;

        /**
         * @brief Adds a push constant range to the layout.
         * @param size Specifies the size of the push constant range.
         * @param stages Specifies the shader stages where the push constant range is used.
         * @throw std::out_of_range If the number of added ranges exceeds the maximum number of push
         * constant ranges specifies by the template argument 'N'.
         * @throw std::runtime_error If the total size of all push constant ranges, including the new
         * added one, would exceed the maximum size of the push constant buffer.
         * @note The push constant buffer can contain multiple ranges.
         */
        constexpr void add(uint32_t size, VkShaderStageFlags stages);

        /**
         * @brief Creates a PushConstant object with 'this' layout.
         * @return Returns a valid push constant object.
         */
        inline PushConstants<N> create_push_constants(void);

        /// @return Returns the number of push constant ranges.
        constexpr uint32_t count(void) const noexcept;

        /**
         * @return Returns the total size of all ranges combined.
         * @note This is the size of memory a PushConstant object will allocate.
         */
        constexpr uint32_t size(void) const noexcept;

        /// @returns Returns an array of 'count()' elements of vulkan VkPushConstantRange structs.
        constexpr const VkPushConstantRange* data(void) const noexcept;

        /**
         * @param idx Specifies the index (position) of the VkPushConstantRange to return.
         * @return Returns the vulkan VkPushConstantRange at the specifies index (position).
         * @throw std::out_of_range If the index is greater than the maximum number of ranges specified
         * by the template parameter 'N'.
         * @details This function does perform a range check.
         */
        constexpr const VkPushConstantRange& at(uint32_t idx) const;

        /**
         * @param idx Specifies the index (position) of the VkPushConstantRange to return.
         * @return Returns the vulkan VkPushConstantRange at the specifies index (position).
         * @details The index operator does not perform a range check. Therefore accessing with an
         * invalid index may cause undefined behaviour.
         */
        constexpr const VkPushConstantRange& operator[] (uint32_t idx) const noexcept;

        /**
         * @brief Checks, if all 'N' push constant ranges have been used.
         * @throw std::runtime_error If any push constant range is unused.
         * @detail This is an optional function that can be called to check, if all push constant ranges
         * have actually been used. Unused push constant ranges will have a size of 0 and do not occupy
         * any push constant buffer memory. If an unused range is detected, the check fails and an
         * exception is thrown.
         */
        constexpr void validate(void) const;
    };

    template<uint32_t N>
    class PushConstants
    {
    private:
        static constexpr const char* MSG_ACCESS = "[vka::PushConstants::at]: Accessing a push constant range from an invalid PushConstant object is forbidden.";

        const PushConstantLayout<N>* m_layout;
        detail::PushConstantBuffer m_buff;

    public:

        /// @brief Default initialization. Does not allocate any memory.
        inline PushConstants(void) noexcept;

        /**
         * @brief Creates the PushConstant object with a specified layout.
         * @param layout Specifies the layout that is used for the push constants.
         * @throw std::bad_alloc If memory allocation failed.
         * @details This constructor allocates memory for all push constant ranges specified by the
         * layout and has the same functionality as the function 'create()'.
         */
        explicit inline PushConstants(const PushConstantLayout<N>& layout);

        // It does not make much sense to copy a push constants.
        PushConstants(const PushConstants&) = delete;
        PushConstants& operator= (const PushConstants&) = delete;

        /**
         * @brief Moves another object of PushConstants into 'this'.
         * @param src Specifies the source object to move into 'this'.
         * @details 'this' now holds the ownership of the push constant memory from the source object.
         * The source object becomes invalidated after the move operation. If 'this' already contains
         * a valid push constant buffer, this buffer will be deleted and replaced by the source buffer.
         * It is valid to call create() on the source object after the move as this function will
         * completely reinitialize the object.
         */
        inline PushConstants(PushConstants&& src) noexcept;
        inline PushConstants& operator= (PushConstants&& src) noexcept;

        virtual ~PushConstants(void) = default;

        /**
         * @brief Creates the PushConstant object with a specified layout.
         * @param layout Specifies the layout that is used for the push constants.
         * @throw std::bad_alloc If memory allocation failed.
         * @details This function allocates memory for all push constant ranges specified by the layout.
         */
        inline void create(const PushConstantLayout<N>& layout);

        /// @brief Deletes the allocated memory.
        inline void destroy(void) noexcept;

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
         * @throw std::runtime_error If 'this' is not a valid PushConstants object. An access to an
         * uninitialized / invalid object may cause undefined behaviour.
         * @details This function does perform a range check.
         */
        inline PushConstantView at(uint32_t idx) const;

        /**
         * @param idx Specifies the index (position) of the push constant range to return.
         * @return Returns a view on the push constant range at a specified index (position).
         * @details The index operator does not perform a range check. Therefore accessing with an
         * invalid index may cause undefined behaviour. Undefined behaviour may also be cause, if
         * 'this' is an uninitialized / invalid object.
         */
        inline PushConstantView operator[] (uint32_t idx) const noexcept;

        /// @return Returns 'true', if 'this' is valid and 'false' otherwise.
        inline bool is_valid(void) const noexcept;
    };
}

#include "vka_push_constant_inline_impl.inl"
