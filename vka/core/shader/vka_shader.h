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
        VkDevice _device;
        char* _entry_point;
        VkShaderStageFlagBits _stage;
        VkShaderModule _module;
        size_t _size;
    
    public:
        Shader(void) noexcept;
    
        /**
        * @param[in] device         logical device
        * @param[in] entry_point    the shader's entry point
        */
        explicit Shader(VkDevice device, const std::string& entry_point = "main") noexcept;
        
        Shader(const Shader&) = delete;
        Shader& operator= (const Shader&) = delete;
        
        Shader(Shader&& shader);
        Shader& operator= (Shader&& shader);
        
        virtual ~Shader(void);
        
        /**
        * @brief            Loads the pre-compiled shader file (file extension: .spv)
        * @param[in] path   path to the shader file
        * @param[in] stage  shader stage (vertex, geometry, fragment,...)
        * @return           vulkan result
        */
        VkResult load(const std::string& path, VkShaderStageFlagBits stage);
        
        /** @brief Clears the shader. The shader can be reloaded again. */
        void clear(void);
        
        /** @param[in] device logical device */
        void set_device(VkDevice device) noexcept;
        
        /** @param[in] entry_point shader entry point */
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
        ShaderProgram(void) noexcept = default;
        
        /**
        * @brief                Constructor to attach multiple shaders.
        * @param[in] shaders    shaders to attach
        */
        explicit ShaderProgram(std::vector<Shader>& shaders);
        
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator= (const ShaderProgram&) = delete;
        
        ShaderProgram(ShaderProgram&&) = delete;
        ShaderProgram& operator= (ShaderProgram&&) = delete;
        
        virtual ~ShaderProgram(void) = default;
        
        /**
        * @brief                Attaches a shader to the shader program.
        * @param[in] shader     shader to attach
        */
        void attach(Shader& shader);
        
        /** @brief Cleares the shader program. No shaders are attached anymore after clear. */
        void clear(void) noexcept;
        
        /** @return amount of shaders attached to the shader program */
        uint32_t count(void) const noexcept;
        
        /** 
         * @return  array of VkPipelineShaderStageCreateInfo structs for the rendering pipeline 
         *          The returned create info structs are in the same order as the shaders were attached.
         */
        const VkPipelineShaderStageCreateInfo* get_stages(void) const noexcept;
    };

    class Shader2
    {
    private:
        static constexpr const char FILE_OPEN_FAILED[] = "[vka::Shader::create]: Failed to open shader file.";
        static constexpr const char SHADER_CREATE_FAILED[] = "[vka::Shader::create]: Failed to create shader module.";

        VkDevice m_device;
        VkShaderModule m_module;        // this is the shader handle

        /*
        * Checks if everything is correct at creation. Throws exceptions if anything was wrong
        * initialized, or was not initialized.
        */
        inline void validate(void);
        inline void destroy_handles(void) noexcept;

    public:
        /*
        * Initialization constructor which initializes 'this' with a device. The device does not
        * have to be valid at initialization, it must be valid at creation. This constructor has
        * the same functionality as the .init() function. Furthermore, this constructor is also
        * used as the default constructor, where the device is initialized to a VK_NULL_HANDLE.
        * All other handles are initialized to a VK_NULL_HANDLE and every other member variable
        * contains its default initialization.
        */
        explicit Shader2(VkDevice device = VK_NULL_HANDLE);

        // There is no need for a shader to be copyable.
        Shader2(const Shader2&) = delete;
        Shader2& operator= (const Shader2&) = delete;

        /*
        * Moves another object of Shader into 'this'. 'This' now holds the ownership of all the
        * handles of the source object. The source object will become invalidated and contains its
        * default initialization except for the device. The device will be copied and is preserved
        * in the moved object. If 'this' was created and is a valid object, 'this' is destroyed and
        * replaced by the handles of the moved object.
        */
        Shader2(Shader2&& src) noexcept;
        Shader2& operator= (Shader2&& src) noexcept;

        // The destructor destroys all the vulkan handles.
        virtual ~Shader2(void);

        /*
        * Initializes 'this' with a device. The device does not have to be valid at initialization.
        * However, it must be valid at creation. The initialization cannot be changed, if the
        * Shader is a valid object.
        */
        void init(VkDevice device) noexcept;

        /*
        * This function creates the Shader and the internal handles are now valid, if no error
        * occured. If an error occured while creating, a std::runtime_error exception is thrown
        * with an appropriate message about the error. The shader code is loaded from a file and
        * the path to the file is specified by 'path'. The shader stage is specified by 'stage'.
        * Optionally, a custom entry point for the shader program can be specified by 'entry' which
        * is "main" by default. A std::invalid_argument exception is thrown, if 'this' has not been
        * initialized.
        */
        void create(const char* path);

        /*
        * Destroys the Shader object. After destroying, 'this' holds its default initialization
        * except for the device. The device will be preserved after destroying and 'this' does not
        * need to be reinitialized. This is also done by the destructor.
        */
        void destroy(void) noexcept;

        // Returns the shader module handle.
        inline VkShaderModule handle(void) const noexcept;

        // Returns true, if the Shader is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;
    };
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_shader_impl.inl"
    #include "vka_shader_program_impl.inl"
#endif
#include "vka_shader_inline_impl.inl"
