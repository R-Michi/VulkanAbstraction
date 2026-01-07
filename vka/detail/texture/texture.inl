#pragma once

#include "texture.h"

inline void vka::detail::texture::destroy(VkDevice device, const Handle& handle, const VkAllocationCallbacks* allocator)
{
    for (uint32_t i = 0; i < handle.view_count; i++)
        vkDestroyImageView(device, handle.views[i], allocator);
    delete[] handle.views;

    vkDestroySampler(device, handle.sampler, allocator);
    vkFreeMemory(device, handle.memory, allocator);
    vkDestroyImage(device, handle.image, allocator);
}

consteval bool vka::detail::texture::is_format_contained(VkFormat format, const VkFormat* formats, uint32_t count) noexcept
{
    for (uint32_t i = 0; i < count; i++)
    {
        if (formats[i] == format)
            return true;
    }
    return false;
}

consteval uint8_t vka::detail::texture::format_type_id(VkFormat format) noexcept
{
    if (is_format_contained(format, LOADER_FORMAT_U8, 4)) return 0;
    if (is_format_contained(format, LOADER_FORMAT_U16, 4)) return 1;
    if (is_format_contained(format, LOADER_FORMAT_FLOAT, 4)) return 2;
    return 0xFF;
}

template<VkFormat F>
void vka::detail::texture::copy_image(loader_format_t<F>* dst, const loader_format_t<F>* src, uint64_t px_count, uint32_t img_comp, uint32_t comp_offset) noexcept
{
    constexpr size_t max_comp = format::format_countof(F);
    const uint32_t rem_comp = max_comp - comp_offset;
    for (uint64_t i = 0; i < px_count; i++)
    {
        const uint64_t offset_src = i * img_comp;
        const uint64_t offset_dst = i * max_comp + comp_offset;
        for (uint32_t c = 0; c < img_comp && c < rem_comp; c++)
            dst[offset_dst + c] = src[offset_src + c];
    }
}

template<VkFormat F>
void vka::detail::texture::fill_image(loader_format_t<F>* dst, const loader_format_t<F>* color, uint64_t px_count, uint32_t img_comp, uint32_t comp_offset) noexcept
{
    constexpr size_t max_comp = format::format_countof(F);
    const uint32_t rem_comp = max_comp - comp_offset;
    for (uint64_t i = 0; i < px_count; i++)
    {
        const uint64_t offset_dst = i * max_comp + comp_offset;
        for (uint32_t c = 0; c < img_comp && c < rem_comp; c++)
            dst[offset_dst + c] = color[c];
    }
}