/**
* @file     vka_shader_program_impl.h
* @brief    Shader program implemenation file.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "vka_shader.h"

vka::ShaderProgram::ShaderProgram(void) noexcept {}

vka::ShaderProgram::ShaderProgram(std::vector<Shader>& shaders)
{
    for (Shader& shader : shaders)
        this->attach(shader);
}

vka::ShaderProgram::~ShaderProgram(void) noexcept {}

void vka::ShaderProgram::attach(Shader& shader)
{
    if (shader.get_module() == VK_NULL_HANDLE)
        throw std::invalid_argument("Module of Shader is a VK_NULL_HANDLE! Requiered from shader program attach.");

    VkPipelineShaderStageCreateInfo shader_stage_create_info = {};
    shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_create_info.pNext = nullptr;
    shader_stage_create_info.flags = 0;
    shader_stage_create_info.stage = shader.get_stage();
    shader_stage_create_info.module = shader.get_module();
    shader_stage_create_info.pName = shader.get_entry_point();
    shader_stage_create_info.pSpecializationInfo = nullptr;
    
    this->_shaders.push_back(std::move(shader));
    this->_shader_stages.push_back(shader_stage_create_info);
}

void vka::ShaderProgram::clear(void) noexcept
{
    this->_shaders.clear();
    this->_shader_stages.clear();
}

uint32_t vka::ShaderProgram::count(void) const noexcept
{
    return this->_shader_stages.size();
}

const VkPipelineShaderStageCreateInfo* vka::ShaderProgram::get_stages(void) const noexcept
{
    if (this->_shader_stages.size() == 0)
        return nullptr;
    return this->_shader_stages.data();
}
