/**
 * @brief Inline implementation for the texture class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

#include "texture_top.h"

constexpr vka::Texture::Texture() noexcept :
    m_extent{0, 0, 0},
    m_layer_count(0),
    m_level_count(0)
{}

constexpr VkImageView vka::Texture::operator[] (uint32_t idx) const noexcept
{
    return this->m_texture.get().views[idx];
}

constexpr vka::Texture::operator bool() const noexcept
{
    return (bool)this->m_texture;
}

constexpr VkExtent3D vka::Texture::size() const noexcept
{
    return this->m_extent;
}

constexpr uint32_t vka::Texture::view_count() const noexcept
{
    return this->m_texture.get().view_count;
}

constexpr uint32_t vka::Texture::layer_count() const noexcept
{
    // Valid values are in the range [1, 65536]. Therefore, the range [0, 65535] is converted to [1, 65536].
    return (uint32_t)this->m_layer_count + 1;
}

constexpr uint32_t vka::Texture::level_count() const noexcept
{
    return this->m_level_count;
}

constexpr uint32_t vka::Texture::level_count(VkExtent3D extent) noexcept
{
    return detail::common::max_ilog2(extent) + 1;
}

constexpr float vka::Texture::max_lod(VkExtent3D extent) noexcept
{
    return (float)detail::common::max_ilog2(extent);
}

constexpr VkImage vka::Texture::image() const noexcept
{
    return this->m_texture.get().image;
}

constexpr VkSampler vka::Texture::sampler() const noexcept
{
    return this->m_texture.get().sampler;
}

constexpr VkDevice vka::Texture::parent() const noexcept
{
    return this->m_texture.parent();
}

constexpr VkImageView vka::Texture::view(uint32_t idx) const
{
    if (idx >= this->m_texture.get().view_count) [[unlikely]]
        detail::error::throw_out_of_range(VIEW_OUT_OF_RANGE);
    return this->m_texture.get().views[idx];
}

#ifdef VKA_STB_ENABLE

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
[[nodiscard]]
vka::Buffer vka::Texture::load(VkCommandBuffer cbo, const TextureComponentMerger<F>& loader, TextureLoadInfo info, uint32_t layer, uint32_t level)
{
    const VkExtent3D extent = loader.extent();
    const VkDeviceSize size = extent.width * extent.height * extent.width * format_sizeof(F);
    Buffer staging = stage(this->m_texture.parent(), loader.data(), size, info);
    this->load(cbo, staging, layer, 1, level);
    return staging;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
[[nodiscard]]
vka::Buffer vka::Texture::load(VkCommandBuffer cbo, const Texture3DMerger<F>& loader, TextureLoadInfo info, uint32_t layer, uint32_t level)
{
    const VkExtent3D extent = loader.extent();
    const VkDeviceSize size = extent.width * extent.height * extent.depth * format_sizeof(F);
    Buffer staging = stage(this->m_texture.parent(), loader.data(), size, info);
    this->load(cbo, staging, layer, 1, level);
    return staging;
}

template<VkFormat F> requires vka::detail::texture::is_loader_format<F>
[[nodiscard]]
vka::Buffer vka::Texture::load(VkCommandBuffer cbo, const TextureLoader<F>& loader, TextureLoadInfo info, uint32_t layer, uint32_t level)
{
    const VkExtent3D extent = loader.extent();
    const VkDeviceSize size = extent.width * extent.height * extent.depth * loader.layer_count() * format_sizeof(F);
    Buffer staging = stage(this->m_texture.parent(), loader.data(), size, info);
    this->load(cbo, staging, layer, loader.layer_count(), level);
    return staging;
}

#endif

inline uint32_t vka::Texture::mip_level_count(const TextureCreateInfo& create_info) noexcept
{
    return create_info.generateMipMap ? level_count(create_info.imageExtent) : 1;
}