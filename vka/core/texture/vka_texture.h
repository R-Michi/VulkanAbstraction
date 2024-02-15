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
        enum State
        {
            STATE_INVALID   = 0,    // 00
            STATE_CREATED   = 1,    // 01
            STATE_LOADING   = 2,    // 10
            STATE_FINISHED  = 3     // 11
        };

    private:
        static constexpr const char IMAGE_CREATE_FAILED[] = "[vka::Texture::create]: Failed to create image handle.";
        static constexpr const char ALLOC_MEMORY_FAILED[] = "[vka::Texture::create]: Failed to allocate memory.";
        static constexpr const char BIND_MEMORY_FAILED[] = "[vka::Texture::create]: Failed to bind memory to image.";
        static constexpr const char VIEW_CREATE_FAILED[] = "[vka::Texture::create]: Failed to create image view.";
        static constexpr const char SAMPLER_CREATE_FAILED[] = "[vka::Texture::create]: Failed to create sampler";
#ifdef VKA_STB_IMAGE_LOAD_ENABLE
        static constexpr const char IMAGE_LOAD_FAILED[] = "[vka::Texture::load_image]: Loading image from file failed.";
#endif

        VkDevice m_device;
        VkImage m_image;
        VkDeviceMemory m_memory;
        VkSampler m_sampler;
        std::vector<VkImageView> m_views;
        VkExtent3D m_extent;
        uint32_t m_level_count;
        uint32_t m_layer_count;
        VkFormat m_format;
        State m_state;

        void destroy_handles(void) noexcept;

        void internal_create(const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info);

        /*
        * Changes the image to the correct layout, before loading data into the image or before
        * generating mip-map levels. If layout has already been changed to the correct layout, this
        * function does nothing.
        * 
        * Changes the image to the correct layout, before loading data into the image or before
        * generating mip-map levels. This specific implementation changes the layout from the
        * 'created' state into the layout required by the 'loading' state.
        */
        inline void change_layout_C2L(VkCommandBuffer cbo) noexcept;

        /*
        * Changes the image to the correct layout, before loading data into the image or before
        * generating mip-map levels. This specific implementation changes the layout from the
        * 'finished' state into the layout required by the 'loading' state.
        */
        inline void change_layout_F2L(VkCommandBuffer cbo) noexcept;

        /*
        * This specific implementation changes the layout from the mip-map creation process into
        * the layout required by the 'finished' state.
        */
        inline void change_layout_M2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        /*
        * This specific implementation changes the layout from the 'created' state into the layout
        * required by the 'finished' state.
        */
        inline void change_layout_C2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        /*
        * This specific implementation changes the layout from the 'loading' state into the layout
        * required by the 'finished' state.
        */
        inline void change_layout_L2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        // Reruns the extent of a given mip level.
        inline VkExtent3D size(uint32_t level) const noexcept;

        /*
        * Executes the commands to create the mip-map levels. A command buffer is required for the
        * execution and is specified by 'cbo'.
        */
        void create_mip_levels(VkCommandBuffer cbo) noexcept;

        // computes the integer logarithm
        static inline uint32_t log2i(uint32_t x) noexcept;

        // returns the maximum integer logarithm of the 3 components
        static inline uint32_t max_log2i(VkExtent3D extent) noexcept;

        // adds an offset to a void pointer
        static inline void* addvp(void* vp, intptr_t offset) noexcept;

        /*
        * This is a wrapper function for stbi_load, stbi_load_16 and stbi_loadf. It decides at
        * compile-time which function is called depending on the template argument.
        * Additionally, it returns the size of the image as VkExtent3D and the number of components
        * as uint32_t instead of an int.
        */
#ifdef VKA_STB_IMAGE_LOAD_ENABLE
        template<ImageDataType Type>
        static inline void* load_image_internal(const char* path, VkExtent3D& extent, uint32_t* components,  uint32_t force_components) noexcept;
#endif

    public:
        /*
        * Initialization constructor which initializes 'this' with a device. The device does not
        * have to be valid at initialization, it must be valid at creation. This constructor has
        * the same functionality as the .init() function. Furthermore, this constructor is also
        * used as the default constructor, where the device is initialized to a VK_NULL_HANDLE.
        * All other handles are initialized to a VK_NULL_HANDLE and every other member variable
        * contains its default initialization.
        */

        inline Texture(void) noexcept;

        explicit inline Texture(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info);

        // Texture is not copyable
        Texture(const Texture&) = delete;
        Texture& operator= (const Texture&) = delete;

        /*
        * Moves another object of Texture into 'this'. 'This' now holds the ownership of all the
        * handles of the source object. The source object will become invalidated and contains its
        * default initialization except for the device. The device will be copied and is preserved
        * in the moved object. If 'this' was created and is a valid object, 'this' is destroyed and
        * replaced by the handles of the moved object.
        */
        inline Texture(Texture&& src) noexcept;
        inline Texture& operator= (Texture&& src) noexcept;

        // The destructor destroys all the vulkan handles.
        virtual inline ~Texture(void);

        /*
        * This function creates the Texture and the internal handles are now valid, if no error
        * occured. If an error occured while creating, a std::runtime_error exception is thrown
        * with an appropriate message about the error. The Texture is created with a
        * TextureCreateInfo structure which is used for the creation of the image and sampler handle.
        * The create-info is specified by 'create_info'. Additionally, the memory properties of the
        * physical device are required and specified by 'properties'. A std::invalid_argument
        * exception is thrown, if 'this' has not been initialized.
        */
        inline void create(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info);

        /*
        * Creates and adds an image view to the texture image, if no error occured. If an error
        * occured while creating, a std::runtime_error exception is thrown with an appropriate
        * message about the error. If the texture has not been created yet (texture object is
        * invalid), this function does nothing.
        * NOTE: If the creation of the image view failed, the view is not added to the texture.
        */
        void create_view(const TextureViewCreateInfo& create_info);

        /*
        * Destroys the Texture object. After destroying, 'this' holds its default initialization
        * except for the device. The device will be preserved after destroying and 'this' does not
        * need to be reinitialized. This is also done by the destructor.
        */
        inline void destroy(void) noexcept;

        /*
        * This function executes the commands to load data from a staging buffer (buffer in host
        * memory) into the texture image. The command buffer to record the commands into is
        * specified by 'cbo', the staging buffer is specified by 'data' and the target array layer
        * of the image is specified by 'layer'. If the given buffer is invalid, this function does
        * nothing. Optionally, an amount of array layers can be specified, if the buffer holds data
        * for multiple array layers. Also, a mip level can optionally be specified by 'level'.
        * By default, the number of array layers is 1 and the mip-map level is 0 (original image).
        * The maximum number of mip-levels can be queried by the function 'level_count()'. 
        */
        void load(VkCommandBuffer cbo, const vka::Buffer& data, uint32_t layer, uint32_t layer_count = 1, uint32_t level = 0) noexcept;

        /*
        * Loads the data provided by 'data' into a staging buffer. The returned staging buffer is
        * specified by 'buffer'. If the Buffer has already been created, it is replaced with the
        * staging buffer. Everything that was previously stored inside the buffer is lost.
        * Additionally, the memory properties of the physical device and the queue family for the
        * staging buffer is required. Optionally, a mip level can be specified by 'level'.
        * 
        * The size of the given data must match the size of the image (in bytes) which is:
        *   'size = width * height * depth * common::format_sizeof(format)'
        * 'Width', 'height' and 'depth' are the dimensions of the image, 'format' is the used
        * format of the image and 'size' is the resulting data size.
        * 
        * If a custom mip-level is specified, then 'width', 'height' and 'depth' are scaled down
        * according to the following formulas:
        *   'w = max(width / 2^level, 1)'
        *   'h = max(height / 2^level, 1)'
        *   'd = max(depth / 2^level, 1)'
        * 'W', 'h' and 'd' are the downscaled image dimensions for the specified mip-level which is
        * represented by 'level'. However, any dimension cannot be 0 and therefore has at least a
        * size of 1. The data size must then be:
        *   'size = w * h * d * common::format_sizeof(format)'.
        * 
        * NOTE: 'common::format_sizeof(format) = components * sizeof(component_type)', where
        * 'components' is the number of components of a pixel of the image and 'component_type' is
        * the type that is used by the color components of a pixel.
        */
        inline void load_staging(const void* data, vka::Buffer& buffer, const VkPhysicalDeviceMemoryProperties& properties, uint32_t qfamidx, uint32_t level = 0) const;

        /*
        * Loads the data from multiple buffers provided by 'data' into one single staging buffer.
        * Every data buffer corresponds to one array layer of the final texture image which are all
        * stored inside one staging buffer. The returned staging buffer is specified by 'buffer'.
        * If the Buffer has already been created, it is replaced with the staging buffer.
        * Everything that was previously stored inside the buffer is lost. Additionally, the memory
        * properties of the physical device and the queue family for the staging buffer is
        * required. Optionally, a mip level can be specified by 'level'.
        * 
        * For the size of the buffers of 'data' see the description of the previous function.
        * It applies to this function as well for every single buffer specified by 'data'.
        * 
        * NOTE: 'common::format_sizeof(format) = components * sizeof(component_type)', where
        * 'components' is the number of components of a pixel of the image and 'component_type' is
        * the type that is used by the color components of a pixel.
        */
        void load_staging(const void* const* data, vka::Buffer& buffer, const VkPhysicalDeviceMemoryProperties& properties, uint32_t qfamidx, uint32_t layer_count, uint32_t level = 0) const;

        /*
        * Finishes the texture creation. This function changes the layout of the image to the final
        * layout required for rendering. Additionally, if mip-map level generation is active, this
        * function also records the commands to generate the mip-map levels. A command buffer is
        * specified by 'cbo' which is used to record all required commands. Furthermore, the
        * (pipeline-) shader stages where the texture is used are specified by 'stages'.
        * NOTE: This function must be called as the last operation when creating a texture.
        */
        void finish(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        /*
        * Finishes the texture creation, if the mip-map levels are specified manually. This function
        * does not generate mip-map levels even, if mip-map generation is activated. It only changes
        * the layout from the image to the final layout required for rendering. A command buffer is
        * specified by 'cbo' which is used to record all requiered commands. Furthermore, the
        * (pipeline-) shader stages where the texture is used are specified by 'stages'.
        * NOTE: This function must be called as the last operation when creating a texture.
        */
        void finish_manual(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        // Returns the extent of the texture.
        inline VkExtent3D size(void) const noexcept;

        /*
        * Returns the number of mip levels of the image. The number of mip levels is at least 1.
        * If the texture has not been created, or is invalid, the return mip level count is 0.
        */
        inline uint32_t level_count(void) const noexcept;
        
        /*
        * Returns the number of mip levels that will be generated from a given extent. The image
        * extent is specified by 'extent'.
        */
        static inline uint32_t level_count(VkExtent3D extent) noexcept;

        // Returns the number of array layers of the image.
        inline uint32_t layer_count(void) const noexcept;

        // Returns the vulkan image handle.
        inline VkImage handle(void) const noexcept;

        // Returns the vulkan sampler handle.
        inline VkSampler sampler(void) const noexcept;

        /*
        * Returns the vulkan image view handle at the specified index 'i'. This function does
        * perform a range check and throws a std::out_of_range exception, if the specified index
        * is greater than or equal to the number of views.
        */
        inline VkImageView view(size_t i) const;

        /*
        * Returns the vulkan image view handle at the specified index 'i'. However, this function
        * does not perform a range check. 
        */
        inline VkImageView viewu(size_t i) const noexcept;

        // Returns true, if the Texture is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;

#ifdef VKA_STB_IMAGE_LOAD_ENABLE
        /*
        * Loads an image from a file. The path to the file is specified by 'path'. The size of the
        * image is returned via the 'extent' and the number of color components/channels of the
        * image is returned via the 'components' parameter. The (address of the-) image's memory is
        * returned. The template argument 'T' specifies the data type of the color components.
        * NOTE: 'extent.depth' is always 1.
        * NOTE: This function can not return nullptr, if VKA_ALLOW_NULL_RETURN is not enabled.
        * Instead, a std::runtime_error exception is thrown which indicates that this operation
        * failed or, if memory allocation failed.
        */
        template<ImageDataType Type>
        static inline void* load_image(const char* path, VkExtent3D& extent, uint32_t& components) noexcept;

        /*
        * Loads an image from a file. The path to the file is specified by 'path'. The size of the
        * image is returned via the 'extent' parameter. The (address of the-) image's memory is
        * returned. The template argument 'T' specifies the data type of the color components and
        * 'force_components' forces the implementation to load a specified number of color
        * components/channels.
        * NOTE: 'extent.depth' is always 1.
        * NOTE: This function can not return nullptr, if VKA_ALLOW_NULL_RETURN is not enabled.
        * Instead, a std::runtime_error exception is thrown which indicates that this operation
        * failed or, if memory allocation failed.
        */
        template<ImageDataType Type, uint32_t force_components>
        static inline void* load_image(const char* path, VkExtent3D& extent) noexcept;

        // Frees allocated image data. Is just a wrapper for the function free().
        static inline void free_image(void* data) noexcept;
#endif
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_texture_impl.inl"
#endif
#include "vka_texture_inline_impl.inl"
