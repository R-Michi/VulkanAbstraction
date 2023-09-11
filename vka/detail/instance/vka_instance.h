/**
* @file     vka_instance.h
* @brief    Detailed declaration of instance functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka
{
    namespace detail
    {
        namespace instance
        {
            inline size_t supports_layer(const VkLayerProperties* properties, uint32_t count, std::string_view layer_name) noexcept;
            inline size_t supports_extension(const VkExtensionProperties* properties, uint32_t count, std::string_view extension_name) noexcept;
        }
    }
}

#ifdef VKA_IMPLEMENTATION
#include "vka_instance_impl.inl"
#endif
