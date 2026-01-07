// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "texture_top.h"

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureLoader<F>::TextureLoader(const component_t* data, VkFormat format, VkExtent3D extent, uint32_t expected_layers) :
    m_data(new component_t[alloc_size(extent, alloc_layers(expected_layers))]{}),
    m_alloc_layers(alloc_layers(expected_layers)),
    m_extent(extent),
    m_layer_count(0)
{
    this->copy_image(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureLoader<F>::TextureLoader(const component_t* color, uint32_t comp, VkExtent3D extent, uint32_t expected_layers) :
    m_data(new component_t[alloc_size(extent, alloc_layers(expected_layers))]{}),
    m_alloc_layers(alloc_layers(expected_layers)),
    m_extent(extent),
    m_layer_count(0)
{
    this->fill_image(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureLoader<F>::TextureLoader(const char* path, uint32_t expected_layers) :
    m_data(nullptr),
    m_alloc_layers(alloc_layers(expected_layers)),
    m_extent{},
    m_layer_count(0)
{
    int32_t width, height, components;
    std::unique_ptr<component_t[]> data(load_f(path, &width, &height, &components, 0));
    const VkExtent3D extent = { (uint32_t)width, (uint32_t)height, 1 };

    if (data == nullptr) [[unlikely]]
        detail::error::throw_invalid_argument(MSG_INVALID_PATH);

    this->m_data = new component_t[alloc_size(extent, this->m_alloc_layers)]{};
    this->m_extent = extent;
    this->copy_image(data.get(), components);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureLoader<F>::TextureLoader(TextureLoader&& src) noexcept :
    m_data(src.m_data),
    m_alloc_layers(src.m_alloc_layers),
    m_extent(src.m_extent),
    m_layer_count(src.m_layer_count)
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
    this->m_layer_count = src.m_layer_count;
    src.m_data = nullptr;
    return *this;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline const vka::TextureLoader<F>::component_t* vka::TextureLoader<F>::data() const noexcept
{
    return this->m_data;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline VkExtent3D vka::TextureLoader<F>::extent() const noexcept
{
    return this->m_extent;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline uint32_t vka::TextureLoader<F>::layer_count() const noexcept
{
    return this->m_layer_count;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureLoader<F>::load(const component_t* data, VkFormat format) noexcept
{
    if (this->m_layer_count >= this->m_alloc_layers)
        this->grow();
    this->copy_image(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureLoader<F>::load(const component_t* color, uint32_t comp) noexcept
{
    if (this->m_layer_count >= this->m_alloc_layers)
        this->grow();
    this->fill_image(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureLoader<F>::load(const char* path)
{
    int32_t width, height, components;
    std::unique_ptr<component_t[]> data(load_f(path, &width, &height, &components, 0));
    const VkExtent3D extent = { (uint32_t)width, (uint32_t)height, 1 };

    if (data == nullptr) [[unlikely]]
        detail::error::throw_invalid_argument(MSG_INVALID_PATH);
    if (!detail::common::cmpeq_extent(this->m_extent, extent)) [[unlikely]]
        detail::error::throw_runtime_error(MSG_EXTENT_MISSMATCH);

    if (this->m_layer_count >= this->m_alloc_layers)
        this->grow();
    this->copy_image(data.get(), components);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureLoader<F>::grow()
{
    const uint32_t new_layers = this->m_layer_count + grow_factor(this->m_layer_count);
    const size_t old_size = alloc_size(this->m_extent, this->m_alloc_layers);
    const size_t new_size = alloc_size(this->m_extent, new_layers);

    component_t* const new_data = new component_t[new_size]{};
    memcpy(new_data, this->m_data, old_size);

    delete[] this->m_data;
    this->m_data = new_data;
    this->m_alloc_layers = new_layers;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureLoader<F>::copy_image(const component_t* data, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    const size_t offset = format_countof(F) * px_count * this->m_layer_count;
    detail::texture::copy_image<F>(this->m_data + offset, data, px_count, img_comp, 0);
    ++this->m_layer_count;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureLoader<F>::fill_image(const component_t* color, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    const size_t offset = format_countof(F) * px_count * this->m_layer_count;
    detail::texture::fill_image<F>(this->m_data + offset, color, px_count, img_comp, 0);
    ++this->m_layer_count;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr size_t vka::TextureLoader<F>::alloc_size(VkExtent3D extent, uint32_t layer_count) noexcept
{
    return format_countof(F) * extent.width * extent.height * extent.depth * layer_count;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr uint32_t vka::TextureLoader<F>::alloc_layers(uint32_t expected_layers) noexcept
{
    return expected_layers < 1 ? 1 : expected_layers;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr uint32_t vka::TextureLoader<F>::grow_factor(uint32_t layer_count) noexcept
{
    const uint32_t factor = (layer_count * 3) >> 1; // x1.5
    return factor < 1 ? 1 : factor;
}