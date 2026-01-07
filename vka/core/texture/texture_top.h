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
#ifdef VKA_STB_ENABLE

    /**
     * Helper class to load and merge 2D images from a file or 3D images from memory into a single image. Every time
     * <c>load()</c> is called, the components of the current image are appended to the already existing components of
     * the texture. The specified format caps the total number of components that can be attached. Excess components are
     * cut off. Unused components are filled with zeros. The extent of all images must be the same. The first image
     * loaded defines the texture's extent.
     *
     * @tparam F Specifies the format of the loader. If an image has a different format data-type than the loader,
     * loading this image results in undefined behavior or even a crash! The texture must be created with a format that
     * has the same number of components as the loader's format!
     */
    template<VkFormat F> requires detail::texture::is_loader_format<F>
    class TextureComponentMerger
    {
        using component_t = detail::texture::loader_format_t<F>;
        static constexpr auto loader_f = detail::texture::loader_f<F>;

    public:
        /**
         * Loads the first image from memory.
         * @param data Specifies the data of the image to load. Must at least contain\n
         * <c>extent.width * extent.height * extent.depth * format_countof(F)</c> elements or\n
         * <c>extent.width * extent.height * extent.depth * format_sizeof(F)</c> bytes.
         * @param format Specifies the format of the image expressed in a vulkan <c>VkFormat</c>.
         * @param extent Specifies the extent of the image. This extent is used as the extent of the texture.
         */
        explicit TextureComponentMerger(const component_t* data, VkFormat format, VkExtent3D extent);

        /**
         * Loads a single color into the image.
         * @param color Specifies the color. Must at least contain <c>comp</c> elements.
         * @param comp Specifies the number of components the color has.
         * @param extent Specifies the extent of the image. This extent is used as the extent of the texture.
         */
        explicit TextureComponentMerger(const component_t* color, uint32_t comp, VkExtent3D extent);

        /**
         * Loads the first image from a file. Its extent is used as the extent of the texture.
         * @param path Specifies the path to the image file.
         * @throw std::invalid_argument Is thrown if the file could not be found.
         */
        explicit TextureComponentMerger(const char* path);

        /**
         * Moves the texture merger. DO NOT use the source buffer after a move! This may lead to unexpected or even
         * undefined behavior.
         */
        inline TextureComponentMerger(TextureComponentMerger&& src) noexcept;

        /// Destroys the texture merger.
        inline ~TextureComponentMerger();

        /**
         * Moves the texture merger. DO NOT use the source buffer after a move! This may lead to unexpected or even
         * undefined behavior.
         */
        inline TextureComponentMerger& operator= (TextureComponentMerger&& src) noexcept;

        /// @return Returns the raw image data.
        inline const component_t* data() const noexcept;

        /// @return Returns the extent of the image.
        inline VkExtent3D extent() const noexcept;

        /**
         * Loads the first image from memory. The loaded extent is defined by the first loaded image (constructor).
         * @param data Specifies the data of the image to load. Must at least contain\n
         * <c>extent.width * extent.height * extent.depth * format_countof(F)</c> elements or\n
         * <c>extent.width * extent.height * extent.depth * format_sizeof(F)</c> bytes.
         * @param format Specifies the format of the image expressed in a vulkan <c>VkFormat</c>.
         */
        void load(const component_t* data, VkFormat format) noexcept;

        /**
         * Loads a single color into the image. The loaded extent is defined by the first loaded image (constructor).
         * @param color Specifies the color. Must at least contain <c>comp</c> elements.
         * @param comp Specifies the number of components the color has.
         */
        void load(const component_t* color, uint32_t comp) noexcept;

        /**
         * Loads an image from a file.
         * @param path Specifies the path to the image file.
         * @throw std::invalid_argument Is thrown if the file could not be found.
         * @throw std::runtime_error Is thrown if the extent of the image does not match the extent of the texture.
         */
        void load(const char* path);

        // Deleted:
        TextureComponentMerger(const TextureComponentMerger&) = delete;
        TextureComponentMerger& operator= (const TextureComponentMerger&) = delete;

    private:
        static constexpr const char* MSG_EXTENT_MISSMATCH = "[vka::TextureComponentMerger]: Extent of loaded image does not match the extent of the texture.";
        static constexpr const char* MSG_INVALID_PATH = "[vka::TextureComponentMerger]: Cannot find path to the image file.";

        component_t* m_data;
        VkExtent3D m_extent;
        uint32_t m_component_idx;

        /// Copies the image data to the buffer.
        inline void copy_image(const component_t* data, uint32_t img_comp) noexcept;

        /// Fills an image with a single color.
        inline void fill_image(const component_t* color, uint32_t img_comp) noexcept;

        /// Calculates the allocation size for the image buffer.
        static constexpr size_t alloc_size(VkExtent3D extent) noexcept;
    };

    /**
     * Helper class to load 2D images from a file or from memory and to construct a 3D texture out of them. Every time
     * <c>load()</c> is called, the depth of the texture is expanded by <c>1</c>. The depth indices correspond to the
     * order in which the images were loaded, starting at <c>0</c>. The extent of all images must be the same. The first
     * image loaded defines the texture's extent.
     *
     * @tparam F Specifies the format of the loader. If an image has a different format data-type than the loader,
     * loading this image results in undefined behavior or even a crash! If an image has more components than specified,
     * only the first <c>format_countof(F)</c> components are loaded. If an image has fewer components than specified,
     * the remaining components are filled with <c>zeros</c>. The texture must be created with a format that has the
     * same number of components as the loader's format!
     */
    template<VkFormat F> requires detail::texture::is_loader_format<F>
    class Texture3DMerger
    {
        using component_t = detail::texture::loader_format_t<F>;
        static constexpr auto load_f = detail::texture::loader_f<F>;

    public:
        /**
         * Loads the first image from memory.
         * @param data Specifies the data of the image to load. Must at least contain\n
         * <c>extent.width * extent.height * extent.depth * format_countof(F)</c> elements or\n
         * <c>extent.width * extent.height * extent.depth * format_sizeof(F)</c> bytes.
         * @param format Specifies the format of the image expressed in a vulkan <c>VkFormat</c>.
         * @param extent Specifies the extent of the image. This extent is used as the extent of the texture.
         * @param expected_depth Specifies the expected depth. This pre-allocates enough memory.
         */
        explicit Texture3DMerger(const component_t* data, VkFormat format, VkExtent2D extent, uint32_t expected_depth = 1);

        /**
         * Loads a single color into the image.
         * @param color Specifies the color. Must at least contain <c>comp</c> elements.
         * @param comp Specifies the number of components the color has.
         * @param extent Specifies the extent of the image. This extent is used as the extent of the texture.
         * @param expected_depth Specifies the expected depth. This pre-allocates enough memory.
         */
        explicit Texture3DMerger(const component_t* color, uint32_t comp, VkExtent2D extent, uint32_t expected_depth = 1);

        /**
         * Loads the first image from a file. Its extent is used as the extent of the texture.
         * @param path Specifies the path to the image file.
         * @throw std::invalid_argument Is thrown if the file could not be found.
         * @param expected_depth Specifies the expected depth. This pre-allocates enough memory.
         */
        explicit Texture3DMerger(const char* path, uint32_t expected_depth = 1);

        /**
         * Moves the texture merger. DO NOT use the source buffer after a move! This may lead to unexpected or even
         * undefined behavior.
         */
        inline Texture3DMerger(Texture3DMerger&& src) noexcept;

        /// Destroys the texture merger.
        inline ~Texture3DMerger();

        /**
         * Moves the texture merger. DO NOT use the source buffer after a move! This may lead to unexpected or even
         * undefined behavior.
         */
        inline Texture3DMerger& operator= (Texture3DMerger&& src) noexcept;

        /// @return Returns the raw image data.
        inline const component_t* data() const noexcept;

        /// @return Returns the extent of the image. Also includes the depth.
        inline VkExtent3D extent() const noexcept;

        /// @return Returns the resulting depth.
        inline uint32_t depth() const noexcept;

        /**
         * Loads the first image from memory. The loaded extent is defined by the first loaded image (constructor).
         * @param data Specifies the data of the image to load. Must at least contain\n
         * <c>extent.width * extent.height * extent.depth * format_countof(F)</c> elements or\n
         * <c>extent.width * extent.height * extent.depth * format_sizeof(F)</c> bytes.
         * @param format Specifies the format of the image expressed in a vulkan <c>VkFormat</c>.
         */
        void load(const component_t* data, VkFormat format) noexcept;

        /**
         * Loads a single color into the image. The loaded extent is defined by the first loaded image (constructor).
         * @param color Specifies the color. Must at least contain <c>comp</c> elements.
         * @param comp Specifies the number of components the color has.
         */
        void load(const component_t* color, uint32_t comp) noexcept;

        /**
         * Loads an image from a file.
         * @param path Specifies the path to the image file.
         * @throw std::invalid_argument Is thrown if the file could not be found.
         * @throw std::runtime_error Is thrown if the extent of the image does not match the extent of the texture.
         */
        void load(const char* path);

        // Deleted:
        Texture3DMerger(const Texture3DMerger&) = delete;
        Texture3DMerger& operator= (const Texture3DMerger&) = delete;

    private:
        static constexpr const char* MSG_EXTENT_MISSMATCH = "[vka::Texture3DMerger]: Extent of loaded image does not match the extent of the texture.";
        static constexpr const char* MSG_INVALID_PATH = "[vka::Texture3DMerger]: Cannot find path to the image file.";

        component_t* m_data;
        uint32_t m_alloc_depth;
        VkExtent2D m_extent;
        uint32_t m_depth;

        /// Grows the image buffer.
        void grow();

        /// Copies the image data to the buffer.
        inline void copy_image(const component_t* data, uint32_t img_comp) noexcept;

        /// Fills an image with a single color.
        inline void fill_image(const component_t* color, uint32_t img_comp) noexcept;

        /// Calculates the allocation size for the image buffer.
        static constexpr size_t alloc_size(VkExtent2D extent, uint32_t depth) noexcept;

        /// Calculates the initial number of layers to allocate.
        static constexpr uint32_t alloc_depth(uint32_t expected_depth) noexcept;

        /// Calculates the growth factor of the image buffer measured in layers.
        static constexpr uint32_t grow_factor(uint32_t depth) noexcept;
    };

    /**
     * Helper class to load 2D images from a file or 3D images from memory. Every time <c>load()</c> is called, a new
     * image layer is added to the texture, resulting in an array of images. The layer indices correspond to the order
     * in which the images were loaded, starting at <c>0</c>. The extent of all images must be the same. The first image
     * loaded defines the texture's extent.
     *
     * Possible loader formats:
     *
     * 8-bit unsigned integer formats:
     * - <c>VK_FORMAT_R8_UINT</c>
     * - <c>VK_FORMAT_R8G8_UINT</c>
     * - <c>VK_FORMAT_R8G8B8_UINT</c>
     * - <c>VK_FORMAT_R8G8B8A8_UINT</c>
     *
     * 16-bit unsigned integer formats:
     * - <c>VK_FORMAT_R16_UINT</c>
     * - <c>VK_FORMAT_R16G16_UINT</c>
     * - <c>VK_FORMAT_R16G16B16_UINT</c>
     * - <c>VK_FORMAT_R16G16B16A16_UINT</c>
     *
     * 32-bit float formats:
     * - <c>VK_FORMAT_R32_SFLOAT</c>
     * - <c>VK_FORMAT_R32G32_SFLOAT</c>
     * - <c>VK_FORMAT_R32G32B32_SFLOAT</c>
     * - <c>VK_FORMAT_R32G32B32A32_SFLOAT</c>
     *
     * @tparam F Specifies the format of the loader. If an image has a different format data-type than the loader,
     * loading this image results in undefined behavior or even a crash! If an image has more components than specified,
     * only the first <c>format_countof(F)</c> components are loaded. If an image has fewer components than specified,
     * the remaining components are filled with <c>zeros</c>. The texture must be created with a format that has the
     * same number of components as the loader's format!
     */
    template<VkFormat F> requires detail::texture::is_loader_format<F>
    class TextureLoader
    {
        using component_t = detail::texture::loader_format_t<F>;
        static constexpr auto load_f = detail::texture::loader_f<F>;

    public:
        /**
         * Loads the first image from memory.
         * @param data Specifies the data of the image to load. Must at least contain\n
         * <c>extent.width * extent.height * extent.depth * format_countof(F)</c> elements or\n
         * <c>extent.width * extent.height * extent.depth * format_sizeof(F)</c> bytes.
         * @param format Specifies the format of the image expressed in a vulkan <c>VkFormat</c>.
         * @param extent Specifies the extent of the image. This extent is used as the extent of the texture.
         * @param expected_layers Specifies the expected depth. This pre-allocates enough memory.
         */
        explicit TextureLoader(const component_t* data, VkFormat format, VkExtent3D extent, uint32_t expected_layers = 1);

        /**
         * Loads a single color into the image.
         * @param color Specifies the color. Must at least contain <c>comp</c> elements.
         * @param comp Specifies the number of components the color has.
         * @param extent Specifies the extent of the image. This extent is used as the extent of the texture.
         * @param expected_layers Specifies the expected depth. This pre-allocates enough memory.
         */
        explicit TextureLoader(const component_t* color, uint32_t comp, VkExtent3D extent, uint32_t expected_layers = 1);

        /**
         * Loads the first image from a file. Its extent is used as the extent of the texture.
         * @param path Specifies the path to the image file.
         * @throw std::invalid_argument Is thrown if the file could not be found.
         * @param expected_layers Specifies the expected depth. This pre-allocates enough memory.
         */
        explicit TextureLoader(const char* path, uint32_t expected_layers = 1);

        /**
         * Moves the texture loader. DO NOT use the source buffer after a move! This may lead to unexpected or even
         * undefined behavior.
         */
        inline TextureLoader(TextureLoader&& src) noexcept;

        /// Destroys the texture loader.
        inline ~TextureLoader();

        /**
         * Moves the texture loader. DO NOT use the source buffer after a move! This may lead to unexpected or even
         * undefined behavior.
         */
        inline TextureLoader& operator= (TextureLoader&& src) noexcept;

        /// @return Returns the raw image data.
        inline const component_t* data() const noexcept;

        /// @return Returns the extent of the image.
        inline VkExtent3D extent() const noexcept;

        /// @return Returns the current number of layers.
        inline uint32_t layer_count() const noexcept;

        /**
         * Loads the first image from memory. The loaded extent is defined by the first loaded image (constructor).
         * @param data Specifies the data of the image to load. Must at least contain\n
         * <c>extent.width * extent.height * extent.depth * format_countof(F)</c> elements or\n
         * <c>extent.width * extent.height * extent.depth * format_sizeof(F)</c> bytes.
         * @param format Specifies the format of the image expressed in a vulkan <c>VkFormat</c>.
         */
        void load(const component_t* data, VkFormat format) noexcept;

        /**
         * Loads a single color into the image. The loaded extent is defined by the first loaded image (constructor).
         * @param color Specifies the color. Must at least contain <c>comp</c> elements.
         * @param comp Specifies the number of components the color has.
         */
        void load(const component_t* color, uint32_t comp) noexcept;

        /**
         * Loads an image from a file.
         * @param path Specifies the path to the image file.
         * @throw std::invalid_argument Is thrown if the file could not be found.
         * @throw std::runtime_error Is thrown if the extent of the image does not match the extent of the texture.
         */
        void load(const char* path);

        // Deleted:
        TextureLoader(const TextureLoader&) = delete;
        TextureLoader& operator= (const TextureLoader&) = delete;

    private:
        static constexpr const char* MSG_EXTENT_MISSMATCH = "[vka::TextureLoader]: Extent of loaded image does not match the extent of the texture.";
        static constexpr const char* MSG_INVALID_PATH = "[vka::TextureLoader]: Cannot find path to the image file.";

        component_t* m_data;
        uint32_t m_alloc_layers;
        VkExtent3D m_extent;
        uint32_t m_layer_count;

        /// Grows the image buffer.
        void grow();

        /// Copies the image data to the buffer.
        inline void copy_image(const component_t* data, uint32_t img_comp) noexcept;

        /// Fills an image with a single color.
        inline void fill_image(const component_t* color, uint32_t img_comp) noexcept;

        /// Calculates the allocation size for the image buffer.
        static constexpr size_t alloc_size(VkExtent3D extent, uint32_t layer_count) noexcept;

        /// Calculates the initial number of layers to allocate.
        static constexpr uint32_t alloc_layers(uint32_t expected_layers) noexcept;

        /// Calculates the growth factor of the image buffer measured in layers.
        static constexpr uint32_t grow_factor(uint32_t layer_count) noexcept;
    };
#endif

    /// Simplifies creating textures in vulkan.
    class Texture
    {
        using TextureHandle = detail::texture::Handle;

    public:
        /// Default initialization = empty texture.
        constexpr Texture() noexcept;

        /**
         * Creates the texture.
         * @param device Specifies the device with which the texture is created.
         * @param properties Specifies the memory properties of a physical device.
         * @param create_info Specifies the create-info for the texture. It also contains the command buffer, in which
         * the commands required for texture creation are recorded.
         */
        explicit Texture(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info);

        /**
         * No range check is performed.
         * @retun Returns the vulkan <c>VkImageView</c> handle at the specified index.
         */
        constexpr VkImageView operator[] (uint32_t idx) const noexcept;

        /// @return Returns whether the texture is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the dimensions of the texture.
        constexpr VkExtent3D size() const noexcept;

        /// @return Returns the number of image views that where created from the texture.
        constexpr uint32_t view_count() const noexcept;

        /// @return Returns the number of array layers the texture has.
        constexpr uint32_t layer_count() const noexcept;

        /// @return Returns the number of mip-map levels the texture has.
        constexpr uint32_t level_count() const noexcept;

        /// @return Returns the number of mip-map levels that can be generated.
        static constexpr uint32_t level_count(VkExtent3D extent) noexcept;

        /// @returns Returns the maximum LOD the texture can have.
        static constexpr float max_lod(VkExtent3D extent) noexcept;

        /// @return Returns the vulkan <c>VkImage</c> handle.
        constexpr VkImage image() const noexcept;

        /// @return Returns the vulkan <c>VkSampler</c> handle.
        constexpr VkSampler sampler() const noexcept;

        /// @return Returns the parent handle.
        constexpr VkDevice parent() const noexcept;

        /**
         * Performs a range check on the index.
         * @return Returns the vulkan <c>VkImageView</c> handle at the specified index.
         * @throw std::out_of_range Is thrown if the specified index is out of bounds of the image-view array.
         */
        constexpr VkImageView view(uint32_t idx) const;

        /**
         * Loads image data into the texture.
         * @param cbo Specifies the command buffer in which the load command is recorded.
         * @param data Specifies the buffer which holds the texture data.
         * @param layer Specifies the target array layer.
         * @param count Specifies the number of affected layers. Range of affected layers:\n
         * <c>[layer, layer + count - 1]</c>.
         * @param level Specifies the target mip-map level. You can load the mip levels yourself.
         */
        void load(VkCommandBuffer cbo, const Buffer& data, uint32_t layer, uint32_t count = 1, uint32_t level = 0) noexcept;

#ifdef VKA_STB_ENABLE

        /**
         * Loads the data of a <c>TextureComponentMerger</c> object into the texture.
         * @param cbo Specifies the command buffer in which the load command is recorded.
         * @param loader Specifies the <c>TextureComponentMerger</c> whose data should be uploaded.
         * @param info Provides information for the staging buffer.
         * @param layer Specifies the target array layer. Only this layer is affected.
         * @param level Specifies the target mip-map level. You can load the mip levels yourself.
         * @return Returns the staging buffer.
         */
        template<VkFormat F> requires detail::texture::is_loader_format<F>
        [[nodiscard]]
        Buffer load(VkCommandBuffer cbo, const TextureComponentMerger<F>& loader, TextureLoadInfo info, uint32_t layer, uint32_t level = 0);

        /**
         * Loads the data of a <c>Texture3DMerger</c> object into the texture.
         * @param cbo Specifies the command buffer in which the load command is recorded.
         * @param loader Specifies the <c>Texture3DMerger</c> whose data should be uploaded.
         * @param info Provides information for the staging buffer.
         * @param layer Specifies the target array layer. Only this layer is affected.
         * @param level Specifies the target mip-map level. You can load the mip levels yourself.
         * @return Returns the staging buffer.
         */
        template<VkFormat F> requires detail::texture::is_loader_format<F>
        [[nodiscard]]
        Buffer load(VkCommandBuffer cbo, const Texture3DMerger<F>& loader, TextureLoadInfo info, uint32_t layer, uint32_t level = 0);

        /**
         * Loads the data of a <c>TextureLoader</c> object into the texture.
         * @param cbo Specifies the command buffer in which the load command is recorded.
         * @param loader Specifies the <c>TextureLoader</c> whose data should be uploaded.
         * @param info Provides information for the staging buffer.
         * @param layer Specifies the target array layer. Range of affected layers:\n
         * <c>[layer, layer + loader.layer_count() - 1]</c>
         * @param level Specifies the target mip-map level. You can load the mip levels yourself.
         * @return Returns the staging buffer.
         */
        template<VkFormat F> requires detail::texture::is_loader_format<F>
        [[nodiscard]]
        Buffer load(VkCommandBuffer cbo, const TextureLoader<F>& loader, TextureLoadInfo info, uint32_t layer, uint32_t level = 0);

#endif

        /**
         * Finishes the texture creation and creates the mip-map (if mip-map creation is activated). This operation must
         * be executed after loading the texture data.
         * @param cbo Specifies the command buffer in which the finishing commands are recorded.
         * @param stages Specifies the pipeline stages in which the texture is used.
         */
        void finish(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        /**
         * Finishes the texture creation. However, it does not create the mip-map. This operation must be executed if
         * you either have mip-map creation disabled or if you specified custom mip-map levels.
         * @param cbo Specifies the command buffer in which the finishing commands are recorded.
         * @param stages Specifies the pipeline stages in which the texture is used.
         */
        void finish_manual(VkCommandBuffer cbo, VkPipelineStageFlags stages) noexcept;

        // Default:
        Texture(Texture&&) = default;
        ~Texture() = default;
        Texture& operator= (Texture&&) = default;

        // Deleted:
        Texture(const Texture&) = delete;
        Texture& operator= (const Texture&) = delete;

    private:
        static constexpr const char* IMAGE_CREATE_FAILED = "[vka::Texture]: Failed to create image handle.";
        static constexpr const char* ALLOC_MEMORY_FAILED = "[vka::Texture]: Failed to allocate memory.";
        static constexpr const char* BIND_MEMORY_FAILED = "[vka::Texture]: Failed to bind memory to image.";
        static constexpr const char* SAMPLER_CREATE_FAILED = "[vka::Texture]: Failed to create sampler";
        static constexpr const char* VIEW_CREATE_FAILED = "[vka::Texture]: Failed to create image view.";
        static constexpr const char* VIEW_OUT_OF_RANGE = "[vka::Texture]: Image view index out of range.";
        static constexpr VkOffset3D ZERO_OFFSET = { 0, 0, 0 };

        unique_handle<TextureHandle> m_texture;
        VkExtent3D m_extent;
        uint16_t m_layer_count;
        uint16_t m_level_count;

        /// Creates the texture handle.
        static TextureHandle create_texture(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const TextureCreateInfo& create_info);

        /// Calculates the number of mip-map levels.
        static inline uint32_t mip_level_count(const TextureCreateInfo& create_info) noexcept;

        /// Changes the image layout from the creation state to the loading state.
        inline void change_layout_C2L(VkCommandBuffer cbo) const noexcept;

        /// Change the layout from loadig state to final state.
        inline void change_layout_L2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) const noexcept;

        /// Changes the layout from mip-map creation state to final state.
        inline void change_layout_M2F(VkCommandBuffer cbo, VkPipelineStageFlags stages) const noexcept;

        /// Creates the mip-map levels.
        void create_mipmap(VkCommandBuffer cbo) const noexcept;

        /// Creates the staging buffer and loads the image data into the buffer.
        static Buffer stage(VkDevice device, const void* data, VkDeviceSize size, TextureLoadInfo info);
    };
}
