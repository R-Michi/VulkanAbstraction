/**
* @file     vka_error.h
* @brief    Implementation of error handling functionality.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

void vka::detail::error::throw_invalid_argument(const char* msg)
{
    throw std::invalid_argument(msg);
}

void vka::detail::error::throw_runtime_error(const char* msg)
{
    throw std::runtime_error(msg);
}