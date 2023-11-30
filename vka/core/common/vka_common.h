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
    /*
     * This class implements a one-time submit command buffer which is used to record buffer copy
     * and texture create commands. The destructor of this class does free the command buffer
     * automatically.
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

        void internal_begin(VkDevice device, VkCommandPool pool);

        // destroys the command buffer handle
        inline void destroy(void) noexcept;

    public:
        // The default constructor initializes the internal command buffer handle with VK_NULL_HANDLE
        inline CommandBufferOTS(void) noexcept;

        /*
         * This constructor allocates the command buffer and begins its recording. A device and
         * command pool is required and is specified by 'device' and 'pool'. If an error occured
         * while allocating the command buffer or while beginning the command buffer recording, a
         * std::runtime_error exception is thrown with an appropriate message about the error.
         * NOTE: This constructor does the same as begin().
         */
        explicit inline CommandBufferOTS(VkDevice device, VkCommandPool pool);

        // Copy constructor & operator is deleted
        CommandBufferOTS(const CommandBufferOTS&) = delete;
        CommandBufferOTS& operator= (const CommandBufferOTS&) = delete;

        /*
         * Moves another OTS command buffer into this. Only the command buffer handle is moved.
         * The device and command pool remains in the source object. More informally, the ownership
         * of the command buffer handle is transferred from 'src' into this.
         */
        inline CommandBufferOTS(CommandBufferOTS&& src) noexcept;
        inline CommandBufferOTS& operator= (CommandBufferOTS&& src) noexcept;

        // Frees the command buffer.
        inline ~CommandBufferOTS(void);

        /*
         * allocates the command buffer and begins its recording. A device and command pool is
         * required and is specified by 'device' and 'pool'. If an error occured while allocating
         * the command buffer or while beginning the command buffer recording, a std::runtime_error
         * exception is thrown with an appropriate message about the error.
         */
        inline void begin(VkDevice device, VkCommandPool pool);

        // Ends the recording of the command buffer and submits it to the specified queue 'queue'.
        void end(VkQueue queue) const;

        /*
         * Does the same as end() but it additionally waits for the command buffer to finish.
         * If no fence is specified, it is waited until the specified queue goes into idle mode.
         * Otherwise, it is waited for the fence to become signaled, of if the timeout time has
         * been elapsed. If the fence timed out VK_TIMEOUT is returned. If the fence did not time
         * out, VK_SUCCESS is returned. If an error occured while waiting on either the queue or
         * the fence, a std::runtime_error exception is thrown with an appropriate message about
         * the error.
         */
        VkResult end_wait(VkQueue queue, VkFence fence = VK_NULL_HANDLE, uint64_t timeout = vka::NO_TIMEOUT) const;

        // Returns the vulkan VkCommandBuffer handle.
        inline VkCommandBuffer handle(void) const noexcept;
    };

    namespace common
    {
        /*
        * Converts format feature flags to image usage flags. The format feature flags to convert
        * are specified by 'format_feature'. The corresponding image usage flags are returned.
        * NOTE: The function name stands for "convert format feature to image usage".
        */
        constexpr VkImageUsageFlags cvt_ff2iu(VkFormatFeatureFlags format_feature) noexcept;

        /*
        * Converts image usage flags to format feature flags. The image usage flags to convert are
        * specified by 'image_usage'. The corresponding format feature flags are returned.
        * NOTE: The function name stands for "convert image usage to format feature".
        */
        constexpr VkFormatFeatureFlags cvt_iu2ff(VkImageUsageFlags image_usage) noexcept;

        /*
        * Converts a vector of std::string strings to an array of const char*. The vector of std
        * strings is given by 'std_in' and the resulting output vector is stored in 'ccp_out'.
        * This function is mainly used to convert extension and layer names from std::string to
        * const char* in order to pass them to the corresponding create-infos. The size of the
        * array must correspond to the size of the vector.
        * NOTE: The function name stands for "convert std::string to const char pointer vector"
        */
        void cvt_stdstr2ccpv(const std::vector<std::string>& std_in, const char** ccp_out) noexcept;

        /*
        * Converts a vulkan format to a size in bytes. The format to convert is specified by
        * 'format'. The size in bytes is returned.
        */
        inline size_t format_sizeof(VkFormat format);
    }
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_common_impl.inl"
#endif
#include "vka_common_inline_impl.inl"
