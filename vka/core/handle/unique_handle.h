#pragma once

namespace vka
{
    /**
     * A unique-handle is an object that manages a non-dispatchable vulkan handle and subsequently destroys it if it
     * goes out of scope. This specialization requires a parent handle associated with the managed handle.
     *
     * You can also use implementations of unique_handle with custom handle-types, parent-types and custom deleters.
     * However, the deleter function requires a specific signature. It must be of the function-pointer-type
     * 'DestroyFunc<Handle, Parent>' or 'ParentDestroyFunc<Handle>' for parent handles respectively.
     *
     * The custom handle-type needs the following requirements:
     * - It must be convertable to bool.
     * - It must be copyable.
     *
     * Optionally, the handle-type can overload the following operators:
     * - operator== (null_handle_t) -> compares with VK_NULL_HANDLE for equality
     * - operator== (const Handle&) -> compares for equality
     *
     * The custom parent-type needs the following requirements:
     * - It must be copyable.
     *
     * Allowed signatures for deleter functions for normal handles are:
     * - void(*)(Parent, Handle, const VkAllocationCallbacks*)
     * - void(*)(Parent, const Handle&, const VkAllocationCallbacks*)
     * - void(*)(const Parent&, Handle, const VkAllocationCallbacks*)
     * - void(*)(const Parent&, const Handle&, const VkAllocationCallbacks*)
     *
     * Allowed signatures for deleter functions for parent handles are:
     * - void(*)(Handle, const VkAllocationCallbacks*)
     * - void(*)(const Handle&, const VkAllocationCallbacks*)
     *
     * If you specify any custom handle-type (which can be a combination of multiple vulkan handles), the default parent
     * is always VkDevice.
     *
     * Furthermore, you may want to extend the functionality of this class. For this purpose you are allowed to inherit
     * from it. But never use it in a context like:
     *  - unique_handle<Handle>* p = new Derived(...) or
     *  - function(unique_handle<Handle>& p) and function(object_of_derived)
     *
     * unless you know what you are doing because the destructor is not virtual - this is by design!
     */
    template<
        typename Handle,
        auto deleter = detail::handle::destroy_f<detail::handle::remove_array<Handle>>,
        typename Parent = detail::handle::parent_t<detail::handle::remove_array<Handle>>
    >
    class unique_handle
    {
        static_assert(detail::handle::copyable_c<Handle>, "Handle type must be copyable.");
        static_assert(detail::handle::copyable_c<Parent>, "Parent handle type must be copyable.");
        static_assert(detail::handle::bool_convertable_c<Handle>, "Handle type must be convertable to bool via static_cast.");
        static_assert(detail::handle::destroy_func_c<decltype(deleter)>, "Invalid deleter function for the specified handle.");
    public:
        /// Default initialization.
        constexpr unique_handle() noexcept;

        /**
         * Initialize with a handle and its associated parent handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with 'handle'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, 'handle' must be VK_NULL_HANDLE.
         * @param handle Specifies the handle. It must either be VK_NULL_HANDLE or a valid handle.\n
         * Precondition: If this parameter is a valid handle, 'parent' must also be a valid handle.
         */
        explicit constexpr unique_handle(Parent parent, Handle handle) noexcept;

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
         * Releases the current handle from the object and replaces it with a new handle.
         * @param handle Specifies a handle with which the current handle is replaced. It must either be VK_NULL_HANDLE
         * or a valid handle.\n
         * Precondition: The handle must have been created with the same parent handle as stored in this. If the parent
         * handle stored in this is VK_NULL_HANDLE, the specified handle must also be VK_NULL_HANDLE.
         * @return Returns the current handle.
         */
        constexpr Handle release_reset(Handle handle) noexcept;

        /**
         * Releases the current handle from the object and replaces it with a new handle and a new associated parent
         * handle.
         * @param parent Specifies the parent handle. It must either be VK_NULL_HANDLE or a valid parent which is
         * associated with 'handle'.\n
         * Precondition: If this parameter is VK_NULL_HANDLE, 'handle' must be VK_NULL_HANDLE.
         * @param handle Specifies a handle with which the current handle is replaced. It must either be VK_NULL_HANDLE
         * or a valid handle.\n
         * Precondition: If this parameter is a valid handle, 'parent' must be a valid handle.
         * @return Returns the current handle.
         */
        constexpr Handle release_reset(Parent parent, Handle handle) noexcept;

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
         * @param handle Specifies a handle with which the current handle is replaced. It must either be VK_NULL_HANDLE
         * or a valid handle.\n
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

        /**
         * Resets the internal state.
         * Precondition: The memory has been freed or moved.
         */
        constexpr void reset_state() noexcept;
    };

    /// This specialization does not require a parent handle.
    template<typename Handle, auto deleter>
    class unique_handle<Handle, deleter, void>
    {
        static_assert(detail::handle::copyable_c<Handle>, "Handle type must be copyable.");
        static_assert(detail::handle::bool_convertable_c<Handle>, "Handle type must be convertable to bool via static_cast.");
        static_assert(detail::handle::parent_destroy_func_c<decltype(deleter)>, "Invalid deleter function for the specified handle.");
    public:
        /// Default initialization.
        constexpr unique_handle() noexcept;

        /**
         * Initialize with a handle.
         * @param handle Specifies the handle. It must either be VK_NULL_HANDLE or a valid handle.
         */
        explicit constexpr unique_handle(Handle handle = {}) noexcept;

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
         * Releases the current handle from the object and replaces it with a new handle.
         * @param handle Specifies a handle with which the current handle is replaced. It must either be VK_NULL_HANDLE
         * or a valid handle.
         * @return Returns the current handle.
         */
        constexpr Handle release_reset(Handle handle) noexcept;

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

        /**
         * Resets the internal state.
         * Precondition: The memory has been freed or moved.
         */
        constexpr void reset_state() noexcept;
    };

    /// This specialization manages a dynamic array of handles. It is a combination of unique_ptr and unique_handle.
    template<typename Handle, auto deleter, typename Parent>
    class unique_handle<Handle[], deleter, Parent>
    {
        static_assert(detail::handle::copyable_c<Handle>, "Handle type must be copyable.");
        static_assert(detail::handle::copyable_c<Parent>, "Parent handle type must be copyable.");
        static_assert(detail::handle::bool_convertable_c<Handle>, "Handle type must be convertable to bool via static_cast.");
        static_assert(detail::handle::destroy_func_c<decltype(deleter)>, "Invalid deleter function for the specified handle.");
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
        constexpr unique_handle(Parent parent, Handle* handles, uint32_t count) noexcept;

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
        constexpr Handle* get() noexcept;

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
         * Releases the current handles and the dynamic array from the object and replaces them with a new dynamic array
         * of handles.
         * @param handles Specifies a dynamic array with which the current handles are replaced. The array must either
         * be 'nullptr' or must contain VK_NULL_HANDLE or valid handles.\n
         * Precondition: The handles must have been created with the same parent handle as stored in this. If the parent
         * handle stored in this is VK_NULL_HANDLE, 'handles' must either be nullptr or all handles in the array must be
         * VK_NULL_HANDLE. If this parameter is nullptr, 'count' must also be 0.
         * @param count Specifies the number of handles.\n
         * Precondition: If this parameter is greater than 0, 'handles' must be a valid pointer to an array of either
         * VK_NULL_HANDLE or valid handles.
         * @return Returns the current handles.
         */
        constexpr const Handle* release_reset(Handle* handles, uint32_t count) noexcept;

        /**
         * Releases the current handles and the dynamic array from the object and replaces them with a new dynamic array
         * of handles.
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
         * @return Returns the current handles.
         */
        constexpr const Handle* release_reset(Parent parent, Handle* handles, uint32_t count) noexcept;

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
        constexpr void reset(Handle* handles, uint32_t count) noexcept;

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
        constexpr void reset(Parent parent, Handle* handles, uint32_t count) noexcept;

        /// Destroys the current handles and the dynamic array owned by the object. The parent handle is not affected.
        constexpr void destroy() noexcept;

        /// Swaps the handles of the two unique-handle-objects.
        void swap(unique_handle& handle) noexcept;

        /// Returns the number of managed handles by this object.
        constexpr uint32_t size() const noexcept;

        /// @return Returns a pointer to the first handle.
        constexpr Handle* begin() noexcept;

        /// @return Returns a pointer to the first handle.
        constexpr const Handle* begin() const noexcept;

        /// @return Returns a pointer end pointer.
        constexpr const Handle* end() const noexcept;

        // deleted
        unique_handle(const unique_handle&) = delete;
        unique_handle& operator= (const unique_handle&) = delete;

    private:
        Parent m_parent;
        Handle* m_handles;
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
    template<typename Handle, auto deleter>
    class unique_handle<Handle[], deleter, void>
    {
        static_assert(detail::handle::always_false_v<Handle, 0>, "Use an array of unique_handle<Handle> for parent handles.");
    };
}