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

inline VkShaderModule vka::Shader2::handle(void) const noexcept
{
    return this->m_module;
}

inline bool vka::Shader2::is_valid(void) const noexcept
{
    return (this->m_module != VK_NULL_HANDLE);
}

inline void vka::Shader2::validate(void)
{
    if (this->m_device == VK_NULL_HANDLE) [[unlikely]]
        detail::error::throw_invalid_argument("[vka::Shader::create]: Device is a VK_NULL_HANDLE.");
}

inline void vka::Shader2::destroy_handles(void) noexcept
{
    if (this->m_module != VK_NULL_HANDLE)
        vkDestroyShaderModule(this->m_device, this->m_module, nullptr);
}
