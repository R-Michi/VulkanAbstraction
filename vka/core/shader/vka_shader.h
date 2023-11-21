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
        explicit Shader(VkDevice device = VK_NULL_HANDLE);

        // There is no need for a shader to be copyable.
        Shader(const Shader&) = delete;
        Shader& operator= (const Shader&) = delete;

        /*
        * Moves another object of Shader into 'this'. 'This' now holds the ownership of all the
        * handles of the source object. The source object will become invalidated and contains its
        * default initialization except for the device. The device will be copied and is preserved
        * in the moved object. If 'this' was created and is a valid object, 'this' is destroyed and
        * replaced by the handles of the moved object.
        */
        Shader(Shader&& src) noexcept;
        Shader& operator= (Shader&& src) noexcept;

        // The destructor destroys all the vulkan handles.
        virtual ~Shader(void);

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
        void create(const std::string& path);

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

        /*
        * Generates a pipeline shader stage from this shader. 'stage' specifies the stage that is used
        * for this shader. 'flags' specifies the used flags for this shader stage which are 0 by
        * default. 'entry' specifies the entry point (name of the main function) of the shader which
        * is "main" by default. 'specialization' specifies additional specialization info used for the
        * shader stage which is 'nullptr' (unused) by default. A Vulkan VkPipelineShaderStageCreateInfo
        * structure is returned.
        * NOTE: For more information also see the vulkan documentation of VkPipelineShaderStageCreateInfo.
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
