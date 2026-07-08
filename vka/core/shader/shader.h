/**
 * @brief Helper class for creating shaders.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
    /**
     * Abstraction to simplify reading shaders from a file and creating shader modules. Contains the vulkan
     * <c>VkShaderModule</c> handle.
     *
     * <b>Default initialization:</b>\n
     * The default constructor creates an <b>empty</b> shader module. Any member function returning a vulkan handle
     * returns <c>VK_NULL_HANDLE</c>. Calling <c>destroy()</c> does nothing. Calling <c>make_stage()</c> returns an
     * invalid <c>VkPipelineShaderStageCreateInfo</c>.
     *
     * <b>Initialization:</b>\n
     * The initialization constructor creates a valid shader module, if no exception was thrown.
     *
     * <b>Copy behaviour:</b>\n
     * The copy constructor and operator are deleted.
     *
     * <b>Moving behaviour:</b>\n
     * When calling the move constructor or operator, the moved object is invalidated and performing any operation on it
     * is unsafe. This may lead to undefined behaviour or even a crash. If an already valid object is replaced by a
     * move, the current object is destroyed.
     *
     * <b>Destroy behaviour:</b>\n
     * Destroys all vulkan handles and sets everything back to default values. After destroying the object is an
     * <b>empty</b> shader module.
     *
     * <b>Inheritance behaviour:</b>\n
     * This class is final and cannot be inherited.
     *
     * <b>Threading behaviour:</b>\n
     * This class can be created and used from any thread. However, if you use this class across multiple threads,
     * actions must be externally synchronized.
     */
    class Shader final
    {
    public:
        /**
         * Loads a shader file and creates a vulkan shader module from it.
         * @param device Device with which the shader is created.
         * @param path Path to the shader file.
         */
        explicit Shader(VkDevice device, const std::string& path);

        /// @return Returns whether the shader module is valid.
        explicit constexpr operator bool() const noexcept;

        /// @return Returns the parent handle.
        constexpr VkDevice parent() const noexcept;

        /// @return Returns the vulkan <c>VkShaderModule</c> handle.
        constexpr VkShaderModule handle() const noexcept;

        /// Destroys the shader module. After destroying the shader module is empty and therefore invalid.
        constexpr void destroy() noexcept;

        /**
         * Creates a pipeline shader stage from the shader module.
         * @param stage Stage in which the shader module is used.
         * @param flags Optional flags for the shader stage.
         * @param entry_point Entrypoint for the shader program. Default is <c>main()</c>.
         * @param specialization Optional specialization info for the shader stage.
         * @return Returns the shader stage in the form of a <c>VkPipelineShaderStageCreateInfo</c> structure
         */
        constexpr VkPipelineShaderStageCreateInfo make_stage(
            VkShaderStageFlagBits stage,
            VkPipelineShaderStageCreateFlags flags = 0,
            const char* entry_point = "main",
            const VkSpecializationInfo* specialization = nullptr
        ) const noexcept;

        // default:
        Shader() = default;
        Shader(Shader&& src) = default;
        ~Shader() = default;
        Shader& operator= (Shader&& src) = default;

    private:
        static constexpr char FILE_OPEN_FAILED[] = "[vka::Shader]: Failed to open shader file.";
        static constexpr char SHADER_CREATE_FAILED[] = "[vka::Shader]: Failed to create shader module.";

        unique_handle<VkShaderModule> m_module;

        /// Loads the shader file and creates the shader module.
        static unique_handle<VkShaderModule> create_shader_module(VkDevice device, const std::string& path);
    };
}
