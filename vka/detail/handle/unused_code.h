/// ####################################################################################################################
/// ############################################## STATIC ARRAY DEFINITION #############################################
/// ####################################################################################################################

/// This specialization manages an array of handles.
template<typename Handle, uint32_t N, auto deleter, typename Parent>
class unique_handle<Handle[], N, deleter, Parent> final
{
    static_assert(detail::handle::copyable_c<Handle>, "Handle type must be copyable.");
    static_assert(detail::handle::copyable_c<Parent>, "Parent handle type must be copyable.");
    static_assert(detail::handle::bool_convertable_c<Handle>, "Handle type must be convertable to bool via static_cast.");
    static_assert(detail::handle::destroy_func_c<decltype(deleter)>, "Invalid deleter function for the specified handle.");
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

    /**
     * Resets the internal state.
     * Precondition: The memory has been freed or moved.
     */
    constexpr void reset_state() noexcept;
};

/// This is equivalent to an array of unique_handle<Handle>.
template<typename Handle, uint32_t N, auto deleter>
class unique_handle<Handle[], N, deleter, void>
{
    static_assert(detail::handle::always_false_v<Handle, N>, "Use an array of unique_handle<Handle> for parent handles.");
};


/// ####################################################################################################################
/// ############################################ STATIC ARRAY IMPLEMENTATION ###########################################
/// ####################################################################################################################

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], N, deleter, Parent>::unique_handle() noexcept :
    m_parent{}, // VK_NULL_HANDLE for primitive handles
    m_handles{} // VK_NULL_HANDLE for primitive handles
{}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], N, deleter, Parent>::unique_handle(Parent parent, const Handle* handles) noexcept :
    m_parent(parent)
{
    for (uint32_t i = 0; i < N; i++)
        this->m_handles[i] = handles[i];
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], N, deleter, Parent>::unique_handle(Parent parent, const std::array<Handle, N>& handles) noexcept :
    m_parent(parent),
    m_handles(handles)
{}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], N, deleter, Parent>::unique_handle(unique_handle&& src) noexcept :
    m_parent(src.m_parent),
    m_handles(src.m_handles)
{
    src.reset_state();
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], N, deleter, Parent>::~unique_handle()
{
    this->destroy_handles();
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], N, deleter, Parent>& vka::unique_handle<Handle[], N, deleter, Parent>::operator= (null_handle_t) noexcept
{
    this->destroy_handles();
    this->reset_state();
    return *this;
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], N, deleter, Parent>& vka::unique_handle<Handle[], N, deleter, Parent>::operator= (unique_handle&& src) noexcept
{
    this->destroy_handles();
    this->m_parent = src.m_parent;
    this->m_handles = src.m_handles;
    src.reset_state();
    return *this;
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr bool vka::unique_handle<Handle[], N, deleter, Parent>::operator== (const unique_handle& src) const noexcept
{
    return this->m_handles == src.m_handles;
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr bool vka::unique_handle<Handle[], N, deleter, Parent>::operator!= (const unique_handle& src) const noexcept
{
    return !(*this == src);
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr Handle vka::unique_handle<Handle[], N, deleter, Parent>::operator[] (uint32_t index) const noexcept(detail::handle::debug_noexcept)
{
#ifdef _DEBUG
    if (index >= N) [[unlikely]]
        throw std::out_of_range("unique_handle: Index out of range.");
#endif
    return this->m_handles[index];
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr vka::unique_handle<Handle[], N, deleter, Parent>::operator bool() const noexcept
{
    for (uint32_t i = 0; i < N; i++)
    {
        if (static_cast<bool>(this->m_handles[i]))
            return true;
    }
    return false;
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], N, deleter, Parent>::destroy_handles() noexcept
{
    for (const Handle handle : this->m_handles)
    {
        if (static_cast<bool>(handle))
            deleter(this->m_parent, handle, nullptr);
    }
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], N, deleter, Parent>::reset_state() noexcept
{
    this->m_handles = {}; // sets primitive handles to VK_NULL_HANDLE
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], N, deleter, Parent>::get() const noexcept
{
    return this->m_handles.data();
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr Parent vka::unique_handle<Handle[], N, deleter, Parent>::parent() const noexcept
{
    return this->m_parent;
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr std::array<Handle, N> vka::unique_handle<Handle[], N, deleter, Parent>::release() noexcept
{
    std::array<Handle, N> handles = this->m_handles;
    this->reset_state();
    return handles;
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], N, deleter, Parent>::release(Handle* handles) noexcept
{
    for (uint32_t i = 0; i < N; i++)
        handles[i] = this->m_handles[i];
    this->reset_state();
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], N, deleter, Parent>::reset(const Handle* handles) noexcept
{
    this->destroy_handles();
    for (uint32_t i = 0; i < N; i++)
        this->m_handles[i] = handles[i];
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], N, deleter, Parent>::reset(const std::array<Handle, N>& handles) noexcept
{
    this->destroy_handles();
    this->m_handles = handles;
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], N, deleter, Parent>::reset(Parent parent, const Handle* handles) noexcept
{
    this->destroy_handles();
    this->m_parent = parent;
    for (uint32_t i = 0; i < N; i++)
        this->m_handles[i] = handles[i];
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], N, deleter, Parent>::reset(Parent parent, const std::array<Handle, N>& handles) noexcept
{
    this->destroy_handles();
    this->m_parent = parent;
    this->m_handles = handles;
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr void vka::unique_handle<Handle[], N, deleter, Parent>::destroy() noexcept
{
    this->destroy_handles();
    this->reset_state();
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
void vka::unique_handle<Handle[], N, deleter, Parent>::swap(unique_handle& handle) noexcept
{
    Parent tmp_parent = this->m_parent;
    std::array<Handle, N> tmp_handles = this->m_handles;
    this->m_parent = handle.m_parent;
    this->m_handles = handle.m_handles;
    handle.m_parent = tmp_parent;
    handle.m_handles = tmp_handles;
}

template<typename Handle, uint32_t N, auto deleter, typename Parent>
constexpr uint32_t vka::unique_handle<Handle[], N, deleter, Parent>::size() noexcept
{
    return N;
}

/// ####################################################################################################################
/// ############################################### DYNAMIC HANDLE ARRAY ###############################################
/// ####################################################################################################################

template<detail::handle::dispatchable Handle, typename Parent>
class unique_handle<Handle[], deleter, Parent>
{
    static_assert(std::is_void_v<Parent>, "Use an array of unique_handle<Handle>, if Handle does not have a parent handle.");

private:
    static constexpr uint32_t MIN_CAPACITY = 16;

public:
    constexpr unique_handle() noexcept :
        m_parent(VK_NULL_HANDLE),
        m_handles(nullptr),
        m_capacity(0),
        m_count(0)
    {}

    unique_handle(Parent parent, const Handle* handles, uint32_t count) :
        m_parent(parent),
        m_handles(nullptr),
        m_capacity(0),
        m_count(0)
    {
        this->allocate_first(count, handles);
    }

    unique_handle(Parent parent, const std::vector<Handle>& handles) :
        m_parent(parent),
        m_handles(nullptr),
        m_capacity(0),
        m_count(0)
    {
        this->allocate_first(handles.size(), handles.data());
    }

    constexpr unique_handle(unique_handle&& src) noexcept :
        m_parent(src.m_parent),
        m_handles(src.m_handles),
        m_capacity(src.m_capacity),
        m_count(src.m_count)
    {
        src.reset_state();
    }

    constexpr ~unique_handle()
    {
        this->free();
    }

    constexpr unique_handle& operator= (null_handle_t) noexcept
    {
        // Only invalidate the memory and not destroy it.
        this->destroy_handles();
        this->invalidate_memory();
        return *this;
    }

    constexpr unique_handle& operator= (unique_handle&& src) noexcept
    {
        this->free();
        this->m_parent = src.m_parent;
        this->m_handles = src.m_handles;
        this->m_capacity = src.m_capacity;
        this->m_count = src.m_count;
        src.reset_state();
        return *this;
    }

    constexpr bool operator== (const unique_handle& src) const noexcept
    {
        if (this->m_count != src.m_count)
            return false;

        for (uint32_t i = 0; i < this->m_count; i++)
        {
            if (this->m_handles[i] != src.m_handles[i])
                return false;
        }
        return true;
    }

    constexpr bool operator!= (const unique_handle& src) const noexcept
    {
        return !(*this == src);
    }

    constexpr Handle operator[](uint32_t index) const noexcept
    {
#ifdef _DEBUG
        if (index >= this->m_count) [[unlikely]]
            throw std::out_of_range("unique_handle: Index out of range.");
#endif
        return this->m_handles[index];
    }

    explicit constexpr operator bool() const noexcept
    {
        for (uint32_t i = 0; i < this->m_count; i++)
        {
            if (this->m_handles[i] != VK_NULL_HANDLE)
                return true;
        }
        return false;
    }

    [[nodiscard]]
    std::vector<Handle> release()
    {
        // Returns an empty vector if no memory is allocated.
        if (this->m_handles == nullptr)
            return std::vector<Handle>{};

        // copy the handles away
        std::vector<Handle> handles(this->m_handles, this->m_handles + this->m_count);

        // Only invalidate the memory and not destroy it.
        this->invalidate_memory();
        return handles;
    }

    constexpr void release(Handle* handle = nullptr) noexcept
    {
        if (handle != nullptr)
        {
            // m_handles == nullptr implies m_count == 0
            // m_count != 0 implies m_handles != nullptr
            for (uint32_t i = 0; i < this->m_count; i++)
                handle[i] = this->m_handles[i];
        }

        // Only invalidate the memory and not destroy it.
        this->invalidate_memory();
    }

    void reset(const Handle* handles, uint32_t count)
    {
        this->destroy_handles();
        this->allocate(count, handles);
    }

    void reset(const std::vector<Handle>& handles)
    {
        this->destroy_handles();
        this->allocate(handles.size(), handles.data());
    }

    void reset(Parent parent, const Handle* handles, uint32_t count)
    {
        this->destroy_handles();
        this->m_parent = parent;
        this->allocate(count, handles);
    }

    void reset(Parent parent, const std::vector<Handle>& handles)
    {
        this->destroy_handles();
        this->m_parent = parent;
        this->allocate(handles.size(), handles.data());
    }

    constexpr void destroy() noexcept
    {
        this->destroy_handles();
        this->invalidate_memory();
    }

    void swap(unique_handle& handle) noexcept
    {
        if (this->m_handles == handle.m_handles == nullptr)
            return;

        if (this->m_handles == nullptr)
        {
            // Precondition: handle.m_count > 0
            Parent tmp_parent = this->m_parent;
            this->m_parent = handle.m_parent;
            this->m_handles = handle.m_handles;
            this->m_capacity = handle.m_capacity;
            this->m_count = handle.m_count;
            handle.m_parent = tmp_parent;
            handle.reset_state();
        }
        else if (handle == nullptr)
        {
            // Precondition: this->m_count > 0
            Parent tmp_parent = handle.m_parent;
            handle.m_parent = this->m_parent;
            handle.m_handles = this->m_handles;
            handle.m_capacity = this->m_capacity;
            handle.m_count = this->m_count;
            this->m_parent = tmp_parent;
            this->reset_state();
        }
        else
        {
            // Precondition: handle.m_count > 0 and this->m_count > 0
            const uint32_t tmp_count = this->m_count;
            const Parent tmp_parent = this->m_handles;
            Handle* const tmp_handles = new Handle[this->m_count];
            for (uint32_t i = 0; i < this->m_count; i++)
                tmp_handles[i] = this->m_handles[i];

            this->m_parent = handle.m_parent;
            this->allocate(handle.m_count, handle.m_handles);

            handle.m_parent = tmp_parent;
            handle.allocate(tmp_count, tmp_handles);
            delete[] tmp_handles;
        }
    }

    constexpr uint32_t size() const noexcept
    {
        return this->m_count;
    }

private:
    Parent m_parent;
    Handle* m_handles;
    uint32_t m_capacity;
    uint32_t m_count;

    /// Destroys all handles but does not invalidate them.
    constexpr void destroy_handles() noexcept
    {
        // m_handles == nullptr implies m_count == 0
        // m_count != 0 implies m_handles != nullptr
        for (uint32_t i = 0; i < this->m_count; i++)
        {
            if (this->m_handles[i] != VK_NULL_HANDLE)
                detail::handle::destroy_f<Handle>(this->m_parent, this->m_handles[i], nullptr);
        }
    }

    /**
     * Allocates the initial memory with at least 'count' elements.\n
     * Precondition: m_handles == nullptr
     */
    void allocate_first(uint32_t size, const Handle* handles)
    {
        // If the size is 0, m_handles remains nullptr.
        if (size > 0 && handles != nullptr)
        {
            // the capacity is at least MIN_CAPACITY or 1.5*size
            const uint32_t capacity = size <= MIN_CAPACITY ? MIN_CAPACITY : (size << 1) / 3;

            // Allocate memory and initialize it.
            this->m_handles = new Handle[capacity];
            for (uint32_t i = 0; i < size; i++)
                this->m_handles[i] = handles[i];

            this->m_capacity = capacity;
            this->m_count = size;
        }
    }

    /**
     * Checks if the size has changed and reallocates memory if required.\n
     * Precondition: m_handles != nullptr, handles != nullptr. The old handles have been deleted or moved.
     */
    void grow(uint32_t new_size, const Handle* handles)
    {
        // do nothing if 'handles' is nullptr
        if (handles == nullptr) return;

        // Reallocation only needed if the new size exceeds the capacity.
        if (new_size > this->m_capacity) [[unlikely]] // optimize for the else path
        {
            // growth factor 1.5x
            const uint32_t new_capacity = (new_size << 1) / 3;

            // If it throws a bad_alloc, the state of this has not been changed. After that nothing can throw an
            // exception.
            Handle* const new_handles = new Handle[new_capacity];

            // The handles have been moved successfully.
            delete[] this->m_handles;
            this->m_handles = new_handles;
            this->m_capacity = new_capacity;
        }

        // initialize the memory with new handles
        for (uint32_t i = 0; i < new_size; i++)
            this->m_handles[i] = handles[i];
        this->m_count = new_size;
    }

    /// Allocates / reallocates memory.
    void allocate(uint32_t new_size, const Handle* handles)
    {
        if (this->m_handles == nullptr)
            this->allocate_first(new_size, handles);
        else
            this->grow(new_size, handles);
    }

    /// Destroys all handles and frees the memory.
    constexpr void free() noexcept
    {
        this->destroy_handles();
        delete[] this->m_handles;
    }

    /**
     * Resets the internal state.
     * Precondition: The memory has been freed or moved.
     */
    constexpr void reset_state() noexcept
    {
        this->m_handles = nullptr;
        this->m_count = this->m_capacity = 0;
    }

    /// Invalidates the memory.
    constexpr void invalidate_memory() noexcept
    {
        this->m_count = 0;
    }
};