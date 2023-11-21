/**
* @file     vka_shader_impl.h
* @brief    Shader implemenation file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

vka::Shader::Shader(VkDevice device)
    : m_device(device), m_module(VK_NULL_HANDLE)
{}

vka::Shader::Shader(Shader&& src) noexcept
    : m_device(src.m_device), m_module(src.m_module)
{
    src.m_module = VK_NULL_HANDLE;
}

vka::Shader& vka::Shader::operator= (Shader&& src) noexcept
{
    // destroy 'this' shader, if it has been created, otherwise this function does nothing
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_module = src.m_module;
    src.m_module = VK_NULL_HANDLE;
    return *this;
}

vka::Shader::~Shader(void)
{
    this->destroy_handles();
}

void vka::Shader::init(VkDevice device) noexcept
{
    if (!this->is_valid())
        this->m_device = device;
}

void vka::Shader::create(const std::string& path)
{
    if (!this->is_valid())
    {
        this->validate();

        // read shader file
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) [[unlikely]] // it is expexted that the file exists
            detail::error::throw_runtime_error(FILE_OPEN_FAILED);

        // get size of file and set cursor to begin of file
        size_t file_size = file.tellg();
        file.seekg(0, file.beg);

        // allocate buffer and read file
        // use unique ptr for memory guard
        std::unique_ptr<char> code(new char[file_size]);
        file.read(code.get(), file_size);
        file.close();

        const VkShaderModuleCreateInfo ci = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .codeSize = file_size,
            .pCode = reinterpret_cast<uint32_t*>(code.get()) // shader code size is always a multiple of 4, its ok to cast from char* to uint32_t*
        };
        detail::error::check_result(vkCreateShaderModule(this->m_device, &ci, nullptr, &this->m_module), SHADER_CREATE_FAILED);
    }
}

void vka::Shader::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_module = VK_NULL_HANDLE;
}
