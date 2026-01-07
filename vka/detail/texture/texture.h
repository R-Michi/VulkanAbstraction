/**
 * @brief Includes implementation details for textures.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::detail::texture
{
    /// Maps from format type-ID to the format type.
    template<uint8_t FID> requires (FID <= 2)
    struct loader_format_type {};
    template<> struct loader_format_type<0> { using type = uint8_t;  };
    template<> struct loader_format_type<1> { using type = uint16_t; };
    template<> struct loader_format_type<2> { using type = float;    };

#ifdef VKA_STB_ENABLE
    /// Maps form format type-ID to the load function for the format.
    template<uint8_t FID> requires (FID <= 2)
    struct loader_function {};
    template<> struct loader_function<0>    { static constexpr auto func = stbi_load;    };
    template<> struct loader_function<1>    { static constexpr auto func = stbi_load_16; };
    template<> struct loader_function<2>    { static constexpr auto func = stbi_loadf;   };
#endif

    /// Specifies all uint8_t formats for the texture loader.
    static constexpr VkFormat LOADER_FORMAT_U8[4] = {
        VK_FORMAT_R8_UINT,
        VK_FORMAT_R8G8_UINT,
        VK_FORMAT_R8G8B8_UINT,
        VK_FORMAT_R8G8B8A8_UINT
    };

    /// Specifies all uint16_t formats for the texture loader.
    static constexpr VkFormat LOADER_FORMAT_U16[4] = {
        VK_FORMAT_R16_UINT,
        VK_FORMAT_R16G16_UINT,
        VK_FORMAT_R16G16B16_UINT,
        VK_FORMAT_R16G16B16A16_UINT
    };

    /// Specifies all float formats for the texture loader.
    static constexpr VkFormat LOADER_FORMAT_FLOAT[4] = {
        VK_FORMAT_R32_SFLOAT,
        VK_FORMAT_R32G32_SFLOAT,
        VK_FORMAT_R32G32B32_SFLOAT,
        VK_FORMAT_R32G32B32A32_SFLOAT
    };

    /// Checks if a format is contained in an array.
    consteval bool is_format_contained(VkFormat format, const VkFormat* formats, uint32_t count) noexcept;

    /// 0: uint8_t, 1: uint16_t, 2: float
    consteval uint8_t format_type_id(VkFormat format) noexcept;

    /// Concept whether the format can be used to load images.
    template<VkFormat F>
    concept is_loader_format =  is_format_contained(F, LOADER_FORMAT_U8, 4)     ||
                                is_format_contained(F, LOADER_FORMAT_U16, 4)    ||
                                is_format_contained(F, LOADER_FORMAT_FLOAT, 4);

    /// Data-type of the format.
    template<VkFormat F>
    using loader_format_t = loader_format_type<format_type_id(F)>::type;

#ifdef VKA_STB_ENABLE
    /// Loader function for the format.
    template<VkFormat F>
    constexpr auto loader_f = loader_function<format_type_id(F)>::func;
#endif

    /// Defines the texture handle which is used as a custom handle in unique_handle. Parent: VkDevice.
    struct Handle
    {
        VkImage image;
        VkDeviceMemory memory;
        VkSampler sampler;
        const VkImageView* views;
        uint32_t view_count;

        explicit constexpr operator bool() const noexcept { return image != VK_NULL_HANDLE; }
    };

    /// Destroys the texture handle.
    inline void destroy(VkDevice device, const Handle& handle, const VkAllocationCallbacks* allocator);

    /// Copies image data from src to dst.
    template<VkFormat F>
    void copy_image(loader_format_t<F>* dst, const loader_format_t<F>* src, uint64_t px_count, uint32_t img_comp, uint32_t comp_offset) noexcept;

    /// Fills the image with a single color.
    template<VkFormat F>
    void fill_image(loader_format_t<F>* dst, const loader_format_t<F>* color, uint64_t px_count, uint32_t img_comp, uint32_t comp_offset) noexcept;
}
