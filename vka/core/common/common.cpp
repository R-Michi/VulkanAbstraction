/**
 * @brief Implementation for the one-time submission command buffer class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vka/vka.h>

vka::CommandBufferOTS::CommandBufferOTS(VkDevice device, VkCommandPool pool) :
    m_device(device),
    m_pool(pool),
    m_cbo(this->allocate())
{
    constexpr VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = nullptr
    };
    check_result(vkBeginCommandBuffer(this->m_cbo, &begin_info), CBO_BEGIN_FAILED);
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
    check_result(vkEndCommandBuffer(this->m_cbo), CBO_END_FAILED);
    check_result(vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE), CBO_SUBMIT_FAILED);
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
    check_result(vkEndCommandBuffer(this->m_cbo), CBO_END_FAILED);
    check_result(vkQueueSubmit(queue, 1, &submit_info, fence), CBO_SUBMIT_FAILED);

    if(fence == VK_NULL_HANDLE)
    {
        check_result(vkQueueWaitIdle(queue), WAIT_QUEUE_FAILED);
        return VK_SUCCESS;
    }

    const VkResult res = vkWaitForFences(this->m_device, 1, &fence, VK_TRUE, timeout);
    check_result(res, WAIT_FENCE_FAILED);
    return res;
}

VkCommandBuffer vka::CommandBufferOTS::allocate() const
{
    const VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = this->m_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    VkCommandBuffer cbo;
    check_result(vkAllocateCommandBuffers(this->m_device, &alloc_info, &cbo), CBO_ALLOC_FAILED);
    return cbo;
}



void vka::common::cvt_stdstr2ccpv(const std::vector<std::string>& std_in, const char** ccp_out, size_t buff_size) noexcept
{
    for (size_t i = 0; i < std_in.size() && i < buff_size; i++)
        ccp_out[i] = std_in[i].c_str();
}
