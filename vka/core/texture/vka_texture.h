/**
* @file     vka_texture.h
* @brief    Texture definition file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka
{
    class Texture
    {
    private:
        VkPhysicalDevice _pyhsical_device;
        VkDevice _device;
        VkCommandPool _pool;
        VkQueue _queue;

        VkImageCreateInfo _ici, _backup_ici;
        VkSamplerCreateInfo _sci, _backup_sci;
        std::vector<VkImageViewCreateInfo> _vcis;

        VkImage _image;
        VkDeviceMemory _memory;
        VkSampler _sampler;
        std::vector<VkImageView> _views;
        std::vector<Buffer> _staging_buffers;

        VkMemoryPropertyFlags _properties;
        size_t _mem_size;

        static void init_ici(VkImageCreateInfo& ci);
        static void init_sci(VkSamplerCreateInfo& ci);
        bool validate_buffers(void) const noexcept;
        bool validate_views(void) const noexcept;

    public:
        Texture(void) noexcept;
    
        /**
        * @param[in] physical_device    physical device
        * @param[in] device             logical device
        * @param[in] cmd_pool           command pool for internal commands
        * @param[in] queue              command queue of execution
        */
        Texture(VkPhysicalDevice physical_device, VkDevice device, VkCommandPool cmd_pool, VkQueue queue) noexcept;

        Texture(const Texture&) = delete;
        Texture& operator= (const Texture&) = delete;

        /** 
        * @brief   Move constructor and move operator. 
        * @note    Both the move constructor and move operator
        *          copy all the properties of the texture and transfer
        *          ownership of the texture object's handles.
        *          Therefore, the source texture will become cleared.
        */
        Texture(Texture&& src);
        Texture& operator= (Texture&& src);

        virtual ~Texture(void);

        /** @brief Setter for the parameter of the VkImageCreateInfo. */
        void set_image_flags(VkImageCreateFlags flags) noexcept;
        void set_image_type(VkImageType type) noexcept;
        void set_image_format(VkFormat format) noexcept;
        void set_image_extent(VkExtent3D extent) noexcept;
        void set_image_array_layers(uint32_t layers) noexcept;
        void set_image_queue_families(uint32_t queue_family_index) noexcept;
        void set_image_create_info(const VkImageCreateInfo& create_info) noexcept;

        /** @brief Setter for the parameter of the VkSamplerCreateInfo. */
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

        /** @param[in] physical_device: physical device */
        void set_pyhsical_device(VkPhysicalDevice physical_device) noexcept;

        /** @param[in] device: logical device */
        void set_device(VkDevice device) noexcept;

        /** @param[in] cmd_pool: command pool for internal commands*/
        void set_command_pool(VkCommandPool command_pool) noexcept;

        /** @param[in] queue: command queue of execution */
        void set_queue(VkQueue queue) noexcept;

        /**
         * @brief       Adds an image view to the texture.
         * @param ci    Create info of the image view.
         * @note        VkImageViewCreateInfo::subresourceRange::baseMipLevel,
         *              VkImageViewCreateInfo::subresourceRange::levelCount and
         *              VkImageViewCreateInfo::subresourceRange::aspectMask are unused
         *              and managed by the Texture class.
         */
        void add_view(const VkImageViewCreateInfo& ci);

        /**
         * @brief               Loads an image to a specified array layer.
         * @param array_layer   target array layer
         * @param data          data to the image's pixels
         * @return              result wether the load was successful
         * @note                The extent is equal for every array layer!
         */
        VkResult load(uint32_t array_layer, const void* data);

        /**
         * @brief               Creates the texture and its mip maps.
         * @param mipgen        indicates wether mip maps should be generated.
         * @param mip_filter    filtering between mip maps, is ignored if 'mipgen' is set to false
         * @return              result wether the create was successful (VK_SUCCESS if successful)
         */
        VkResult create(bool mipgen, VkFilter mip_filter);

        /** @brief Cleares the texture. The texture's handles are no longer aviable after a clear. */
        void clear(void);

        /** @return image create info at the time point when the texture was created */
        const VkImageCreateInfo& get_image_create_info(void) const noexcept;

        /** @return sampler create info at the time point when the texture was created */
        const VkSamplerCreateInfo& get_sampler_create_info(void) const noexcept;

        /** @return image view create infos at the time point when the texture was created */
        const std::vector<VkImageViewCreateInfo>& get_view_create_infos(void) const noexcept;

        /** @return image handle */
        VkImage image(void) const noexcept;
    
        /** @return sampler handle */
        VkSampler sampler(void) const noexcept;

        /** @return image view handles */
        const std::vector<VkImageView>& views(void) const noexcept;

        /** @return extent of the texture */
        VkExtent3D extent(void) const noexcept;
    
        /** @return number of pixels */
        uint32_t count(void) const noexcept;

        /** @return allocated memory size of the texture in bytes */
        size_t mem_size(void) const noexcept;
    
        /** @return number of mipmap generated levels */
        uint32_t mip_levels(void) const noexcept;
    
        /** @return number of array layers specified in the create info */
        uint32_t array_layers(void) const noexcept;

        /** @return Boolean wether the texture has already been created. */
        bool is_created(void) const noexcept;

        static uint32_t to_mip_levels(const VkExtent3D& extent);
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_texture_impl.inl"
#endif
