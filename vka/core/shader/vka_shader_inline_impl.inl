/**
* @file     vka_shader_impl.h
* @brief    Shader implemenation file of inline functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline vka::Shader::Shader(void) noexcept :
    m_device(VK_NULL_HANDLE),
    m_module(VK_NULL_HANDLE)
{}

inline vka::Shader::Shader(VkDevice device, const std::string& path) :
    m_device(device),
    m_module(VK_NULL_HANDLE)
{
    this->internal_create(path);
}

inline vka::Shader::Shader(Shader&& src) noexcept :
    m_device(src.m_device),
    m_module(src.m_module)
{
    src.m_module = VK_NULL_HANDLE;
}

inline vka::Shader& vka::Shader::operator= (Shader&& src) noexcept
{
    // destroy 'this' shader, if it has been created, otherwise this function does nothing
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_module = src.m_module;
    src.m_module = VK_NULL_HANDLE;
    return *this;
}

inline vka::Shader::~Shader(void)
{
    this->destroy_handles();
}

inline void vka::Shader::create(VkDevice device, const std::string& path)
{
    if (!this->is_valid())
    {
        this->m_device = device;
        this->internal_create(path);
    }
}

inline void vka::Shader::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_module = VK_NULL_HANDLE;
}

inline VkShaderModule vka::Shader::handle(void) const noexcept
{
    return this->m_module;
}

inline bool vka::Shader::is_valid(void) const noexcept
{
    return (this->m_module != VK_NULL_HANDLE);
}

inline VkPipelineShaderStageCreateInfo vka::Shader::make_stage(
    VkShaderStageFlagBits stage,
    VkPipelineShaderStageCreateFlags flags,
    const char* entry_point,
    const VkSpecializationInfo* specialization
) const noexcept
{
    return {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = flags,
        .stage = stage,
        .module = this->m_module,
        .pName = entry_point,
        .pSpecializationInfo = nullptr
    };
}
