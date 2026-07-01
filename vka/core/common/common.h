/**
 * @brief Includes conversion functions from image use flags to format feature flags and vice versa. Includes a helper
 * class to create one-time submission command buffers.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
    /// Helper class for a command buffer that can only be submitted once.
    class CommandBufferOTS
    {
    public:
        /**
         * Allocates the command buffer and begins its recording.
         * @param device Device from which <c>pool</c> was created.
         * @param pool Pool from which the command buffer is allocated.
         */
        explicit CommandBufferOTS(VkDevice device, VkCommandPool pool);

        /**
         * Moves a command buffer. The source command buffer becomes invalidated and using to results in undefined
         * behaviour.
         */
        inline CommandBufferOTS(CommandBufferOTS&& src) noexcept;

        /// Frees the command buffer.
        inline ~CommandBufferOTS();

        /**
         * Moves a command buffer. The source command buffer becomes invalidated and using to results in undefined
         * behaviour. An already allocated command buffer is destroyed.
         */
        inline CommandBufferOTS& operator= (CommandBufferOTS&& src) noexcept;

        /// @return Returns the vulkan <c>VkCommandBuffer</c> handle.
        inline VkCommandBuffer handle() const noexcept;

        /**
         * Ends the recording of the command buffer and submits it. This operation does not wait for the execution of
         * the command buffer to finish. If you want to wait use <c>end_wait</c>.
         * @param queue Queue to which the command buffer is submitted.
         */
        void end(VkQueue queue) const;

        /**
         * Ends the recording of the command buffer, submits it and waits for the command buffer execution to finish.
         * @param queue Queue to which the command buffer is submitted.
         * @param fence Optionally specifies a fence which is used for the wait. If no fence is specified, it is waited
         * until the queue goes into idle mode.
         * @param timeout Optionally specifies a timeout for the wait.
         * @return Returns <c>VK_TIMEOUT</c> if the fence timed out. Otherwise, <c>VK_SUCCESS</c> is returned.
         */
        VkResult end_wait(VkQueue queue, VkFence fence = VK_NULL_HANDLE, uint64_t timeout = NO_TIMEOUT) const;

        // deleted:
        CommandBufferOTS(const CommandBufferOTS&) = delete;
        CommandBufferOTS& operator= (const CommandBufferOTS&) = delete;

    private:
        static constexpr char CBO_ALLOC_FAILED[] = "[vka::CommandBufferOTS]: Failed to allocate command buffer.";
        static constexpr char CBO_BEGIN_FAILED[] = "[vka::CommandBufferOTS]: Failed to begin command buffer recording.";
        static constexpr char CBO_END_FAILED[] = "[vka::CommandBufferOTS]: Failed to end command buffer recording.";
        static constexpr char CBO_SUBMIT_FAILED[] = "[vka::CommandBufferOTS]: Failed to submit command buffer.";
        static constexpr char WAIT_QUEUE_FAILED[] = "[vka::CommandBufferOTS]: Failed to wait for queue to become idling.";
        static constexpr char WAIT_FENCE_FAILED[] = "[vka::CommandBufferOTS]: Failed to wait for fence.";

        VkDevice m_device;
        VkCommandPool m_pool;
        VkCommandBuffer m_cbo;

        /// Frees the command buffer.
        inline void destroy() const noexcept;

        /// Allocates the command buffer.
        VkCommandBuffer allocate() const;
    };

    namespace common
    {
        /**
         * Converts a vector of <c>std::string</c> strings to an array of <c>const char*</c>. This function is mainly
         * used to convert extension and layer names from <c>std::string</c> to <c>const char*</c> to pass them to the
         * corresponding create-infos.
         * @param std_in Vector of <c>std::string</c> strings.
         * @param ccp_out Array in which to store the <c>const char*</c> strings. The size of the array must correspond
         * to the size of the vector.
         * @param buff_size Optionally specifies the maximum buffer size.
         * @note The function name stands for "convert std::string to const char pointer vector"
         */
        void cvt_stdstr2ccpv(const std::vector<std::string>& std_in, const char** ccp_out, size_t buff_size = NSIZE) noexcept;

        /**
         * Converts format feature flags to image usage flags.
         * @param format_feature Format feature flags to convert.
         * @return Returns the corresponding image usage flags.
         * @note The function name stands for "convert format feature to image usage".
         */
        constexpr VkImageUsageFlags cvt_ff2iu(VkFormatFeatureFlags format_feature) noexcept;

        /**
         * Converts image usage flags to format feature flags.
         * @param image_usage Image usage flags to convert.
         * @return Returns the corresponding format feature flags.
         * @note The function name stands for "convert image usage to format feature".
         */
        constexpr VkFormatFeatureFlags cvt_iu2ff(VkImageUsageFlags image_usage) noexcept;

        /// Calculates the extent of a given mip-map level.
        constexpr VkExtent3D mip_extent(VkExtent3D extent, uint32_t level) noexcept;

        /// Calculates the size of the next mip-map level.
        constexpr VkExtent3D next_mip_extent(VkExtent3D extent) noexcept;
        constexpr VkOffset3D next_mip_extent(VkOffset3D extent) noexcept;
    }
}
