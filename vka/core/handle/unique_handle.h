#pragma once

namespace vka
{
    /**
     * A unique-handle is an object that manages a dispatchable vulkan handle and subsequently destroys it if it goes
     * out of scope. This specialization requires a parent handle associated with the managed handle.
     */
    template<detail::handle::dispatchable_c Handle, uint32_t N = 0, typename Parent = detail::handle::parent_t<Handle>>
    class unique_handle final
    {
    public:
        /**
         * Initialize with a handle and its associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with 'handle'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, 'handle' must be VK_NULL_HANDLE.
         * @param handle Specifies the handle. It must either be VK_NULL_HANDLE or a valid handle.\n
         * Precondition: If this parameter is a valid handle, 'parent' must also be a valid handle.
         */
        explicit constexpr unique_handle(Parent parent = VK_NULL_HANDLE, Handle handle = VK_NULL_HANDLE) noexcept;

        /// Transfers the ownership of the handle in 'src' to 'this'.
        constexpr unique_handle(unique_handle&& src) noexcept;

        /// Destroys the current handle.
        constexpr ~unique_handle();

        /// Destroys the current handle and replaces it with VK_NULL_HANDLE. Same functionality as destroy().
        constexpr unique_handle& operator= (null_handle_t) noexcept;

        /// Destroys the current handle and transfers the ownership of the handle in 'src' to 'this'.
        constexpr unique_handle& operator= (unique_handle&& src) noexcept;

        /// Checks if the current handle is a VK_NULL_HANDLE.
        constexpr bool operator== (null_handle_t) const noexcept;

        /// Compares two handles for equality.
        constexpr bool operator== (const unique_handle& src) const noexcept;

        /// Checks if the current handle is not a VK_NULL_HANDLE. Same functionality as operator bool().
        constexpr bool operator!= (null_handle_t) const noexcept;

        /// Compares two handles for inequality.
        constexpr bool operator!= (const unique_handle& src) const noexcept;

        /// @return Returns 'true' if the current handle is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the current handle.
        constexpr Handle get() const noexcept;

        /// @return Returns the current parent handle.
        constexpr Parent parent() const noexcept;

        /**
         * Releases the current handle from the object, and it is not managed anymore.
         * @return Returns the current handle.
         */
        constexpr Handle release() noexcept;

        /**
         * Destroys the current handle and replaces it with a new handle.
         * @param handle Specifies a handle with which the current handle is replaced. It must either be VK_NULL_HANDLE
         * or a valid handle.\n
         * Precondition: The handle must have been created with the same parent handle as stored in this. If the parent
         * handle stored in this is VK_NULL_HANDLE, the specified handle must also be VK_NULL_HANDLE.
         */
        constexpr void reset(Handle handle) noexcept;

        /**
         * Destroys the current handle and replaces it with a new handle and a new associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with 'handle'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, 'handle' must be VK_NULL_HANDLE.
         * @param handle Specifies the handle. It must either be VK_NULL_HANDLE or a valid handle.\n
         * Precondition: If this parameter is a valid handle, 'parent' must be a valid handle.
         */
        constexpr void reset(Parent parent, Handle handle) noexcept;

        /// Destroys the current handle and sets it to VK_NULL_HANDLE. The parent handle is not affected.
        constexpr void destroy() noexcept;

        /// Swaps the handles of the two unique-handle-objects.
        void swap(unique_handle& handle) noexcept;

        // deleted
        unique_handle(const unique_handle&) = delete;
        unique_handle& operator= (const unique_handle&) = delete;

    private:
        Parent m_parent;
        Handle m_handle;

        /// Destroys the handle but does not invalidate it.
        constexpr void destroy_handle() noexcept;
    };

    /// This specialization does not require a parent handle.
    template<detail::handle::dispatchable_c Handle, uint32_t N>
    class unique_handle<Handle, N, void> final
    {
    public:
        /**
         * Initialize with a handle.
         * @param handle Specifies the handle. It must either be VK_NULL_HANDLE or a valid handle.
         */
        explicit constexpr unique_handle(Handle handle = VK_NULL_HANDLE) noexcept;

        /// Transfers the ownership of the handle in 'src' to 'this'.
        constexpr unique_handle(unique_handle&& src) noexcept;

        /// Destroys the current handle.
        constexpr ~unique_handle();

        /// Destroys the current handle and replaces it with VK_NULL_HANDLE. Same functionality as destroy().
        constexpr unique_handle& operator= (null_handle_t) noexcept;

        /// Destroys the current handle and transfers the ownership of the handle in 'src' to 'this'.
        constexpr unique_handle& operator= (unique_handle&& src) noexcept;

        /// Checks if the current handle is a VK_NULL_HANDLE.
        constexpr bool operator== (null_handle_t) const noexcept;

        /// Compares two handles for equality.
        constexpr bool operator== (const unique_handle& src) const noexcept;

        /// Checks if the current handle is not a VK_NULL_HANDLE. Same functionality as operator bool().
        constexpr bool operator!= (null_handle_t) const noexcept;

        /// Compares two handles for inequality.
        constexpr bool operator!= (const unique_handle& src) const noexcept;

        /// @return Returns 'true' if the current handle is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the current handle.
        constexpr Handle get() const noexcept;

        /**
         * Releases the current handle from the object, and it is not managed anymore.
         * @return Returns the current handle.
         */
        constexpr Handle release() noexcept;

        /**
         * Destroys the current handle and replaces it with a new handle.
         * @param handle Specifies a handle with which the current handle is replaced. It must either be VK_NULL_HANDLE
         * or a valid handle.
         */
        constexpr void reset(Handle handle) noexcept;

        /// Destroys the current handle and sets it to VK_NULL_HANDLE. The parent handle is not affected.
        constexpr void destroy() noexcept;

        /// Swaps the handles of the two unique-handle-objects.
        void swap(unique_handle& handle) noexcept;

        // deleted
        unique_handle(const unique_handle&) = delete;
        unique_handle& operator= (const unique_handle&) = delete;

    private:
        Handle m_handle;

        /// Destroys the handle but does not invalidate it.
        constexpr void destroy_handle() noexcept;
    };

    /**
     * This specialization manages an array of handles. If the specified handle does not have a parent handle, it is
     * equivalent to an array of unique_handle<Handle>.
     */
    template<detail::handle::dispatchable_c Handle, uint32_t N, typename Parent>
    class unique_handle<Handle[], N, Parent> final
    {
    public:
        /// Empty initialization.
        constexpr unique_handle() noexcept;

        /**
         * Initializes with a pointer to an array of handles and their associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with all handles in 'handles'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, all handles in 'handles' must be VK_NULL_HANDLE.
         * @param handles Specifies the handles. The array must contain either VK_NULL_HANDLE or valid handles.\n
         * Precondition: The pointer to the array must not be nullptr. If the array contains valid handles, 'parent'
         * must also be a valid handle.
         */
        constexpr unique_handle(Parent parent, const Handle* handles) noexcept;

        /**
         * Initializes with an array of handles and their associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with all handles in 'handles'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, all handles in 'handles' must be VK_NULL_HANDLE.
         * @param handles Specifies the handles. The array must contain either VK_NULL_HANDLE or valid handles.\n
         * Precondition: If the array contains valid handles, 'parent' must also be a valid handle.
         */
        constexpr unique_handle(Parent parent, const std::array<Handle, N>& handles) noexcept;

        /// Transfers the ownership of the handles in 'src' to 'this'.
        constexpr unique_handle(unique_handle&& src) noexcept;

        /// Destroys the handles.
        constexpr ~unique_handle();

        /// Destroys the current handles and replaces them with VK_NULL_HANDLE. Same functionality as destroy().
        constexpr unique_handle& operator= (null_handle_t) noexcept;

        /// Destroys the current handles and transfers the ownership of the handles in 'src' to 'this'.
        constexpr unique_handle& operator= (unique_handle&& src) noexcept;

        /// Compares the two handle arrays for equality.
        constexpr bool operator==(const unique_handle& src) const noexcept;

        /// Compares the two handle arrays for inequality.
        constexpr bool operator!=(const unique_handle& src) const noexcept;

        /**
         * Does not perform a range check on the specified index.
         * @param index Specifies the index of the handle.\n
         * Precondition: The index must be a valid index in the range [0, N-1].
         * @return Returns the handle at the specified index.
         */
        constexpr Handle operator[](uint32_t index) const noexcept(detail::handle::debug_noexcept);

        /**
         * The object is valid if at least one handle is not equal to VK_NULL_HANDLE.
         * @return Returns 'true' if the object is valid.
         */
        explicit constexpr operator bool() const noexcept;

        /// @return Returns a pointer to the managed handles.
        constexpr const Handle* get() const noexcept;

        /// @return Returns the current parent handle.
        constexpr Parent parent() const noexcept;

        /**
         * Releases the current handles from the object, and they are not managed anymore.
         * @return Returns the current handles.
         */
        constexpr std::array<Handle, N> release() noexcept;

        /**
         * Releases the current handles from the object, and they are not managed anymore.
         * @param handles Returns the current handles by pointer.
         */
        constexpr void release(Handle* handles) noexcept;

        /**
         * Destroys the current handles and replaces them with new handles.
         * @param handles Specifies the handles with which the current handles are replaced. They must either be
         * VK_NULL_HANDLE or valid handles.\n
         * Precondition: The pointer to the array must not be nullptr. The handles must have been created with the same
         * parent handle as stored in this. If the parent handle stored in this is VK_NULL_HANDLE, the specified handles
         * must also be VK_NULL_HANDLE.
         */
        constexpr void reset(const Handle* handles) noexcept;

        /**
         * Destroys the current handles and replaces them with new handles.
         * @param handles Specifies the handles with which the current handles are replaced. They must either be
         * VK_NULL_HANDLE or valid handles.\n
         * Precondition: The handles must have been created with the same parent handle as stored in this. If the parent
         * handle stored in this is VK_NULL_HANDLE, the specified handles must also be VK_NULL_HANDLE.
         */
        constexpr void reset(const std::array<Handle, N>& handles) noexcept;

        /**
         * Destroys the current handles and replaces them with new handles and a new associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with all handles in 'handles'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, all handles in 'handles' must be VK_NULL_HANDLE.
         * @param handles Specifies the handles with which the current handles are replaced. The array must contain
         * either VK_NULL_HANDLE or valid handles.\n
         * Precondition: The pointer to the array must not be nullptr. If the array contains valid handles, 'parent'
         * must also be a valid handle.
         */
        constexpr void reset(Parent parent, const Handle* handles) noexcept;

        /**
         * Destroys the current handles and replaces them with new handles and a new associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with all handles in 'handles'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, all handles in 'handles' must be VK_NULL_HANDLE.
         * @param handles Specifies the handles with which the current handles are replaced. The array must contain
         * either VK_NULL_HANDLE or valid handles.\n
         * Precondition: If the array contains valid handles, 'parent' must also be a valid handle.
         */
        constexpr void reset(Parent parent, const std::array<Handle, N>& handles) noexcept;

        /// Destroys the current handles and sets them to VK_NULL_HANDLE. The parent handle is not affected.
        constexpr void destroy() noexcept;

        /// Swaps the handles of the two unique-handle-objects.
        void swap(unique_handle& handle) noexcept;

        /// Returns the number of managed handles by this object.
        static constexpr uint32_t size() noexcept;

        // deleted:
        unique_handle(const unique_handle&) = delete;
        unique_handle& operator= (const unique_handle&) = delete;

    private:
        Parent m_parent;
        std::array<Handle, N> m_handles;

        /// Destroys all handles but does not invalidate them.
        constexpr void destroy_handles() noexcept;
    };

    /// This is equivalent to an array of unique_handle<Handle>.
    template<detail::handle::dispatchable_c Handle, uint32_t N>
    class unique_handle<Handle[], N, void>
    {
        static_assert(detail::handle::always_false_v<Handle, N>, "Use an array of unique_handle<Handle>, if 'Handle' does not have a parent handle.");
    };

    /// This specialization manages a dynamic array of handles. It is a combination of unique_ptr and unique_handle.
    template<detail::handle::dispatchable_c Handle, typename Parent>
    class unique_handle<Handle[], 0, Parent>
    {
    public:
        /// Empty initialization.
        constexpr unique_handle() noexcept;

        /**
         * Initializes with a pointer to a dynamic array of handles and their associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with all handles in 'handles'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, 'handles' must either be nullptr or all handles in the
         * array must be VK_NULL_HANDLE.
         * @param handles Specifies a pointer to a dynamic array of handles. The array must either be 'nullptr' or must
         * contain VK_NULL_HANDLE or valid handles.\n
         * Precondition: If the array contains valid handles, 'parent' must also be a valid handle. If this parameter is
         * nullptr, 'count' must also be 0.
         * @param count Specifies the number of handles.\n
         * Precondition: If this parameter is greater than 0, 'handles' must be a valid pointer to an array of either
         * VK_NULL_HANDLE or valid handles.
         */
        constexpr unique_handle(Parent parent, const Handle* handles, uint32_t count) noexcept;

        /// Transfers the ownership of the handles in 'src' to 'this'.
        constexpr unique_handle(unique_handle&& src) noexcept;

        /// Destroys the handles and the dynamic array owned by the object.
        constexpr ~unique_handle();

        /// Destroys the current handles and the dynamic array owned by the object. Same functionality as destroy().
        constexpr unique_handle& operator= (null_handle_t) noexcept;

        /// Destroys the current handles and transfers the ownership of the handles in 'src' to 'this'.
        constexpr unique_handle& operator= (unique_handle&& src) noexcept;

        /// Compares the two handle arrays for equality.
        constexpr bool operator== (const unique_handle& src) const noexcept;

        /// Compares the two handle arrays for inequality.
        constexpr bool operator!= (const unique_handle& src) const noexcept;

        /**
         * Does not perform a range check on the specified index.
         * @param index Specifies the index of the handle.\n
         * Precondition: The index must be a valid index in the range [0, size()-1].
         * @return Returns the handle at the specified index.
         */
        constexpr Handle operator[](uint32_t index) const noexcept(detail::handle::debug_noexcept);

        /// @return Returns 'true' if the object is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns a pointer to the managed handles.
        constexpr const Handle* get() const noexcept;

        /// @return Returns the current parent handle.
        constexpr Parent parent() const noexcept;

        /**
         * Releases the current handles and the dynamic array from the object. The object manages neither the handles
         * nor the dynamic array anymore.
         * @return Returns the current handles.
         */
        constexpr const Handle* release() noexcept;

        /**
         * Destroys the current handles and replaces them with a new dynamic array of handles.
         * @param handles Specifies a dynamic array with which the current handles are replaced. The array must either
         * be 'nullptr' or must contain VK_NULL_HANDLE or valid handles.\n
         * Precondition: The handles must have been created with the same parent handle as stored in this. If the parent
         * handle stored in this is VK_NULL_HANDLE, 'handles' must either be nullptr or all handles in the array must be
         * VK_NULL_HANDLE. If this parameter is nullptr, 'count' must also be 0.
         * @param count Specifies the number of handles.\n
         * Precondition: If this parameter is greater than 0, 'handles' must be a valid pointer to an array of either
         * VK_NULL_HANDLE or valid handles.
         */
        constexpr void reset(const Handle* handles, uint32_t count);

        /**
         * Destroys the current handles and replaces them with a new dynamic array of handles.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with all handles in 'handles'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, 'handles' must either be nullptr or all handles in the
         * array must be VK_NULL_HANDLE.
         * @param handles Specifies a dynamic array with which the current handles are replaced. The array must either
         * be 'nullptr' or must contain VK_NULL_HANDLE or valid handles.\n
         * Precondition: If the array contains valid handles, 'parent' must also be a valid handle. If this parameter is
         * nullptr, 'count' must also be 0.
         * @param count Specifies the number of handles.\n
         * Precondition: If this parameter is greater than 0, 'handles' must be a valid pointer to an array of either
         * VK_NULL_HANDLE or valid handles.
         */
        constexpr void reset(Parent parent, const Handle* handles, uint32_t count);

        /// Destroys the current handles and the dynamic array owned by the object. The parent handle is not affected.
        constexpr void destroy() noexcept;

        /// Swaps the handles of the two unique-handle-objects.
        void swap(unique_handle& handle) noexcept;

        /// Returns the number of managed handles by this object.
        constexpr uint32_t size() const noexcept;

        // deleted
        unique_handle(const unique_handle&) = delete;
        unique_handle& operator= (const unique_handle&) = delete;

    private:
        Parent m_parent;
        const Handle* m_handles;
        uint32_t m_count;

        /// Destroys all handles and frees the memory.
        constexpr void destroy_handles() noexcept;

        /**
         * Resets the internal state.
         * Precondition: The memory has been freed or moved.
         */
        constexpr void reset_state() noexcept;
    };

    /// This is equivalent to a dynamic array of unique_handle<Handle>.
    template<detail::handle::dispatchable_c Handle>
    class unique_handle<Handle[], 0, void>
    {
        static_assert(detail::handle::always_false_v<Handle, 0>, "Use an array of unique_handle<Handle>, if 'Handle' does not have a parent handle.");
    };
}