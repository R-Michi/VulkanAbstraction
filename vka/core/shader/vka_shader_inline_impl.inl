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

inline VkShaderModule vka::Shader::handle(void) const noexcept
{
    return this->m_module;
}

inline bool vka::Shader::is_valid(void) const noexcept
{
    return (this->m_module != VK_NULL_HANDLE);
}

inline void vka::Shader::validate(void)
{
    if (this->m_device == VK_NULL_HANDLE) [[unlikely]]
        detail::error::throw_invalid_argument("[vka::Shader::create]: Device is a VK_NULL_HANDLE.");
}

inline void vka::Shader::destroy_handles(void) noexcept
{
    if (this->m_module != VK_NULL_HANDLE)
        vkDestroyShaderModule(this->m_device, this->m_module, nullptr);
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
