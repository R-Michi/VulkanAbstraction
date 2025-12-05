#pragma once

#include "unique_handle.h"

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::unique_handle<Handle[], 0, Parent>::unique_handle() noexcept :
    m_parent(VK_NULL_HANDLE),
    m_handles(nullptr),
    m_count(0)
{}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::unique_handle<Handle[], 0, Parent>::unique_handle(Parent parent, const Handle* handles, uint32_t count) noexcept :
    m_parent(parent),
    m_handles(handles),
    m_count(count)
{}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::unique_handle<Handle[], 0, Parent>::unique_handle(unique_handle&& src) noexcept :
    m_parent(src.m_parent),
    m_handles(src.m_handles),
    m_count(src.m_count)
{
    src.reset_state();
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::unique_handle<Handle[], 0, Parent>::~unique_handle()
{
    this->destroy_handles();
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::unique_handle<Handle[], 0, Parent>& vka::unique_handle<Handle[], 0, Parent>::operator= (null_handle_t) noexcept
{
    this->destroy_handles();
    this->reset_state();
    return *this;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::unique_handle<Handle[], 0, Parent>& vka::unique_handle<Handle[], 0, Parent>::operator= (unique_handle&& src) noexcept
{
    this->destroy_handles();
    this->m_parent = src.m_parent;
    this->m_handles = src.m_handles;
    this->m_count = src.m_count;
    src.reset_state();
    return *this;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr bool vka::unique_handle<Handle[], 0, Parent>::operator== (const unique_handle& src) const noexcept
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

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr bool vka::unique_handle<Handle[], 0, Parent>::operator!= (const unique_handle& src) const noexcept
{
    return !(*this == src);
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr Handle vka::unique_handle<Handle[], 0, Parent>::operator[](uint32_t index) const noexcept(detail::handle::debug_noexcept)
{
#ifdef _DEBUG
    if (index >= this->m_count) [[unlikely]]
        throw std::out_of_range("unique_handle: Index out of range.");
#endif
    return this->m_handles[index];
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr vka::unique_handle<Handle[], 0, Parent>::operator bool() const noexcept
{
    return this->m_handles != nullptr;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr void vka::unique_handle<Handle[], 0, Parent>::destroy_handles() noexcept
{
    // Precondition: m_handles == nullptr implies m_count == 0
    // Precondition: m_count != 0 implies m_handles != nullptr
    for (uint32_t i = 0; i < this->m_count; i++)
    {
        if (this->m_handles[i] != VK_NULL_HANDLE)
            detail::handle::destroy_f<Handle>(this->m_parent, this->m_handles[i], nullptr);
    }
    delete[] this->m_handles;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr void vka::unique_handle<Handle[], 0, Parent>::reset_state() noexcept
{
    this->m_handles = nullptr;
    this->m_count = 0;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], 0, Parent>::get() const noexcept
{
    return this->m_handles;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr Parent vka::unique_handle<Handle[], 0, Parent>::parent() const noexcept
{
    return this->m_parent;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr const Handle* vka::unique_handle<Handle[], 0, Parent>::release() noexcept
{
    const Handle* const handles = this->m_handles;
    this->reset_state();
    return handles;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr void vka::unique_handle<Handle[], 0, Parent>::reset(const Handle* handles, uint32_t count)
{
    this->destroy_handles();
    this->m_handles = handles;
    this->m_count = count;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr void vka::unique_handle<Handle[], 0, Parent>::reset(Parent parent, const Handle* handles, uint32_t count)
{
    this->destroy_handles();
    this->m_parent = parent;
    this->m_handles = handles;
    this->m_count = count;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr void vka::unique_handle<Handle[], 0, Parent>::destroy() noexcept
{
    this->destroy_handles();
    this->reset_state();
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
void vka::unique_handle<Handle[], 0, Parent>::swap(unique_handle& handle) noexcept
{
    Parent tmp_parent = this->m_parent;
    const Handle* tmp_handles = this->m_handles;
    uint32_t tmp_count = this->m_count;
    this->m_parent = handle.m_parent;
    this->m_handles = handle.m_handles;
    this->m_count = handle.m_count;
    handle.m_parent = tmp_parent;
    handle.m_handles = tmp_handles;
    handle.m_count = tmp_count;
}

template<vka::detail::handle::dispatchable_c Handle, typename Parent>
constexpr uint32_t vka::unique_handle<Handle[], 0, Parent>::size() const noexcept
{
    return this->m_count;
}

    // unused code that might be useful later
#if 0
template<detail::handle::dispatchable Handle, typename Parent>
class unique_handle<Handle[], 0, Parent>
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
#endif