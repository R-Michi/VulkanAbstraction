/**
* @file     vka_shader.h
* @brief    Shader definition file.
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
    class Shader
    {
    private:
        static constexpr const char FILE_OPEN_FAILED[] = "[vka::Shader::create]: Failed to open shader file.";
        static constexpr const char SHADER_CREATE_FAILED[] = "[vka::Shader::create]: Failed to create shader module.";

        VkDevice m_device;
        VkShaderModule m_module;        // this is the shader handle

        /// @brief Destroys all created vulkan handles.
        void destroy_handles(void) noexcept;

        /**
         * @brief Creates a shader module from a shader file.
         * @param path Specifies the path to the shader file.
         * @throw std::runtime_error Is thrown, if the file could not be opened or, if creating the shader module
         * failed.
         */
        void internal_create(const std::string& path);

    public:
        /// @details Every vulkan handle is initialized to VK_NULL_HANDLE.
        inline Shader(void) noexcept;

        /**
         * @brief Creates a shader module from a shader file.
         * @details This constructor has the same functionality as the function create().
         * @param device Specifies a valid device.
         * @param path Specifies the path to the shader file.
         * @throw std::runtime_error Is thrown, if the file could not be opened or, if creating the shader module
         * failed.
         */
        explicit inline Shader(VkDevice device, const std::string& path);

        // There is no need for a shader to be copyable.
        Shader(const Shader&) = delete;
        Shader& operator= (const Shader&) = delete;

        /**
         * @brief Transfers ownership of a shader module to 'this'.
         * @details The source shader becomes invalidated. If 'this' has been created before and is a valid instance, it
         * gets destroyed and replaced by the handles of the source object.
         * @param src Specifies the shader to move.
         */
        inline Shader(Shader&& src) noexcept;
        inline Shader& operator= (Shader&& src) noexcept;

        /// @brief Destroys all created vulkan handles.
        virtual inline ~Shader(void);

        /**
         * @brief Creates a shader module from a shader file.
         * @param device Specifies a valid device.
         * @param path Specifies the path to the shader file.
         * @throw std::runtime_error Is thrown, if the file could not be opened or, if creating the shader module
         * failed.
         */
        inline void create(VkDevice device, const std::string& path);

        /**
         * @brief Destroys all created vulkan handles.
         * @details After the handles are destroyed they are reset to VK_NULL_HANDLE. Moreover, parent handles are not
         * destroyed by this function.
         */
        inline void destroy(void) noexcept;

        /// @return Returns the shader module handle.
        inline VkShaderModule handle(void) const noexcept;

        /// @return Returns true, if the shader is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;

        /**
         * @brief Generates a pipeline shader stage from the shader module.
         * @param stage Specifies the stage where the shader is used.
         * @param flags Optionally specifies additional flags for the shader stage.
         * @param entry_point Optionally specifies a custom entry point for the shader stage. The entry point is the
         * name of the main function and is "main" by default.
         * @param specialization Optionally specifies additional specialization-info for the shader stage.
         * @return Returns the shader stage in form of a VkPipelineShaderStageCreateInfo structure which is used to
         * create a pipeline.
         * @note For more information also see the vulkan documentation of VkPipelineShaderStageCreateInfo.
         */
        inline VkPipelineShaderStageCreateInfo make_stage(
            VkShaderStageFlagBits stage,
            VkPipelineShaderStageCreateFlags flags = 0, 
            const char* entry_point = "main", 
            const VkSpecializationInfo* specialization = nullptr
        ) const noexcept;
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_shader_impl.inl"
#endif
#include "vka_shader_inline_impl.inl"
