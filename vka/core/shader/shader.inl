/**
 * @brief Inline implementation for shader class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "shader.h"

constexpr vka::Shader::operator bool() const noexcept
{
    return (bool)this->m_module;
}

constexpr VkDevice vka::Shader::parent() const noexcept
{
    return this->m_module.parent();
}

constexpr VkShaderModule vka::Shader::handle() const noexcept
{
    return this->m_module.get();
}

constexpr void vka::Shader::destroy() noexcept
{
    this->m_module = VK_NULL_HANDLE;
}

constexpr VkPipelineShaderStageCreateInfo vka::Shader::make_stage(
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
        .module = this->m_module.get(),
        .pName = entry_point,
        .pSpecializationInfo = specialization
    };
}
