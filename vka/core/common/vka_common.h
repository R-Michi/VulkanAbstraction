/**
* @file     vka_common.h
* @brief    Commonly used functionality.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../detail/common/vka_common.h"

namespace vka
{
    /**
     * @brief Implements a one-time submit command buffer.
     * @details This command buffer can only be submitted once. Its primary use case is recording buffer copy and
     * texture creation commands and other things that only need to be executed once.
     */
    class CommandBufferOTS final
    {
    private:
        static constexpr const char CBO_ALLOC_FAILED[] = "[vka::CommandBufferOTS::begin]: Failed to allocate command buffer.";
        static constexpr const char CBO_BEGIN_FAILED[] = "[vka::CommandBufferOTS::begin]: Failed to begin command buffer recording.";
        static constexpr const char CBO_END_FAILED[] = "[vka::CommandBufferOTS::end/end_wait]: Failed to end command buffer recording.";
        static constexpr const char CBO_SUBMIT_FAILED[] = "[vka::CommandBufferOTS::end/end_wait]: Failed to submit command buffer.";
        static constexpr const char WAIT_QUEUE_FAILED[] = "[vka::CommandBufferOTS::end_wait]: Failed to wait for queue to become idling.";
        static constexpr const char WAIT_FENCE_FAILED[] = "[vka::CommandBufferOTS::end/end_wait]: Failed to wait for fence.";

        VkDevice m_device;
        VkCommandPool m_pool;
        VkCommandBuffer m_cbo;

        /// @brief Destroy all vulkan handles allocated by this class.
        void destroy_handles(void) noexcept;

        /// @brief Allocates the command buffer and begins it's recording.
        void internal_begin(void);

    public:
        /// @details Initializes the internal command buffer handle with VK_NULL_HANDLE.
        inline CommandBufferOTS(void) noexcept;

        /**
         * @brief Allocates the command buffer and begins it's recording.
         * @details This constructor has the same functionality as the function begin().
         * @param device Specifies a valid device.
         * @param pool Specifies a valid command pool to allocate the command buffer from.
         * @throw std::runtime_error Is thrown, if the allocation or beginning the command buffer failed.
         */
        explicit inline CommandBufferOTS(VkDevice device, VkCommandPool pool);

        // Copy constructor & operator is deleted
        CommandBufferOTS(const CommandBufferOTS&) = delete;
        CommandBufferOTS& operator= (const CommandBufferOTS&) = delete;

        /**
         * @brief Moves the ownership from a OTS command buffer into 'this'.
         * @details The source command buffer becomes invalidated. If 'this' contains an allocated command buffer, it
         * gets destroyed and replaced by the command buffer of the source object.
         * @param src Specifies the command buffer to move.
         */
        inline CommandBufferOTS(CommandBufferOTS&& src) noexcept;
        inline CommandBufferOTS& operator= (CommandBufferOTS&& src) noexcept;

        /**
         * @brief Frees the allocated command buffer.
         * @details Although it is not required as the command pool takes care of this.
         */
        inline ~CommandBufferOTS(void);

        /**
         * @brief Allocates the command buffer and begins it's recording.
         * @param device Specifies a valid device.
         * @param pool Specifies a valid command pool to allocate the command buffer from.
         * @throw std::runtime_error Is thrown, if the allocation or beginning the command buffer failed.
         */
        inline void begin(VkDevice device, VkCommandPool pool);

        /**
         * @brief Ends the recording of the command buffer and submits the commands to a queue.
         * @param queue Specifies a valid queue to submit the commands to.
         * @throw std::runtime_error Is thrown, if ending the recording or submitting the commands failed.
         */
        void end(VkQueue queue) const;

        /**
         * @brief Ends the recording of the command buffer, submits the commands to a queue and waits until the commands
         * have been processed.
         * @details If no fence is specified, the wait lasts until the specified queue goes into idle mode. If a fence
         * is specified, the wait lasts until the fence is signaled or until the specified timeout time expires.
         * elapsed.
         * @param queue Specifies a valid queue to submit the commands to.
         * @param fence Optionally specifies a fence to wait for the command execution to finish.
         * @param timeout Optionally specifies a timeout time for the fence. This parameter defaults to a value so that
         * no timeout occurs.
         * @return Returns VK_TIMEOUT, if the fence timed out. Otherwise, VK_SUCCESS is returned.
         * @throw std::runtime_error Is throw, if ending the recording, submitting the commands, waiting for the fence
         * or the queue failed.
         */
        VkResult end_wait(VkQueue queue, VkFence fence = VK_NULL_HANDLE, uint64_t timeout = vka::NO_TIMEOUT) const;

        /// @return Returns the vulkan command buffer handle.
        inline VkCommandBuffer handle(void) const noexcept;
    };

    namespace common
    {
        /**
         * @brief Converts format feature flags to image usage flags.
         * @param format_feature Specifies the format feature flags to convert.
         * @return Returns the corresponding image usage flags.
         * @note The function name stands for "convert format feature to image usage".
         */
        constexpr VkImageUsageFlags cvt_ff2iu(VkFormatFeatureFlags format_feature) noexcept;

        /**
         * @brief Converts image usage flags to format feature flags.
         * @param image_usage Specifies the image usage flags to convert.
         * @return Returns the corresponding format feature flags.
         * @note The function name stands for "convert image usage to format feature".
         */
        constexpr VkFormatFeatureFlags cvt_iu2ff(VkImageUsageFlags image_usage) noexcept;

        /**
         * @brief Converts a vector of std::string strings to an array of const char*.
         * @details This function is mainly used to convert extension and layer names from std::string to const char* in
         * order to pass them to the corresponding create-infos.
         * @param std_in Specifies the vector of std::string strings.
         * @param ccp_out Specifies an array in which to store the const char* strings. The size of the array must
         * correspond to the size of the vector.
         * @note The function name stands for "convert std::string to const char pointer vector"
         */
        inline void cvt_stdstr2ccpv(const std::vector<std::string>& std_in, const char** ccp_out) noexcept;
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_common_impl.inl"
#endif
#include "vka_common_inline_impl.inl"
