// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "texture_top.h"

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::Texture3DMerger<F>::Texture3DMerger(const component_t* data, VkFormat format, VkExtent2D extent, uint32_t expected_depth) :
    m_data(new component_t[alloc_size(extent, alloc_depth(expected_depth))]{}),
    m_alloc_depth(alloc_depth(expected_depth)),
    m_extent(extent),
    m_depth(0)
{
    this->copy_image(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::Texture3DMerger<F>::Texture3DMerger(const component_t* color, uint32_t comp, VkExtent2D extent, uint32_t expected_depth) :
    m_data(new component_t[alloc_size(extent, alloc_depth(expected_depth))]{}),
    m_alloc_depth(alloc_depth(expected_depth)),
    m_extent(extent),
    m_depth(0)
{
    this->fill_image(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::Texture3DMerger<F>::Texture3DMerger(const char* path, uint32_t expected_depth) :
    m_data(nullptr),
    m_alloc_depth(alloc_depth(expected_depth)),
    m_extent{},
    m_depth(0)
{
    // TODO: refactor this in a separate function in detail::texture
    // TODO: begin
    int32_t width, height, components;
    std::unique_ptr<component_t[]> data(load_f(path, &width, &height, &components, 0));
    const VkExtent2D extent = { (uint32_t)width, (uint32_t)height };
    // TODO: end

    if (data == nullptr) [[unlikely]]
       detail::error::throw_invalid_argument(MSG_INVALID_PATH);

    this->m_data = new component_t[alloc_size(extent, this->m_alloc_depth)]{};
    this->m_extent = extent;
    this->copy_image(data.get(), components);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::Texture3DMerger<F>::Texture3DMerger(Texture3DMerger&& src) noexcept :
    m_data(src.m_data),
    m_alloc_depth(src.m_alloc_depth),
    m_extent(src.m_extent),
    m_depth(src.m_depth)
{
    src.m_data = nullptr;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::Texture3DMerger<F>::~Texture3DMerger()
{
    delete[] this->m_data;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::Texture3DMerger<F>& vka::Texture3DMerger<F>::operator= (Texture3DMerger&& src) noexcept
{
    delete[] this->m_data;
    this->m_data = src.m_data;
    this->m_alloc_depth = src.m_alloc_depth;
    this->m_extent = src.m_extent;
    this->m_depth = src.m_depth;
    src.m_data = nullptr;
    return *this;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline const vka::Texture3DMerger<F>::component_t* vka::Texture3DMerger<F>::data() const noexcept
{
    return this->m_data;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline VkExtent3D vka::Texture3DMerger<F>::extent() const noexcept
{
    return { this->m_extent.width, this->m_extent.height, this->m_depth };
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline uint32_t vka::Texture3DMerger<F>::depth() const noexcept
{
    return this->m_depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::Texture3DMerger<F>::load(const component_t* data, VkFormat format) noexcept
{
    if (this->m_depth >= this->m_alloc_depth)
        this->grow();
    this->copy_image(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::Texture3DMerger<F>::load(const component_t* color, uint32_t comp) noexcept
{
    if (this->m_depth >= this->m_alloc_depth)
        this->grow();
    this->fill_image(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::Texture3DMerger<F>::load(const char* path)
{
    int32_t width, height, components;
    std::unique_ptr<component_t[]> data = load_f(path, &width, &height, &components, 0);
    const VkExtent2D extent = { (uint32_t)width, (uint32_t)height };

    if (data == nullptr) [[unlikely]]
        detail::error::throw_invalid_argument(MSG_INVALID_PATH);
    if (!detail::common::cmpeq_extent(this->m_extent, extent)) [[unlikely]]
        detail::error::throw_runtime_error(MSG_EXTENT_MISSMATCH);

    if (this->m_depth >= this->m_alloc_depth)
        this->grow();
    this->copy_image(data.get(), components);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::Texture3DMerger<F>::grow()
{
    const uint32_t new_depth = this->m_depth + grow_factor(this->m_depth);
    const size_t old_size = alloc_size(this->m_extent, this->m_alloc_depth);
    const size_t new_size = alloc_size(this->m_extent, new_depth);

    component_t* const new_data = new component_t[new_size]{};
    memcpy(new_data, this->m_data, old_size);

    delete[] this->m_data;
    this->m_data = new_data;
    this->m_alloc_depth = new_depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::Texture3DMerger<F>::copy_image(const component_t* data, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height;
    const size_t offset = format_countof(F) * px_count * this->m_depth;
    detail::texture::copy_image<F>(this->m_data + offset, data, px_count, img_comp, 0);
    ++this->m_depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::Texture3DMerger<F>::fill_image(const component_t* color, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height;
    const size_t offset = format_countof(F) * px_count * this->m_depth;
    detail::texture::fill_image<F>(this->m_data + offset, color, px_count, img_comp, 0);
    ++this->m_depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr size_t vka::Texture3DMerger<F>::alloc_size(VkExtent2D extent, uint32_t depth) noexcept
{
    return format_countof(F) * extent.width * extent.height * depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr uint32_t vka::Texture3DMerger<F>::alloc_depth(uint32_t expected_depth) noexcept
{
    return expected_depth < 1 ? 1 : expected_depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr uint32_t vka::Texture3DMerger<F>::grow_factor(uint32_t depth) noexcept
{
    const uint32_t factor = (depth * 3) >> 1; // x1.5
    return factor < 1 ? 1 : factor;
}