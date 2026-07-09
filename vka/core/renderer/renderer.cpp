#include <vka/vka.h>

#ifdef VKA_GLFW_ENABLE

vka::Renderer::Renderer(VkDevice device, const Window& window, uint32_t fif_count) :
    m_window(&window),
    m_context(create_context(device, window, fif_count)),
    m_map_image2frame(window.image_count(), NPOS),
    m_map_frame2image(fif_count, NPOS),
    m_frame_index(0)
{
    if (fif_count > window.image_count()) [[unlikely]]
        detail::error::throw_invalid_argument(MSG_INVALID_FIF_COUNT);
}

bool vka::Renderer::execute(VkQueue queue, const VkCommandBuffer* cbos, VkPipelineStageFlags sync_stage)
{
    const uint32_t frame_index = this->next_frame();
    const VkFence fence = this->m_context.get().fences[frame_index];
    const VkSemaphore sem_acquire = this->m_context.get().sem_acquire[frame_index];

    // Wait for the next frame in flight becomes available and update the entries in the maps.
    this->wait_fence(fence);
    if (const uint32_t a_image_idx = this->m_map_frame2image[frame_index]; a_image_idx != NPOS)
    {
        this->m_map_image2frame[a_image_idx] = NPOS;
        this->m_map_frame2image[frame_index] = NPOS;
    }

    // Request the next image from the swapchain.
    uint32_t image_index;
    const VkResult res = this->acquire_image(sem_acquire, image_index);
    if (res == VK_ERROR_OUT_OF_DATE_KHR) [[unlikely]]
        return true;

    // Check the lookup if the current image index is already on the queue. If this is the case, wait for the process to
    // finish and update the maps accordingly. Don't reset the fence because ist must stay signaled until the respective
    // frame in flight uses it again.
    if (const uint32_t a_frame_idx = this->m_map_image2frame[image_index]; a_frame_idx != NPOS)
    {
        this->wait_fence(this->m_context.get().fences[a_frame_idx]);
        this->m_map_image2frame[image_index] = NPOS;
        this->m_map_frame2image[a_frame_idx] = NPOS;
    }

    // Reset the fence of the current frame in flight and keep track of the current image index being submitted to the
    // graphics queue.
    check_result(vkResetFences(this->m_context.parent(), 1, &fence), MSG_FENCE_RESET_FAILED);
    this->m_map_image2frame[image_index] = frame_index;
    this->m_map_frame2image[frame_index] = image_index;

    // Submit commands to the queue.
    const VkSemaphore sem_render = this->m_context.get().sem_render[image_index];
    const VkSubmitInfo submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &sem_acquire,
        .pWaitDstStageMask = &sync_stage,
        .commandBufferCount = 1,
        .pCommandBuffers = cbos + image_index,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &sem_render
    };
    check_result(vkQueueSubmit(queue, 1, &submit_info, fence), MSG_SUBMIT_FAILED);

    // Present the image on the screen.
    return this->preset_image(queue, sem_render, image_index, res);
}

VkResult vka::Renderer::wait(uint64_t timeout)
{
    const VkResult res = vkWaitForFences(this->m_context.parent(), this->m_context.get().fif_count, this->m_context.get().fences, VK_TRUE, timeout);
    check_result(res, MSG_FENCE_WAIT_FAILED);

    // Reset image index tracking because at this point the queue is empty.
    for (uint32_t& i : this->m_map_image2frame) i = NPOS;
    for (uint32_t& i : this->m_map_frame2image) i = NPOS;
    return res;
}

inline uint32_t vka::Renderer::next_frame() noexcept
{
    return this->m_frame_index++ % this->m_context.get().fif_count;
}

inline void vka::Renderer::wait_fence(VkFence fence) const
{
    const VkResult res = vkWaitForFences(this->m_context.parent(), 1, &fence, VK_TRUE, NO_TIMEOUT);
    check_result(res, MSG_FENCE_WAIT_FAILED);
}

VkResult vka::Renderer::acquire_image(VkSemaphore semaphore, uint32_t& image_index)
{
    const VkResult res = vkAcquireNextImageKHR(this->m_context.parent(), this->m_window->swapchain(), NO_TIMEOUT, semaphore, VK_NULL_HANDLE, &image_index);
    if (res == VK_ERROR_OUT_OF_DATE_KHR) [[unlikely]]
    {
        this->wait(NO_TIMEOUT);
        return res;
    }
    check_result(res, MSG_ACQUIRE_IMAGE_FAILED);
    return res;
}

bool vka::Renderer::preset_image(VkQueue queue, VkSemaphore semaphore, uint32_t image_index, VkResult acquire_result)
{
    const VkSwapchainKHR swapchain = this->m_window->swapchain();
    const VkPresentInfoKHR present_info = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &semaphore,
        .swapchainCount = 1,
        .pSwapchains = &swapchain,
        .pImageIndices = &image_index,
        .pResults = nullptr
    };

    const VkResult res = vkQueuePresentKHR(queue, &present_info);
    if (acquire_result == VK_SUBOPTIMAL_KHR || res == VK_SUBOPTIMAL_KHR || res == VK_ERROR_OUT_OF_DATE_KHR) [[unlikely]]
    {
        this->wait(NO_TIMEOUT);
        return true;
    }
    check_result(res, MSG_PRESENT_FAILED);
    return false;
}

vka::unique_handle<vka::Renderer::Handle> vka::Renderer::create_context(VkDevice device, const Window& window, uint32_t fif_count)
{
    constexpr VkFenceCreateInfo fence_create_info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    constexpr VkSemaphoreCreateInfo semaphore_create_info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0
    };

    unique_handle<VkFence[]> fence_guard(device, new VkFence[fif_count]{ VK_NULL_HANDLE }, fif_count);
    unique_handle<VkSemaphore[]> sem_acquire_guard(device, new VkSemaphore[fif_count]{ VK_NULL_HANDLE }, fif_count);
    unique_handle<VkSemaphore[]> sem_render_guard(device, new VkSemaphore[window.image_count()]{ VK_NULL_HANDLE }, window.image_count());

    for (uint32_t i = 0; i < fif_count; i++)
    {
        check_result(vkCreateFence(device, &fence_create_info, nullptr, fence_guard.get() + i), MSG_FENCE_CREATE_FAILED);
        check_result(vkCreateSemaphore(device, &semaphore_create_info, nullptr, sem_acquire_guard.get() + i), MSG_SEMAPHORE_CREATE_FAILED);
    }

    for (uint32_t i = 0; i < window.image_count(); i++)
        check_result(vkCreateSemaphore(device, &semaphore_create_info, nullptr, sem_render_guard.get() + i), MSG_SEMAPHORE_CREATE_FAILED);

    const Handle handle = {
        .fences = fence_guard.release(),
        .sem_acquire = sem_acquire_guard.release(),
        .sem_render = sem_render_guard.release(),
        .image_count = window.image_count(),
        .fif_count = fif_count
    };
    return unique_handle(device, handle);
}

#endif