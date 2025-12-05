#pragma once

namespace vka
{
    /**
     * A handle guard is an object that takes a pointer to one or more handles in the constructor and subsequently
     * destroys them in the destructor if it goes out of scope. The lifetime of the object is only in a certain scope,
     * and it cannot be moved or copied. Guarded handles are meant to be released on the success path before exiting the
     * scope.
     */
    template<detail::handle::dispatchable_c Handle, typename Parent = detail::handle::parent_t<Handle>>
    class handle_guard final
    {
    public:
        /**
         * Initialize with a handle and its associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with 'handle'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, 'handle' must be nullptr.
         * @param handle Specifies a pointer to the guarded handle. It must be a pointer to a handle which either
         * contains VK_NULL_HANDLE or a valid handle-value. The pointer must not be nullptr.\n
         * Precondition: If this parameter is not nullptr, 'parent' must be a valid handle.
         */
        explicit constexpr handle_guard(Parent parent, const Handle* handle) noexcept;

        /**
         * Initialize with an array of handles and their associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with all handles in 'handles'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, 'handle' must be nullptr.
         * @param handles Specifies a pointer to the guarded handles. It must be a pointer to an array of handles which
         * either contain VK_NULL_HANDLE or valid handle-values. The pointer must not be nullptr.
         * @param count Specifies the number of handles to guard.\n
         * Precondition: If this parameter is not nullptr, 'parent' must be a valid handle.
         */
        explicit constexpr handle_guard(Parent parent, const Handle* handles, uint32_t count) noexcept;

        /**
         * Destroys the guarded handles.
         * Precondition: The handles to guard are either valid or VK_NULL_HANDLE.
         */
        constexpr ~handle_guard();

        /// Releases the current handles from the guard, and they are not managed anymore.
        constexpr void release() noexcept;

        // deleted
        handle_guard(const handle_guard&) = delete;
        handle_guard& operator= (const handle_guard&) = delete;

    private:
        Parent m_parent;
        const Handle* m_handles;
        uint32_t m_count;

        /// Destroys all handles.
        constexpr void destroy_handles() noexcept;
    };

    /// This specialization does not require a parent handle.
    template<detail::handle::dispatchable_c Handle>
    class handle_guard<Handle, void> final
    {
    public:
        /**
         * Initialize with a handle.
         * @param handle Specifies a pointer to the guarded handle. It must be a pointer to a handle which either
         * contains VK_NULL_HANDLE or a valid handle-value. The pointer must not be nullptr.
         */
        explicit constexpr handle_guard(const Handle* handle) noexcept;

        /**
         * Initialize with an array of handles.
         * @param handles Specifies a pointer to the guarded handles. It must be a pointer to an array of handles which
         * either contain VK_NULL_HANDLE or valid handle-values. The pointer must not be nullptr.
         * @param count Specifies the number of handles to guard.
         */
        explicit constexpr handle_guard(const Handle* handles, uint32_t count) noexcept;

        /**
         * Destroys the guarded handles.
         * Precondition: The handles to guard are either valid or VK_NULL_HANDLE.
         */
        constexpr ~handle_guard();

        /// Releases the current handles from the guard, and they are not managed anymore.
        constexpr void release() noexcept;
        // deleted
        handle_guard(const handle_guard&) = delete;
        handle_guard& operator= (const handle_guard&) = delete;

    private:
        const Handle* m_handles;
        uint32_t m_count;

        /// Destroys all handles.
        constexpr void destroy_handles() noexcept;
    };
}