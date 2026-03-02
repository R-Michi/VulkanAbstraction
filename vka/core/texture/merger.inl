// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "texture_top.h"

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureMerger<F>::TextureMerger(VkExtent3D extent) :
    m_data(new component_t[alloc_size(extent)]{}),
    m_extent(extent),
    m_component_idx(0)
{}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureMerger<F>::TextureMerger(const component_t* data, VkFormat format, VkExtent3D extent) : TextureMerger(extent)
{
    this->copy_image(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureMerger<F>::TextureMerger(const component_t* color, uint32_t comp, VkExtent3D extent) : TextureMerger(extent)
{
    this->fill_image(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
vka::TextureMerger<F>::TextureMerger(const char* path) :
    m_data(nullptr),
    m_extent{},
    m_component_idx(0)
{
    VkExtent2D tmp_extent; uint32_t components;
    std::unique_ptr<component_t[]> data =  detail::texture::load<F>(path, tmp_extent, components);
    const VkExtent3D extent = { tmp_extent.width, tmp_extent.height, 1 };

    this->m_data = new component_t[alloc_size(extent)]{};
    this->m_extent = extent;
    this->copy_image(data.get(), components);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureMerger<F>::TextureMerger(TextureMerger&& src) noexcept :
    m_data(src.m_data),
    m_extent(src.m_extent),
    m_component_idx(0)
{
    src.m_data = nullptr;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureMerger<F>::~TextureMerger()
{
    delete[] this->m_data;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline vka::TextureMerger<F>& vka::TextureMerger<F>::operator= (TextureMerger&& src) noexcept
{
    delete[] this->m_data;
    this->m_data = src.m_data;
    this->m_extent = src.m_extent;
    this->m_component_idx = src.m_component_idx;
    src.m_data = nullptr;
    return *this;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline const vka::TextureMerger<F>::component_t* vka::TextureMerger<F>::data() const noexcept
{
    return this->m_data;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline VkExtent3D vka::TextureMerger<F>::extent() const noexcept
{
    return this->m_extent;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline VkDeviceSize vka::TextureMerger<F>::size() const noexcept
{
    return alloc_size(this->m_extent);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureMerger<F>::load(const component_t* data, VkFormat format) noexcept
{
    if (this->m_component_idx < format_countof(F))
        this->copy_image(data, format_countof(format));
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureMerger<F>::load(const component_t* color, uint32_t comp) noexcept
{
    if (this->m_component_idx < format_countof(F))
        this->fill_image(color, comp);
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
void vka::TextureMerger<F>::load(const char* path)
{
    if (this->m_component_idx < format_countof(F))
    {
        VkExtent2D extent; uint32_t components;
        std::unique_ptr<component_t[]> data =  detail::texture::load<F>(path, extent, components);
        if (!detail::common::cmpeq_extent(this->m_extent, extent) || this->m_extent.depth != 1) [[unlikely]]
            detail::error::throw_runtime_error(MSG_EXTENT_MISSMATCH);

        this->copy_image(data.get(), components);
    }
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureMerger<F>::copy_image(const component_t* data, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    detail::texture::copy_image<F>(this->m_data, data, px_count, img_comp, this->m_component_idx);
    this->m_component_idx += img_comp;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
inline void vka::TextureMerger<F>::fill_image(const component_t* color, uint32_t img_comp) noexcept
{
    const uint64_t px_count = (uint64_t)this->m_extent.width * this->m_extent.height * this->m_extent.depth;
    detail::texture::fill_image<F>(this->m_data, color, px_count, img_comp, this->m_component_idx);
    this->m_component_idx += img_comp;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
constexpr size_t vka::TextureMerger<F>::alloc_size(VkExtent3D extent) noexcept
{
    return format_sizeof(F) * extent.width * extent.height * extent.depth;
}
