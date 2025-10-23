/**
 * @brief Helper class for creating textures.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

// ReSharper disable CppRedundantInlineSpecifier
#pragma once

namespace vka
{
    class Texture final
    {
        enum State : uint8_t
        {
            STATE_INVALID   = 0,    // 00
            STATE_CREATED   = 1,    // 01
            STATE_LOADING   = 2,    // 10
            STATE_FINISHED  = 3     // 11
        };

    private:
        static constexpr char IMAGE_CREATE_FAILED[] = "[vka::Texture::create]: Failed to create image handle.";
        static constexpr char ALLOC_MEMORY_FAILED[] = "[vka::Texture::create]: Failed to allocate memory.";
        static constexpr char BIND_MEMORY_FAILED[] = "[vka::Texture::create]: Failed to bind memory to image.";
        static constexpr char VIEW_CREATE_FAILED[] = "[vka::Texture::create]: Failed to create image view.";
        static constexpr char SAMPLER_CREATE_FAILED[] = "[vka::Texture::create]: Failed to create sampler";
#ifdef VKA_STB_IMAGE_LOAD_ENABLE
        static constexpr char IMAGE_LOAD_FAILED[] = "[vka::Texture::load_image]: Loading image from file failed.";
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

        /// @brief Destroys all created vulkan handles.
        void destroy_handles() const noexcept;

        /**
         * @brief Creates the texture and allocates its memory.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the creation info for the texture.
         * @throw std::runtime_error Is thrown, if creating the texture image or the sampler, allocating the memory or
         * binding the memory failed.
         */
        void internal_create(const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info);

        /**
         * @brief Changes the image layout from the created state to the loading state.
         * @param cbo Specifies a command buffer in which to record the layout change.
         * @details The loading state includes loading data into the image buffer and creating mip-map levels. If the
         * image already has the correct layout, this function has no effect. Loading data into the image or creating
         * mip-map levels after the texture has been created requires calling this function.
         */
        inline void change_layout_C2L(VkCommandBuffer cbo) noexcept;

        /**
         * @brief Changes the image layout from the finished state to the loading state.
         * @param cbo Specifies a command buffer in which to record the layout change.
         * @details The loading state includes loading data into the image buffer and creating mip-map levels. If the
         * image already has the correct layout, this function has no effect. Loading data into the image or creating
         * mip-map levels after the texture has been finished requires calling this function.
         */
        inline void change_layout_F2L(VkCommandBuffer cbo) noexcept;

        /**
         * @brief Changes the image layout from the mip-map creation state to the finished state.
         * @param cbo Specifies a command buffer in which to record the layout changes.
         * @param stages Specifies the pipeline shader stages where the texture is used.
         * @details The mip-map creation state is an intermediate state, which is a result of required layout changes
         * during the mip-map creation process. Finishing the texture after creating mip-map levels requires calling
         * this function.
         */
        inline void change_layout_M2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        /**
         * @brief Changes the image layout from the created state to the finished state.
         * @param cbo Specifies a command buffer in which to record the layout changes.
         * @param stages Specifies the pipeline shader stages where the texture is used.
         * @details The finished state implies it is ready for the use in shaders. If the image already has the correct
         * layout, this function has no effect. Finishing the texture after creating the texture requires calling this
         * function.
         */
        inline void change_layout_C2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        /**
         * @brief Changes the image layout from the loading to the finished state.
         * @param cbo Specifies a command buffer in which to record the layout changes.
         * @param stages Specifies the pipeline shader stages where the texture is used.
         * @details The finished state implies it is ready for the use in shaders. If the image already has the correct
         * layout, this function has no effect. Finishing the texture after loading data requires calling this function.
         */
        inline void change_layout_L2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        /**
         * @brief Calculates the size of a mip-map level.
         * @param level Specifies the number of the mip-map level.
         * @return Returns the size/extent of the mip-map level
         */
        inline VkExtent3D size(uint32_t level) const noexcept;

        /**
         * @brief Executes the commands required to create the mip-map levels.
         * @param cbo Specifies a command buffer in which to record the required commands.
         */
        void create_mip_levels(VkCommandBuffer cbo) noexcept;

        /**
         * @brief Computes the integer base 2 logarithm.
         * @param x Specifies the value from which the logarithm is computed.
         * @return Returns the integer base 2 logarithm.
         */
        static inline uint32_t log2i(uint32_t x) noexcept;

        /**
         * @brief Computes the maximum logarithm of the 3 components of a VkExtent3D structure.
         * @param extent Specifies the extent from which the maximum logarithm is computed.
         * @return Returns the maximum logarithm of the 3 components of the VkExtent3D structure.
         */
        static inline uint32_t max_log2i(VkExtent3D extent) noexcept;

#ifdef VKA_STB_IMAGE_LOAD_ENABLE
        /**
         * @brief Wrapper function for stbi_load, stbi_load_16 and stbi_loadf.
         * @tparam T Specifies the data type of the color component of a pixel.
         * @param path Specifies the path to the image file to load.
         * @param extent Returns the size/extent of the image.
         * @param components Returns the number of color components of the image.
         * @param force_components Forces the implementation to load a specified number of color components.
         * @throw std::runtime_error Is thrown if loading the image failed.
         * @return Returns a pointer to the image's memory.
         * @details The 'depth' component of the image's extent/size will always return 1. Furthermore, color components
         * can have one of the following data types: uint8_t, uint16_t and float.
         */
        template<typename T>
        static inline T* load_image_internal(const char* path, VkExtent3D& extent, uint32_t* components, uint32_t force_components);
#endif

    public:
        /// @details Every vulkan handle is initialized to VK_NULL_HANDLE.
        inline Texture() noexcept;

        /**
         * @brief Creates the texture and allocates its memory.
         * @param device Specifies a valid device.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the creation info for the texture.
         * @throw std::runtime_error Is thrown, if creating the texture image or the sampler, allocating the memory or
         * binding the memory failed.
         * @details This constructor has the same functionality as the function create(). After creating the texture is
         * in the created state.
         */
        explicit inline Texture(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info);

        // Texture is not copyable
        Texture(const Texture&) = delete;
        Texture& operator= (const Texture&) = delete;

        /**
         * @brief Moves the ownership of a texture to a different object.
         * @param src Specifies the texture to move.
         * @details The source texture becomes invalidated. If the move destination has been created before and is a
         * valid object, it gets destroyed.
         */
        inline Texture(Texture&& src) noexcept;
        inline Texture& operator= (Texture&& src) noexcept;

        /// @brief Destroys all created vulkan handles.
        inline ~Texture();

        /**
         * @brief Creates the texture and allocates its memory.
         * @param device Specifies a valid device.
         * @param properties Specifies memory properties of a physical device.
         * @param create_info Specifies the create-info for the texture.
         * @throw std::runtime_error Is thrown, if creating the texture image or the sampler, allocating the memory or
         * binding the memory failed.
         * @details After creating the texture is in the created state.
         */
        inline void create(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info);

        /**
         * @brief Creates and adds an image view to the texture.
         * @param create_info Specifies the create-info for the image view. This is not a VkImageViewCreateInfo, instead
         * a simplified version of the create-info structure is used.
         * @throw std::runtime_error Is thrown if creating the image view failed.
         * @details If the texture has not been created yet, this function does nothing. Additionally, if the image view
         * creation fails, it will not be added.
         */
        void create_view(const TextureViewCreateInfo& create_info);

        /**
         * @brief Destroys all created vulkan handles.
         * @details After the handles are destroyed, they are reset to VK_NULL_HANDLE. Moreover, this function does not
         * destroy parent handles.
         */
        inline void destroy() noexcept;

        /**
         * @brief Records the required command to load the image data from a staging buffer into the texture image.
         * @param cbo Specifies a valid command buffer in which the load command is recorded.
         * @param data Specifies the staging buffer to load.
         * @param layer Specifies the target layer of the load operation.
         * @param layer_count Optionally specifies the number of affected layers if the staging buffer contains data
         * for multiple layers. By default, one layer is affected.
         * @param level Optionally specifies the affected mip-level of the texture. By default, the base mip-level¹ is
         * the affected one.
         * @details Staging buffers usually use host memory.\n
         * ¹The base mip-level specifies the original image without any scaling applied.
         */
        void load(VkCommandBuffer cbo, const Buffer& data, uint32_t layer, uint32_t layer_count = 1, uint32_t level = 0) noexcept;

        /**
         * @brief Loads image data into a staging buffer.
         * @param data Specifies the image data.
         * @param properties Specifies memory properties of a physical device.
         * @param qfamidx Specifies the queue family (index) used for the buffer.
         * @param level Optionally specifies the mip-level of the texture that the buffer should store.
         * @return Returns the staging buffer containing the texture data.
         * @details For simplicity following variables are defined as:\n
         *  w := image width\n w' := downscaled image with\n h := image height\n h' := downscaled image height\n
         *  d := image depth\n d' := downscaled image depth\n f := image format\n s := data size in bytes\n
         *  l := mip-level\n c := number of color components of a pixel\n type := data type of color component\n\n
         * The size of the given data must match the size of the image in bytes, which is:\n
         *  >> s = w * h * d * format_sizeof(f) <<\n\n
         * If a custom mip-level is specified, then the image dimensions (w, h, d) are scaled down according to
         * following formulas:\n
         *  >> w' = max(w / 2^l, 1) <<\n
         *  >> h' = max(h / 2^l, 1) <<\n
         *  >> d' = max(d / 2^l, 1) <<\n
         * Note that no dimension can have a size of 0. Consequently, the minimum size for any dimension is 1.
         * The data size must then be:\n
         *  >> s = w' * h' * d' * format_sizeof(f) <<\n\n
         * The function 'format_sizeof' translates to the following formula:\n
         *  >> format_sizeof(f) = c * sizeof(type) <<\n
         * where 'sizeof' is the C++ sizeof operator.
         * @note If the staging buffer already contained data, that data will now be overwritten by the image data.
         */
        inline Buffer stage(const void* data, const VkPhysicalDeviceMemoryProperties& properties, uint32_t qfamidx, uint32_t level = 0) const;

        /**
         * @brief Loads data from multiple images into one single staging buffer.
         * @param data Specifies the data of all images to be loaded into the staging buffer.
         * @param properties Specifies memory properties of a physical device.
         * @param qfamidx Specifies the queue family (index) used for the buffer.
         * @param layer_count Specifies the number of array layers to load into the staging buffer.
         * @param level Optionally specifies the mip-level of the texture that the buffer should store.
         * @return Returns the staging buffer containing the texture data.
         * @details Image data must be provided for every array layer to load. The data is passed by an array of buffers
         * where each buffer contains the data for one array layer. Additionally, all buffers of the array must meet the
         * size requirements described in the previous function. The number of elements this array must contain is
         * greater than or equal to the number of array layers to load.
         * @note If the staging buffer already contained data, that data will now be overwritten by the image data.
         */
        Buffer stage(const void* const* data, const VkPhysicalDeviceMemoryProperties& properties, uint32_t qfamidx, uint32_t layer_count, uint32_t level = 0) const;

        /**
         * @brief Finishes the texture creation. This texture is now ready to be used in shaders.
         * @param cbo Specifies a valid command buffer in which the commands are recorded.
         * @param stages Specifies the (pipeline-) shader stages where the texture is used.
         * @details The layout is changed to the final layout required for rendering. Additionally, if mip-map creation
         * is active, this function also records the commands to generate the mip-map levels.
         * @note This function or 'finish_manual' must be called as the last operation when creating a texture.
         */
        void finish(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        /**
         * @brief Finishes the texture creation if mip-map levels were loaded manually. This texture is now ready to be
         * used in shaders.
         * @param cbo Specifies a valid command buffer in which the commands are recorded.
         * @param stages Specifies the (pipeline-) shader stages where the texture is used.
         * @details If mip-map creation is deactivated, this function has the same functionality as 'finish'. It does
         * not generate mip-map levels even if mip-map generation is activated. It only changes the image's layout to
         * the final layout required for rendering.
         * @note This function or 'finish' must be called as the last operation when creating a texture.
         */
        void finish_manual(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        /// @return Returns the extent of the texture.
        inline VkExtent3D size() const noexcept;

        /**
         * @return Returns the number of mip levels contained in the image which is at least 1. If the texture has not
         * been created or is invalid, the returned mip-level count is 0.
         */
        inline uint32_t level_count() const noexcept;

        /**
         * @param extent Specifies the image extent from which to calculate the mip levels.
         * @return Returns the number of mip-levels that will be generated from a given extent.
         */
        static inline uint32_t level_count(VkExtent3D extent) noexcept;

        /// @return Returns the number of array layers contained in the texture.
        inline uint32_t layer_count() const noexcept;

        /// @return Returns the vulkan image handle.
        inline VkImage handle() const noexcept;

        /// @return Returns the vulkan sampler handle.
        inline VkSampler sampler() const noexcept;

        /**
         * @param i Specifies the index of the view to return.
         * @return Returns the vulkan image view handle.
         * @throw std::out_of_range Is thrown if the specified index is greater than or equal to the number of views.
         */
        inline VkImageView view(size_t i) const;

        /**
         * @param i Specifies the index of the view to return.
         * @return Returns the vulkan image view handle.
         * @note This function does not perform a range check.
         */
        inline VkImageView view_u(size_t i) const noexcept;

        /// @return Returns true if the Texture is a valid object and false otherwise.
        inline bool is_valid() const noexcept;

#ifdef VKA_STB_IMAGE_LOAD_ENABLE
        /**
         * @brief Loads an image from a file.
         * @tparam T Specifies the data type of the color component of a pixel.
         * @param path Specifies the path to the image file.
         * @param extent Returns the extent/size of the image.
         * @param components Returns the number of color components of a pixel or number of color channels of the image
         * respectively.
         * @return Returns a pointer to a buffer where the image data is stored.
         * @throw std::runtime_error Is thrown if loading the image failed.
         * @details The 'depth' component of the image's extent/size will always return 1. Furthermore, color components
         * can have one of the following data types: uint8_t, uint16_t and float.
         */
        template<typename T>
        static inline T* load_image(const char* path, VkExtent3D& extent, uint32_t& components);

        /**
         * @brief Loads an image from a file.
         * @tparam T Specifies the data type of the color component of a pixel.
         * @tparam force_components Forces the implementation to load a desired number of color components independent
         * of how many color channels the image file has. This value must be in the interval [1, 4].
         * @param path Specifies the path to the image file.
         * @param extent Returns the extent/size of the image.
         * @return Returns a pointer to a buffer where the image data is stored.
         * @throw std::runtime_error Is thrown if loading the image failed.
         * @details The 'depth' component of the image's extent/size will always return 1. Furthermore, color components
         * can have one of the following data types: uint8_t, uint16_t and float.
         */
        template<typename T, uint32_t force_components>
        static inline T* load_image(const char* path, VkExtent3D& extent);

        /**
         * @brief Frees allocated image data.
         * @param data Specifies the data for free.
         * @details Is just a wrapper for the function free().
         */
        static inline void free_image(void* data) noexcept;
#endif
    };
}
