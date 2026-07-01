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
    class Shader
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
