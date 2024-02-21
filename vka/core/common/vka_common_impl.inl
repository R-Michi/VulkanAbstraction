/**
* @file     vka_common_impl.inl
* @brief    Implementation of commonly used functionality.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

void vka::CommandBufferOTS::destroy_handles(void) noexcept
{
    if (this->m_cbo != VK_NULL_HANDLE)
        vkFreeCommandBuffers(this->m_device, this->m_pool, 1, &this->m_cbo);
}

void vka::CommandBufferOTS::internal_begin(void)
{
    const VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = this->m_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    detail::error::check_result(vkAllocateCommandBuffers(this->m_device, &alloc_info, &this->m_cbo), CBO_ALLOC_FAILED);

    constexpr VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = nullptr
    };
    detail::error::check_result(vkBeginCommandBuffer(this->m_cbo, &begin_info), CBO_BEGIN_FAILED);
}

void vka::CommandBufferOTS::end(VkQueue queue) const
{
    const VkSubmitInfo submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = nullptr,
        .commandBufferCount = 1,
        .pCommandBuffers = &this->m_cbo,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = nullptr
    };

    detail::error::check_result(vkEndCommandBuffer(this->m_cbo), CBO_END_FAILED);
    detail::error::check_result(vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE), CBO_SUBMIT_FAILED);
}

VkResult vka::CommandBufferOTS::end_wait(VkQueue queue, VkFence fence, uint64_t timeout) const
{
    const VkSubmitInfo submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = nullptr,
        .commandBufferCount = 1,
        .pCommandBuffers = &this->m_cbo,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = nullptr
    };

    detail::error::check_result(vkEndCommandBuffer(this->m_cbo), CBO_END_FAILED);
    detail::error::check_result(vkQueueSubmit(queue, 1, &submit_info, fence), CBO_SUBMIT_FAILED);
    if(fence == VK_NULL_HANDLE)
    {
        detail::error::check_result(vkQueueWaitIdle(queue), WAIT_QUEUE_FAILED);
        return VK_SUCCESS;
    }
    const VkResult res = vkWaitForFences(this->m_device, 1, &fence, VK_TRUE, timeout);
    detail::error::check_result(res, WAIT_FENCE_FAILED);
    return res;
}
