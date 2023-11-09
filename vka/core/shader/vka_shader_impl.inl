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

vka::Shader::Shader(void) noexcept : Shader(VK_NULL_HANDLE) {}

vka::Shader::Shader(VkDevice device, const std::string& entry_point) noexcept
{
    this->_device = device;
    this->_entry_point = nullptr;
    this->set_entry_point(entry_point);
    this->_stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    this->_module = VK_NULL_HANDLE;
    this->_size = 0;
}

vka::Shader::~Shader(void)
{
    this->clear();
    delete[] this->_entry_point;
}

vka::Shader::Shader(Shader&& shader) : Shader()
{
    *this = std::move(shader);
}

vka::Shader& vka::Shader::operator= (Shader&& shader)
{
    this->_device = shader._device;
    this->_entry_point = shader._entry_point;
    this->_stage = shader._stage;
    this->_module = shader._module;
    this->_size = shader._size;
    shader._entry_point = nullptr; // the entry point string must not get destroyed, only the ownership of the pointer must be transfered
    shader._module = VK_NULL_HANDLE;
    shader.clear();

    return *this;
}

VkResult vka::Shader::load(const std::string& path, VkShaderStageFlagBits stage)
{
    if (this->_module != VK_NULL_HANDLE)
        throw std::runtime_error("[vka::Shader::load]: Shader already loaded!");

    if (this->_device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::Shader::load]: Device of Shader is VK_NULL_HANDLE!");

    std::fstream shader_file(path, std::ios::in | std::ios::binary | std::ios::ate);    // read file backwards
    if (!shader_file)
        throw std::invalid_argument("[vka::Shader::load]: Path to shader-file not found! (\"" + path + "\")");

    this->_stage = stage;

    this->_size = shader_file.tellg();  // get cursor position (last position = number of bytes in that file)
    shader_file.seekg(0);               // set cursor at first character (begin of the file)

    char* byte_buff = new char[this->_size];
    shader_file.read(byte_buff, this->_size);
    shader_file.close();

    VkShaderModuleCreateInfo shader_module_create_info = {};
    shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.pNext = nullptr;
    shader_module_create_info.flags = 0;
    shader_module_create_info.codeSize = this->_size;
    shader_module_create_info.pCode = reinterpret_cast<uint32_t*>(byte_buff);

    return vkCreateShaderModule(this->_device, &shader_module_create_info, nullptr, &this->_module);
}

void vka::Shader::clear(void)
{
    if(this->_module != VK_NULL_HANDLE)
    {
        if (this->_device == VK_NULL_HANDLE)
            throw std::invalid_argument("[vka::Shader::clear]: Device of Shader is VK_NULL_HANDLE!");

        vkDestroyShaderModule(this->_device, this->_module, nullptr);
        this->_module = VK_NULL_HANDLE;
    }

    this->_size = 0;
    this->set_entry_point("main");
    this->_stage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
}

void vka::Shader::set_device(VkDevice device) noexcept
{
    this->_device = device;
}

void vka::Shader::set_entry_point(const std::string& entry_point) noexcept
{
    if(this->_entry_point != nullptr)    // if an entry point already exists, delete the old one
        delete[] this->_entry_point;
    
    // construct new string
    this->_entry_point = new char[entry_point.size() + 1];   // +1 because of \0
    memcpy(this->_entry_point, entry_point.data(), entry_point.size() + 1);
}

VkShaderModule vka::Shader::get_module(void) const noexcept
{
    return this->_module;
}

VkShaderStageFlagBits vka::Shader::get_stage(void) const noexcept
{
    return this->_stage;
}

const char* vka::Shader::get_entry_point(void) const noexcept
{
    return this->_entry_point;
}

size_t vka::Shader::size(void) const noexcept
{
    return this->_size;
}



vka::Shader2::Shader2(VkDevice device)
    : m_device(device), m_module(VK_NULL_HANDLE)
{}

vka::Shader2::Shader2(Shader2&& src) noexcept
    : m_device(src.m_device), m_module(src.m_module)
{
    src.m_module = VK_NULL_HANDLE;
}

vka::Shader2& vka::Shader2::operator= (Shader2&& src) noexcept
{
    // destroy 'this' shader, if it has been created, otherwise this function does nothing
    this->destroy_handles();
    this->m_device = src.m_device;
    this->m_module = src.m_module;
    src.m_module = VK_NULL_HANDLE;
    return *this;
}

vka::Shader2::~Shader2(void)
{
    this->destroy_handles();
}

void vka::Shader2::init(VkDevice device) noexcept
{
    if (!this->is_valid())
        this->m_device = device;
}

void vka::Shader2::create(const std::string& path)
{
    if (!this->is_valid())
    {
        this->validate();

        // read shader file
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file)
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

void vka::Shader2::destroy(void) noexcept
{
    this->destroy_handles();
    this->m_module = VK_NULL_HANDLE;
}
