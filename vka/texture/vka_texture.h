/**
* @file     vka_texture.h
* @brief    Texture definition file.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../vka_global.h"

namespace vka
{
    class Texture
    {
    private:
        VkPhysicalDevice _physical_device;
        VkDevice _device;
        VkCommandPool _command_pool;
        VkQueue _command_queue;
    
        VkImageCreateInfo _image_create_info;
        VkImageViewCreateInfo _view_create_info;
        VkSamplerCreateInfo _sampler_create_info;
    
        VkImage _image;
        VkDeviceMemory _memory;
        VkImageView _view;
        VkSampler _sampler;
        bool _mipmap;
        VkFilter _mipfilter;
    
        size_t _size;
        size_t _px_count;
    
        void _default_img_create_info(void) noexcept;
        void _default_view_create_info(void) noexcept;
        void _default_sampler_create_info(void) noexcept;
        
        /** @brief Begins command buffer recording. */
        VkResult record_commands(VkCommandBuffer& command_buffer);
        
        /** @brief Executes command buffer.  */
        VkResult execute_commands(VkCommandBuffer command_buffer);
        
        /** @brief Copies a buffer into an image. */
        void buffer_copy(VkCommandBuffer command_buffer, VkBuffer buffer, VkImage image, uint32_t array_layers, VkExtent3D extent);
        
        /** @brief Performs an image layout transision. */
        void transform_image_layout(VkCommandBuffer command_buffer, VkImageLayout _old, VkImageLayout _new);
        
        /** @brief Generates the mipmaps. */
        void generate_mipmaps(VkCommandBuffer command_buffer);
    
    public:
        Texture(void) noexcept;
        
        /**
        * @param[in] physical_device: physical device
        * @param[in] device: logical device
        * @param[in] cmd_pool: command pool for internal commands
        * @param[in] queue: command queue of execution
        */
        Texture(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool, VkQueue queue) noexcept;
        
        Texture(const Texture&) = delete;
        Texture& operator= (const Texture&) = delete;
        
        Texture(Texture&&) = delete;
        Texture& operator= (Texture&&) = delete;
        
        virtual ~Texture(void);
        
        /**
        * @brief Setter for the parameter of the VkImageCreateInfo.
        */
        void set_image_flags(VkImageCreateFlags flags) noexcept;
        void set_image_type(VkImageType type) noexcept;
        void set_image_format(VkFormat format) noexcept;
        void set_image_extent(VkExtent3D extent) noexcept;
        void set_image_array_layers(uint32_t layers) noexcept;
        void set_image_queue_families(uint32_t queue_family_index) noexcept;
        void set_image_create_info(const VkImageCreateInfo& create_info) noexcept;
        
        /**
        * @brief Setter for the parameter of the VkImageViewCreateInfo.
        */
        void set_view_type(VkImageViewType type) noexcept;
        void set_view_format(VkFormat format) noexcept;
        void set_view_components(VkComponentMapping component_mapping) noexcept;
        void set_view_subresource_range(VkImageSubresourceRange subressource_range) noexcept;
        void set_view_create_info(const VkImageViewCreateInfo& create_info) noexcept;
        
        /**
        * @brief Setter for the parameter of the VkSamplerCreateInfo.
        */
        void set_sampler_mag_filter(VkFilter mag_filter) noexcept;
        void set_sampler_min_filter(VkFilter min_filter) noexcept;
        void set_sampler_mipmap_mode(VkSamplerMipmapMode mode) noexcept;
        void set_sampler_address_mode(VkSamplerAddressMode u, VkSamplerAddressMode v, VkSamplerAddressMode w) noexcept;
        void set_sampler_mip_lod_bias(float bias) noexcept;
        void set_sampler_anisotropy_enable(bool enable) noexcept;
        void set_sampler_max_anisotropy(float max) noexcept;
        void set_sampler_compare_enable(bool enable) noexcept;
        void set_sampler_compare_op(VkCompareOp op) noexcept;
        void set_sampler_lod(float min_lod, float max_lod) noexcept;
        void set_sampler_border_color(VkBorderColor border_color) noexcept;
        void set_sampler_unnormalized_coordinates(bool unnormalized) noexcept;
        void set_sampler_create_info(const VkSamplerCreateInfo& create_info) noexcept;
        
        /** @brief Sets wether mipmaps should be generated or not. */
        void mipmap_generate(bool generate) noexcept;
        
        /** @brief Sets the mipmap filter. */
        void mipmap_filter(VkFilter filter) noexcept;
        
        /** @param[in] physical_device: physical device */
        void set_pyhsical_device(VkPhysicalDevice physical_device) noexcept;
        
        /** @param[in] device: logical device */
        void set_device(VkDevice device) noexcept;
        
        /** @param[in] cmd_pool: command pool for internal commands*/
        void set_command_pool(VkCommandPool command_pool) noexcept;
        
        /** @param[in] queue: command queue of execution */
        void set_queue(VkQueue queue) noexcept; 
        
        /**
        * @brief Creates the texture object.
        * @param[in] pdata: pointer to pixel data
        * @param[in] pixel_stride: size of one pixel in bytes
        * @return vulkan result
        */
        VkResult create(const void* pdata, size_t pixel_stride);
        
        /** @brief Cleares the texture. The texture is no longer aviable after a clear. */
        void clear(void);
        
        /** @return image view handle */
        VkImageView view(void) const noexcept;
        
        /** @return sample handle */
        VkSampler sampler(void) const noexcept;
        
        /** @return size of the texture in bytes */
        size_t size(void) const noexcept;
        
        /** @return number of pixels */
        size_t count(void) const noexcept;
        
        /** @return number of mipmap levels */
        uint32_t mip_levels(void) const noexcept;
        
        /** @return number of array layers */
        uint32_t array_layers(void) const noexcept;
    };
}