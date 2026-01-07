// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "texture_top.h"

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureComponentMerger<F>::TextureComponentMerger(const component_t* data, VkFormat format, VkExtent3D extent) :
    m_data(new component_t[alloc_size(extent)]{}),
    m_extent(extent),
    m_component_idx(0)
{
    this->copy_image(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureComponentMerger<F>::TextureComponentMerger(const component_t* color, uint32_t comp, VkExtent3D extent) :
    m_data(new component_t[alloc_size(extent)]{}),
    m_extent(extent),
    m_component_idx(0)
{
    this->fill_image(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureComponentMerger<F>::TextureComponentMerger(const char* path) :
    m_data(nullptr),
    m_extent{},
    m_component_idx(0)
{
    int32_t width, height, components;
    std::unique_ptr<component_t[]> data(loader_f(path, &width, &height, &components, 0));
    const VkExtent3D extent = { (uint32_t)width, (uint32_t)height, 1 };

    if (data == nullptr) [[unlikely]]
        detail::error::throw_invalid_argument(MSG_INVALID_PATH);

    this->m_data = new component_t[alloc_size(extent)]{};
    this->m_extent = extent;
    this->copy_image(data.get(), components);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureComponentMerger<F>::TextureComponentMerger(TextureComponentMerger&& src) noexcept :
    m_data(src.m_data),
    m_extent(src.m_extent),
    m_component_idx(0)
{
    src.m_data = nullptr;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureComponentMerger<F>::~TextureComponentMerger()
{
    delete[] this->m_data;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureComponentMerger<F>& vka::TextureComponentMerger<F>::operator= (TextureComponentMerger&& src) noexcept
{
    delete[] this->m_data;
    this->m_data = src.m_data;
    this->m_extent = src.m_extent;
    this->m_component_idx = src.m_component_idx;
    src.m_data = nullptr;
    return *this;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline const vka::TextureComponentMerger<F>::component_t* vka::TextureComponentMerger<F>::data() const noexcept
{
    return this->m_data;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline VkExtent3D vka::TextureComponentMerger<F>::extent() const noexcept
{
    return this->m_extent;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureComponentMerger<F>::load(const component_t* data, VkFormat format) noexcept
{
    if (this->m_component_idx < format_countof(F))
        this->copy_image(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureComponentMerger<F>::load(const component_t* color, uint32_t comp) noexcept
{
    if (this->m_component_idx < format_countof(F))
        this->fill_image(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureComponentMerger<F>::load(const char* path)
{
    if (this->m_component_idx < format_countof(F))
    {
        int32_t width, height, components;
        std::unique_ptr<component_t[]> data(loader_f(path, &width, &height, &components, 0));
        const VkExtent3D extent = { (uint32_t)width, (uint32_t)height, 1 };

        if (data == nullptr) [[unlikely]]
            detail::error::throw_invalid_argument(MSG_INVALID_PATH);
        if (!detail::common::cmpeq_extent(this->m_extent, extent)) [[unlikely]]
            detail::error::throw_runtime_error(MSG_EXTENT_MISSMATCH);

        this->copy_image(data.get(), components);
    }
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureComponentMerger<F>::copy_image(const component_t* data, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    detail::texture::copy_image<F>(this->m_data, data, px_count, img_comp, this->m_component_idx);
    this->m_component_idx += img_comp;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureComponentMerger<F>::fill_image(const component_t* color, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    detail::texture::fill_image<F>(this->m_data, color, px_count, img_comp, this->m_component_idx);
    this->m_component_idx += img_comp;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr size_t vka::TextureComponentMerger<F>::alloc_size(VkExtent3D extent) noexcept
{
    return format_countof(F) * extent.width * extent.height * extent.depth;
}
