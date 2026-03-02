// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "texture_top.h"

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureLoader<F>::TextureLoader(VkExtent3D extent) noexcept :
    m_data(new component_t[alloc_size(extent)]{}),
    m_extent(extent),
    m_alloc_layers(extent.depth)
{}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureLoader<F>::TextureLoader(VkExtent2D extent, uint32_t estimated_layers) noexcept :
    m_data(new component_t[alloc_size({ extent.width, extent.height, alloc_layers(estimated_layers) })]{}),
    m_extent({ extent.width, extent.height, 0 }),
    m_alloc_layers(alloc_layers(estimated_layers))
{}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureLoader<F>::TextureLoader(const component_t* data, VkFormat format, VkExtent2D extent, uint32_t estimated_layers) :
    TextureLoader(extent, estimated_layers)
{
    this->copy_image2D(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureLoader<F>::TextureLoader(const component_t* data, VkFormat format, VkExtent3D extent) :
    TextureLoader(extent)
{
    this->copy_image3D(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureLoader<F>::TextureLoader(const component_t* color, uint32_t comp, VkExtent2D extent, uint32_t estimated_layers) :
    TextureLoader(extent, estimated_layers)
{
    this->fill_image2D(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureLoader<F>::TextureLoader(const component_t* color, uint32_t comp, VkExtent3D extent) :
    TextureLoader(extent)
{
    this->fill_image3D(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureLoader<F>::TextureLoader(const char* path, uint32_t estimated_layers) :
    m_data(nullptr),
    m_extent{},
    m_alloc_layers(alloc_layers(estimated_layers))
{
    VkExtent2D extent; uint32_t components;
    std::unique_ptr<component_t[]> data =  detail::texture::load<F>(path, extent, components);
    const VkExtent3D alloc_extent = { extent.width, extent.height, this->m_alloc_layers };

    this->m_data = new component_t[alloc_size(alloc_extent)]{};
    this->m_extent = { extent.width, extent.height, 0 };
    this->copy_image2D(data.get(), components);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureLoader<F>::TextureLoader(TextureLoader&& src) noexcept :
    m_data(src.m_data),
    m_extent(src.m_extent),
    m_alloc_layers(src.m_alloc_layers)
{
    src.m_data = nullptr;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureLoader<F>::~TextureLoader()
{
    delete[] this->m_data;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureLoader<F>& vka::TextureLoader<F>::operator= (TextureLoader&& src) noexcept
{
    delete[] this->m_data;
    this->m_data = src.m_data;
    this->m_alloc_layers = src.m_alloc_layers;
    this->m_extent = src.m_extent;
    src.m_data = nullptr;
    return *this;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline const vka::TextureLoader<F>::component_t* vka::TextureLoader<F>::data() const noexcept
{
    return this->m_data;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline VkExtent2D vka::TextureLoader<F>::extent2D() const noexcept
{
    return { this->m_extent.width, this->m_extent.height };
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline VkExtent3D vka::TextureLoader<F>::extent3D() const noexcept
{
    return this->m_extent;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline uint32_t vka::TextureLoader<F>::layer_count() const noexcept
{
    return this->m_extent.depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureLoader<F>::load(const component_t* data, VkFormat format) noexcept
{
    if (this->m_extent.depth == this->m_alloc_layers)
        this->grow();
    this->copy_image2D(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureLoader<F>::load(const component_t* color, uint32_t comp) noexcept
{
    if (this->m_extent.depth == this->m_alloc_layers)
        this->grow();
    this->fill_image2D(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureLoader<F>::load(const char* path)
{
    VkExtent2D extent; uint32_t components;
    std::unique_ptr<component_t[]> data =  detail::texture::load<F>(path, extent, components);
    if (!detail::common::cmpeq_extent(this->m_extent, extent)) [[unlikely]]
        detail::error::throw_runtime_error(MSG_EXTENT_MISSMATCH);

    if (this->m_extent.depth == this->m_alloc_layers)
        this->grow();
    this->copy_image2D(data.get(), components);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureLoader<F>::grow()
{
    const uint32_t new_layers = this->m_extent.depth + grow_factor(this->m_extent.depth);
    const size_t old_size = alloc_size(this->m_extent);
    const size_t new_size = alloc_size({ this->m_extent.width, this->m_extent.height, new_layers });

    component_t* const new_data = new component_t[new_size]{};
    memcpy(new_data, this->m_data, old_size);

    delete[] this->m_data;
    this->m_data = new_data;
    this->m_alloc_layers = new_layers;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureLoader<F>::copy_image2D(const component_t* data, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    const size_t offset = format_countof(F) * px_count;
    detail::texture::copy_image<F>(this->m_data + offset, data, px_count, img_comp, 0);
    ++this->m_extent.depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureLoader<F>::copy_image3D(const component_t* data, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    detail::texture::copy_image<F>(this->m_data, data, px_count, img_comp, 0);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureLoader<F>::fill_image2D(const component_t* color, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    const size_t offset = format_countof(F) * px_count;
    detail::texture::fill_image<F>(this->m_data + offset, color, px_count, img_comp, 0);
    ++this->m_extent.depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureLoader<F>::fill_image3D(const component_t* color, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    detail::texture::fill_image<F>(this->m_data, color, px_count, img_comp, 0);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr size_t vka::TextureLoader<F>::alloc_size(VkExtent3D extent) noexcept
{
    return format_sizeof(F) * extent.width * extent.height * extent.depth;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr uint32_t vka::TextureLoader<F>::alloc_layers(uint32_t estimated_layers) noexcept
{
    return estimated_layers < 1 ? 1 : estimated_layers;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr uint32_t vka::TextureLoader<F>::grow_factor(uint32_t layer_count) noexcept
{
    const uint32_t factor = (layer_count * 3) >> 1; // x1.5
    return factor < 1 ? 1 : factor;
}