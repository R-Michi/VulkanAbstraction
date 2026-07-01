#pragma once

namespace vka
{
    /**
     * A <c>unique_handle</c> is an object that manages a non-dispatchable vulkan handle and subsequently destroys it if
     * it goes out of scope. You can also use implementations of <c>unique_handle</c> with custom handle-types,
     * parent-types and custom deleters. However, the deleter function requires a specific signature. It must be of the
     * function-pointer-type <c>DestroyFunc<Handle, Parent></c> or <c>ParentDestroyFunc<Handle></c> for parent handles
     * respectively.
     *
     * The custom handle-type needs the following requirements:
     * - It must be convertable to <c>bool</c>.
     * - It must be copyable.
     *
     * Optionally, the handle-type can overload the following operators:
     * - <c>operator== (null_handle_t)</c> -> compares with <c>VK_NULL_HANDLE</c> for equality
     * - <c>operator== (const Handle&)</c> -> compares for equality
     *
     * The custom parent-type needs the following requirements:
     * - It must be copyable.
     *
     * Allowed signatures for deleter functions for normal handles are (they can optionally be <c>noexcept</c>):
     * - <c>void(*)(Parent, Handle, const VkAllocationCallbacks*)</c>
     * - <c>void(*)(Parent, const Handle&, const VkAllocationCallbacks*)</c>
     * - <c>void(*)(const Parent&, Handle, const VkAllocationCallbacks*)</c>
     * - <c>void(*)(const Parent&, const Handle&, const VkAllocationCallbacks*)</c>
     *
     * Allowed signatures for deleter functions for parent handles are (they can optionally be <c>noexcept</c>):
     * - <c>void(*)(Handle, const VkAllocationCallbacks*)</c>
     * - <c>void(*)(const Handle&, const VkAllocationCallbacks*)</c>
     *
     * If you specify any custom handle-type (which can be a combination of multiple vulkan handles), the default parent
     * is always <c>VkDevice</c>.
     *
     * Furthermore, you may want to extend the functionality of this class. For this purpose you are allowed to inherit
     * from it. But never use it in a context like:
     *  - <c>unique_handle<Handle>* p = new Derived(...)</c> or
     *  - <c>function(unique_handle<Handle>& p)</c> and <c>function(object_of_derived)</c>
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
        /// Default initialization. Initializes the managed handles to <c>VK_NULL_HANDLE</c>.
        constexpr unique_handle() noexcept;

        /**
         * Initialize with a handle and its associated parent handle.
         * @param parent Must either be <c>VK_NULL_HANDLE</c> or a valid parent handle associated with <c>handle</c>.
         * @param handle Must either be <c>VK_NULL_HANDLE</c> or a valid handle.
         * @pre If <c>parent</c> is <c>VK_NULL_HANDLE</c>, <c>handle</c> must also be <c>VK_NULL_HANDLE</c>.
         * @pre If <c>handle</c> is a valid handle, <c>parent</c> must also be a valid handle.
         */
        explicit constexpr unique_handle(Parent parent, Handle handle) noexcept;

        /// Transfers the ownership of a handle to a different <c>unique_handle</c> object.
        constexpr unique_handle(unique_handle&& src) noexcept;

        /// Destroys the current handle.
        constexpr ~unique_handle();

        /**
         * Destroys the current handle and replaces it with <c>VK_NULL_HANDLE</c>. Same functionality as
         * <c>destroy()</c>.
         */
        constexpr unique_handle& operator= (null_handle_t) noexcept;

        /**
         * Transfers the ownership of a handle to a different <c>unique_handle</c> object. A current valid handle is
         * destroyed.
         */
        constexpr unique_handle& operator= (unique_handle&& src) noexcept;

        /// Checks if the current handle is a <c>VK_NULL_HANDLE</c>.
        constexpr bool operator== (null_handle_t) const noexcept;

        /// Compares two handles for equality.
        constexpr bool operator== (const unique_handle& src) const noexcept;

        /// Checks if the current handle is not <c>VK_NULL_HANDLE</c>. Same functionality as <c>operator bool()</c>.
        constexpr bool operator!= (null_handle_t) const noexcept;

        /// Compares two handles for inequality.
        constexpr bool operator!= (const unique_handle& src) const noexcept;

        /// @return Returns <c>true</c> if the current handle is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the current parent handle.
        constexpr Parent parent() const noexcept;

        /// @return Returns the current handle.
        constexpr Handle get() const noexcept;

        /**
         * Releases the current handle from the <c>unique_handle</c> object. This handle is not managed anymore.
         * @return Returns the current handle.
         */
        constexpr Handle release() noexcept;

        /**
         * Releases the current handle from the <c>unique_handle</c> object and replaces it with a new handle. The
         * current handle is not managed anymore.
         * @param handle New handle. Must either be <c>VK_NULL_HANDLE</c> or a valid handle.
         * @return Returns the current handle.
         * @pre The new handle must have been created with the same parent handle as currently stored. If the current
         * parent handle is <c>VK_NULL_HANDLE</c>, the new handle must also be <c>VK_NULL_HANDLE</c>.
         */
        constexpr Handle release_reset(Handle handle) noexcept;

        /**
         * Releases the current handle from the <c>unique_handle</c> object and replaces it with a new handle and a new
         * associated parent handle. The current handle is not managed anymore.
         * @param parent New parent handle. Must either be <c>VK_NULL_HANDLE</c> or a valid parent handle associated
         * with <c>handle</c>.
         * @param handle New handle. Must either be <c>VK_NULL_HANDLE</c> or a valid handle.
         * @return Returns the current handle.
         * @pre If <c>parent</c> is <c>VK_NULL_HANDLE</c>, <c>handle</c> must also be <c>VK_NULL_HANDLE</c>.
         * @pre If <c>handle</c> is a valid handle, <c>parent</c> must also be a valid handle.
         */
        constexpr Handle release_reset(Parent parent, Handle handle) noexcept;

        /**
         * Destroys the current handle and replaces it with a new handle.
         * @param handle New handle. Must either be <c>VK_NULL_HANDLE</c> or a valid handle.
         * @pre The new handle must have been created with the same parent handle as currently stored. If the current
         * parent handle is <c>VK_NULL_HANDLE</c>, the new handle must also be <c>VK_NULL_HANDLE</c>.
         */
        constexpr void reset(Handle handle) noexcept;

        /**
         * Destroys the current handle and replaces it with a new handle and a new associated parent handle.
         * @param parent New parent handle. Must either be <c>VK_NULL_HANDLE</c> or a valid parent handle associated
         * with <c>handle</c>.
         * @param handle New handle. Must either be <c>VK_NULL_HANDLE</c> or a valid handle.
         * @pre If <c>parent</c> is <c>VK_NULL_HANDLE</c>, <c>handle</c> must also be <c>VK_NULL_HANDLE</c>.
         * @pre If <c>handle</c> is a valid handle, <c>parent</c> must also be a valid handle.
         */
        constexpr void reset(Parent parent, Handle handle) noexcept;

        /// Destroys the current handle and sets it to <c>VK_NULL_HANDLE</c>. The parent handle is not affected.
        constexpr void destroy() noexcept;

        /// Swaps the handles of the two <c>unique_handle</c> objects.
        constexpr void swap(unique_handle& handle) noexcept;

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
         * @pre The memory has been freed or moved.
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
        /// Default initialization. Initializes the managed handle to <c>VK_NULL_HANDLE</c>.
        constexpr unique_handle() noexcept;

        /**
         * Initialize with a handle.
         * @param handle Must either be <c>VK_NULL_HANDLE</c> or a valid handle.
         */
        explicit constexpr unique_handle(Handle handle) noexcept;

        /// Transfers the ownership of a handle to a different <c>unique_handle</c> object.
        constexpr unique_handle(unique_handle&& src) noexcept;

        /// Destroys the current handle.
        constexpr ~unique_handle();

        /**
         * Destroys the current handle and replaces it with <c>VK_NULL_HANDLE</c>. Same functionality as
         * <c>destroy()</c>.
         */
        constexpr unique_handle& operator= (null_handle_t) noexcept;

        /**
         * Transfers the ownership of a handle to a different <c>unique_handle</c> object. A current valid handle is
         * destroyed.
         */
        constexpr unique_handle& operator= (unique_handle&& src) noexcept;

        /// Checks if the current handle is a <c>VK_NULL_HANDLE</c>.
        constexpr bool operator== (null_handle_t) const noexcept;

        /// Compares two handles for equality.
        constexpr bool operator== (const unique_handle& src) const noexcept;

        /// Checks if the current handle is not <c>VK_NULL_HANDLE</c>. Same functionality as <c>operator bool()</c>.
        constexpr bool operator!= (null_handle_t) const noexcept;

        /// Compares two handles for inequality.
        constexpr bool operator!= (const unique_handle& src) const noexcept;

        /// @return Returns <c>true</c> if the current handle is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the current handle.
        constexpr Handle get() const noexcept;

        /**
         * Releases the current handle from the <c>unique_handle</c> object. This handle is not managed anymore.
         * @return Returns the current handle.
         */
        constexpr Handle release() noexcept;

        /**
         * Releases the current handle from the <c>unique_handle</c> object and replaces it with a new handle. The
         * current handle is not managed anymore.
         * @param handle New handle. Must either be <c>VK_NULL_HANDLE</c> or a valid handle.
         * @return Returns the current handle.
         */
        constexpr Handle release_reset(Handle handle) noexcept;

        /**
         * Destroys the current handle and replaces it with a new handle.
         * @param handle New handle. Must either be <c>VK_NULL_HANDLE</c> or a valid handle.
         */
        constexpr void reset(Handle handle) noexcept;

        /// Destroys the current handle and sets it to <c>VK_NULL_HANDLE</c>. The parent handle is not affected.
        constexpr void destroy() noexcept;

        /// Swaps the handles of the two <c>unique_handle</c> objects.
        constexpr void swap(unique_handle& handle) noexcept;

        // deleted
        unique_handle(const unique_handle&) = delete;
        unique_handle& operator= (const unique_handle&) = delete;

    private:
        Handle m_handle;

        /// Destroys the handle but does not invalidate it.
        constexpr void destroy_handle() noexcept;

        /**
         * Resets the internal state.
         * @pre The memory has been freed or moved.
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
         * @param parent Must either be <c>VK_NULL_HANDLE</c> or a valid parent handle associated with all handles in
         * <c>handles</c>.
         * @param handles The array must either be <c>nullptr</c> or must contain <c>count</c> handles. Those handles
         * can either be <c>VK_NULL_HANDLE</c> or valid handles.
         * @param count Number of handles managed by the <c>unique_handle</c> object.
         * @pre If <c>parent</c> is <c>VK_NULL_HANDLE</c>, <c>handles</c> must either be <c>nullptr</c> or all handles
         * in the array must be <c>VK_NULL_HANDLE</c>.
         * @pre If <c>handles</c> contains valid handles, <c>parent</c> must also be a valid handle.
         * @pre If <c>handles</c> is <c>nullptr</c>, <c>count</c> must also be <c>0</c>.
         * @pre If <c>count</c> is greater than <c>0</c>, <c>handles</c> must be a valid pointer to an array of either
         * <c>VK_NULL_HANDLE</c> or valid handles.
         */
        constexpr unique_handle(Parent parent, Handle* handles, uint32_t count) noexcept;

        /// Transfers the ownership of the handles to a different <c>unique_handle</c> object.
        constexpr unique_handle(unique_handle&& src) noexcept;

        /// Destroys the handles and the dynamic array owned by the <c>unique_handle</c> object.
        constexpr ~unique_handle();

        /**
         * Destroys the current handles and the dynamic array owned by the <c>unique_handle</c> object. Same
         * functionality as <c>destroy()</c>.
         */
        constexpr unique_handle& operator= (null_handle_t) noexcept;

        /**
         * Transfers the ownership of the handles to a different <c>unique_handle</c> object. Current valid handles are
         * destroyed.
         */
        constexpr unique_handle& operator= (unique_handle&& src) noexcept;

        /// Compares the two handle arrays for equality.
        constexpr bool operator== (const unique_handle& src) const noexcept;

        /// Compares the two handle arrays for inequality.
        constexpr bool operator!= (const unique_handle& src) const noexcept;

        /**
         * Does not perform a range check on the specified index.
         * @param index Specifies the index of the handle.
         * @return Returns the handle at the specified index.
         * @pre The index must be a valid index in the range <c>[0, size()-1]</c>.
         */
        constexpr Handle operator[](uint32_t index) const noexcept(detail::handle::debug_noexcept);

        /// @return Returns <c>true</c> if the object is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the current parent handle.
        constexpr Parent parent() const noexcept;

        /// @return Returns a pointer to the managed handles.
        constexpr Handle* get() noexcept;

        /// @return Returns a pointer to the managed handles.
        constexpr const Handle* get() const noexcept;

        /// @return Returns a pointer to the first handle.
        constexpr Handle* begin() noexcept;

        /// @return Returns a pointer to the first handle.
        constexpr const Handle* begin() const noexcept;

        /// @return Returns a pointer that points to the element after the last handle.
        constexpr const Handle* end() const noexcept;

        /// @return Returns the number of managed handles.
        constexpr uint32_t count() const noexcept;

        /**
         * Releases the current handles and the dynamic array from the <c>unique_handle</c> object. Neither the handles
         * nor the dynamic array is handled by the <c>unique_handle</c> object anymore.
         * @return Returns the current handles.
         */
        constexpr const Handle* release() noexcept;

        /**
         * Releases the current handles and the dynamic array from the <c>unique_handle</c> object and replaces them
         * with a new dynamic array of handles. Neither of the current handles nor the dynamic array is handled by the
         * <c>unique_handle</c> object anymore.
         * @param handles New dynamic array of handles. The array must either be <c>nullptr</c> or must contain
         * <c>count</c> handles. Those handles can either be <c>VK_NULL_HANDLE</c> or valid handles.
         * @param count New number of handles managed by the <c>unique_handle</c> object.
         * @return Returns the current handles.
         * @pre The new handles must have been created with the same parent handle as currently stored. If the current
         * parent handle is <c>VK_NULL_HANDLE</c>, <c>handles</c> must either be <c>nullptr</c> or all handles in the
         * array must be <c>VK_NULL_HANDLE</c>.
         * @pre If <c>handles</c> is <c>nullptr</c>, <c>count</c> must also be <c>0</c>.
         * @pre If <c>count</c> is greater than <c>0</c>, <c>handles</c> must be a valid pointer to an array of either
         * <c>VK_NULL_HANDLE</c> or valid handles.
         */
        constexpr const Handle* release_reset(Handle* handles, uint32_t count) noexcept;

        /**
         * Releases the current handles and the dynamic array from the <c>unique_handle</c> object and replaces them
         * with a new dynamic array of handles and their associated parent handle. Neither of the current handles nor
         * the dynamic array is handled by the <c>unique_handle</c> object anymore.
         * @param parent New parent handle. Must either be <c>VK_NULL_HANDLE</c> or a valid parent handle associated
         * with all handles in <c>handles</c>.
         * @param handles New dynamic array of handles. The array must either be <c>nullptr</c> or must contain
         * <c>count</c> handles. Those handles can either be <c>VK_NULL_HANDLE</c> or valid handles.
         * @param count New number of handles managed by the <c>unique_handle</c> object.
         * @return Returns the current handles.
         * @pre If <c>parent</c> is <c>VK_NULL_HANDLE</c>, <c>handles</c> must either be <c>nullptr</c> or all handles
         * in the array must be <c>VK_NULL_HANDLE</c>.
         * @pre If <c>handles</c> contains valid handles, <c>parent</c> must also be a valid handle.
         * @pre If <c>handles</c> is <c>nullptr</c>, <c>count</c> must also be <c>0</c>.
         * @pre If <c>count</c> is greater than <c>0</c>, <c>handles</c> must be a valid pointer to an array of either
         * <c>VK_NULL_HANDLE</c> or valid handles.
         */
        constexpr const Handle* release_reset(Parent parent, Handle* handles, uint32_t count) noexcept;

        /**
         * Destroys the current handles and replaces them with a new dynamic array of handles.
         * @param handles New dynamic array of handles. The array must either be <c>nullptr</c> or must contain
         * <c>count</c> handles. Those handles can either be <c>VK_NULL_HANDLE</c> or valid handles.
         * @param count New number of handles managed by the <c>unique_handle</c> object.
         * @pre The new handles must have been created with the same parent handle as currently stored. If the current
         * parent handle is <c>VK_NULL_HANDLE</c>, <c>handles</c> must either be <c>nullptr</c> or all handles in the
         * array must be <c>VK_NULL_HANDLE</c>.
         * @pre If <c>handles</c> is <c>nullptr</c>, <c>count</c> must also be <c>0</c>.
         * @pre If <c>count</c> is greater than <c>0</c>, <c>handles</c> must be a valid pointer to an array of either
         * <c>VK_NULL_HANDLE</c> or valid handles.
         */
        constexpr void reset(Handle* handles, uint32_t count) noexcept;

        /**
         * Destroys the current handles and replaces them with a new dynamic array of handles.
         * @param parent New parent handle. Must either be <c>VK_NULL_HANDLE</c> or a valid parent handle associated
         * with all handles in <c>handles</c>.
         * @param handles New dynamic array of handles. The array must either be <c>nullptr</c> or must contain
         * <c>count</c> handles. Those handles can either be <c>VK_NULL_HANDLE</c> or valid handles.
         * @param count New number of handles managed by the <c>unique_handle</c> object.
         * @pre If <c>parent</c> is <c>VK_NULL_HANDLE</c>, <c>handles</c> must either be <c>nullptr</c> or all handles
         * in the array must be <c>VK_NULL_HANDLE</c>.
         * @pre If <c>handles</c> contains valid handles, <c>parent</c> must also be a valid handle.
         * @pre If <c>handles</c> is <c>nullptr</c>, <c>count</c> must also be <c>0</c>.
         * @pre If <c>count</c> is greater than <c>0</c>, <c>handles</c> must be a valid pointer to an array of either
         * <c>VK_NULL_HANDLE</c> or valid handles.
         */
        constexpr void reset(Parent parent, Handle* handles, uint32_t count) noexcept;

        /// Destroys the current handles and the dynamic array owned by the object. The parent handle is not affected.
        constexpr void destroy() noexcept;

        /// Swaps the handles of the two <c>unique_handle</c> objects.
        constexpr void swap(unique_handle& handle) noexcept;

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
         * @pre The memory has been freed or moved.
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