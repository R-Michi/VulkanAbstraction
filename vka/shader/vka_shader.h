/**
* @file     vka_shader.h
* @brief    Shader definition file.
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
    class Shader
    {
    private:
        VkDevice _device;
        char* _entry_point;
        VkShaderStageFlagBits _stage;
        VkShaderModule _module;
        size_t _size;
        bool _loaded;
    
    public:
        Shader(void) noexcept;
    
        /**
        * @param[in] device: logical device
        * @param[in] entry_point: shader entry point
        */
        explicit Shader(VkDevice device, const std::string& entry_point = "main") noexcept;
        
        Shader(const Shader&) = delete;
        Shader& operator= (const Shader&) = delete;
        
        Shader(Shader&& shader);
        Shader& operator= (Shader&& shader);
        
        virtual ~Shader(void) noexcept;
        
        /**
        * @brief Loads the pre-compiled shader file (file extension: .spv)
        * @param[in] path: path to the shader file
        * @param[in] stage: shader stage (vertex, geometry, fragment,...)
        * @return vulkan result
        */
        VkResult load(const std::string& path, VkShaderStageFlagBits stage);
        
        /** @brief Clears the shader. The shader can be reloaded again. */
        void clear(void);
        
        /** @param[in] device: logical device */
        void set_device(VkDevice device) noexcept;
        
        /** @param[in] entry_point: shader entry point */
        void set_entry_point(const std::string& entry_point) noexcept;
        
        /** @return shader module handle */
        VkShaderModule get_module(void) const noexcept;
        
        /** @return shader stage */
        VkShaderStageFlagBits get_stage(void) const noexcept;
        
        /** @return shader entry point */
        const char* get_entry_point(void) const noexcept;
        
        /** @return shader code size in bytes */
        size_t size(void) const noexcept;
    };

    class ShaderProgram
    {
    private:
        std::vector<VkPipelineShaderStageCreateInfo> _shader_stages;
        std::vector<Shader> _shaders;
    
    public:
        ShaderProgram(void) noexcept;
        
        /**
        * @brief Construct or to attach multiple shaders.
        * @param[in] shaders: shaders to attach
        */
        explicit ShaderProgram(std::vector<Shader>& shaders);
        
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator= (const ShaderProgram&) = delete;
        
        ShaderProgram(ShaderProgram&&) = delete;
        ShaderProgram& operator= (ShaderProgram&&) = delete;
        
        virtual ~ShaderProgram(void) noexcept;
        
        /**
        * @brief Attaches a shader to the shader program.
        * @param[in] shader: shader to attach
        */
        void attach(Shader& shader);
        
        /** @brief Cleares the shader program. No shaders are attached anymore after clear. */
        void clear(void) noexcept;
        
        /** @return amount of shaders attached to the shader program */
        uint32_t count(void) const noexcept;
        
        /** @return array of VkPipelineShaderStageCreateInfo structs for the rendering pipeline */
        const VkPipelineShaderStageCreateInfo* get_stages(void) const noexcept;
    };
}