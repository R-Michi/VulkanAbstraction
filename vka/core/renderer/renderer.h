#pragma once

namespace vka
{
    /**
     * Abstraction to render images to the screen. This class performs all synchronizations internally.
     *
     * <b>Default initialization:</b>\n
     * The default constructor creates an <b>empty</b> renderer. This empty object is invalid and is not capable of
     * rendering any images. You will get undefined behaviour or even a crash, if you attempt to call <c>execute()</c>.
     * Calling <c>destroy()</c> does nothing.
     *
     * <b>Initialization:</b>\n
     * The initialization constructor creates a valid renderer that is capable of rendering images by calling
     * <c>execute()</c>, if no exception was thrown.
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
     * Destroys all internal vulkan handles and sets everything back to default values. After destroying the object is
     * an <b>empty</b> renderer.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * You can use the renderer in a thread different from the main thread. From the window the renderer references,
     * only the swapchain is used and no action is performed that includes calling a GLFW-function. However, if you use
     * this class across multiple threads, actions must be externally synchronized.
     *
     * <b>Actions:</b>
     * - <b>rendering</b> -- Invoked by <c>execute()</c> renders the next swapchain image to the screen.
     */
    class Renderer final
    {
        using Handle = detail::renderer::Handle;

    public:
        /// Creates an empty renderer. This renderer is invalid.
        constexpr Renderer() noexcept;

        /**
         * Creates the renderer. The renderer is valid if no exception was thrown.
         * @param device Device with which the renderer is created.
         * @param window Window to which the renderer will render.
         * @param fif_count Number of frames in flight to render.
         * @throw std::runtime_error Is thrown, if creating the render context (fences and semaphores) failed.
         * @throw std::invalid_argument Is thrown, if <c>fif_count > window.image_count()</c>
         * @pre <c>window</c> is a valid window.
         */
        explicit Renderer(VkDevice device, const Window& window, uint32_t fif_count);

        /// @return Returns whether the renderer is valid.
        explicit constexpr operator bool() const noexcept;

        /// Destroys the renderer. After destroying the renderer is empty and therefore invalid.
        constexpr void destroy() noexcept;

        /**
         * Executes the render process.
         * @param queue Graphics queue to which the render commands are submitted.
         * @param cbos Array of command buffers created from the swapchain images.
         * @param sync_stage Stage that should be waited on until a swapchain image becomes available.
         * @return Returns <c>true</c> if the swapchain must be recreated.
         * @throw std::runtime_error Is thrown, if acquiring swapchain images, waiting for fences, resetting fences,
         * submitting commands or presenting swapchain images failed.
         * @pre This function is only called on valid renderer objects.
         */
        bool execute(VkQueue queue, const VkCommandBuffer* cbos, VkPipelineStageFlags sync_stage);

        // default:
        Renderer(Renderer&&) = default;
        ~Renderer() = default;
        Renderer& operator= (Renderer&&) = default;

    private:
        static constexpr const char* MSG_FENCE_CREATE_FAILED        = "[vka::Renderer]: Failed to create fences.";
        static constexpr const char* MSG_FENCE_WAIT_FAILED          = "[vka::Renderer]: Failed to wait for fence.";
        static constexpr const char* MSG_FENCE_RESET_FAILED         = "[vka::Renderer]: Failed to reset fence.";
        static constexpr const char* MSG_SEMAPHORE_CREATE_FAILED    = "[vka::Renderer]: Failed to create semaphores.";
        static constexpr const char* MSG_ACQUIRE_IMAGE_FAILED       = "[vka::Renderer]: Failed to acquire image.";
        static constexpr const char* MSG_SUBMIT_FAILED              = "[vka::Renderer]: Failed to submit commands";
        static constexpr const char* MSG_PRESENT_FAILED             = "[vka::Renderer]: Failed to present image";
        static constexpr const char* MSG_INVALID_FIF_COUNT          = "[vka::Renderer]: Number of frames in flight must be less than the number of swapchain images.";

        const Window* m_window;
        unique_handle<Handle> m_context;
        std::vector<uint32_t> m_map_image2frame;
        std::vector<uint32_t> m_map_frame2image;
        uint32_t m_frame_index;

        /// @return Returns the next frame in flight.
        inline uint32_t next_frame() noexcept;

        /// Waits for one fence.
        inline void wait_fence(VkFence fence) const;

        /// Waits for all fences.
        void wait_all_fences();

        /// Acquires the next image.
        VkResult acquire_image(VkSemaphore semaphore, uint32_t& image_index);

        /// Presents the image.
        bool preset_image(VkQueue queue, VkSemaphore semaphore, uint32_t image_index, VkResult acquire_result);

        /// Creates the render context.
        static unique_handle<Handle> create_context(VkDevice device, const Window& window, uint32_t fif_count);

    };
}