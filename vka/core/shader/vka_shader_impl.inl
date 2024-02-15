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

void vka::Shader::destroy_handles(void) noexcept
{
    if (this->m_module != VK_NULL_HANDLE)
        vkDestroyShaderModule(this->m_device, this->m_module, nullptr);
}

void vka::Shader::internal_create(const std::string& path)
{
    // read shader file
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) [[unlikely]] // it is expexted that the file exists
        detail::error::throw_runtime_error(FILE_OPEN_FAILED);

    // get size of file and set cursor to begin of file
    size_t file_size = file.tellg();
    file.seekg(0, std::ifstream::beg);

    // allocate buffer and read file
    // use unique ptr for memory guard
    std::unique_ptr<char> code(new char[file_size]);
    file.read(code.get(), (std::streamsize)file_size);
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
