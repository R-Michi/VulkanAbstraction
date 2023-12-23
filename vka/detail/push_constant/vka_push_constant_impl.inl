/**
* @file     vka_push_constant.h
* @brief    Push constant implementation file of detailed functionality.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

void vka::detail::PushConstantBuffer::allocate(uint32_t size)
{
    if (this->m_data == nullptr)
    {
        this->m_data = malloc(size);
        error::check_memory(this->m_data);
        this->m_size = size;
    }
}

void vka::detail::PushConstantBuffer::free_memory(void) noexcept
{
    if (this->m_data != nullptr)
        std::free(this->m_data);
}
