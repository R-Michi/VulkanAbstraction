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

namespace vka::detail::instance
{
    /*
    * Checks, if a layer at instance level is supported and returns the index in the
    * 'properties' array. If the layer is not supported, vka::NPOS is returned.
    */
    inline size_t supports_layer(const VkLayerProperties* properties, uint32_t count, std::string_view layer_name) noexcept;

    /*
    * Checks, if an extension at instance level is supported and returns the index in the
    * 'properties' array. If the extension is not supported, vka::NPOS is returned.
    */
    inline size_t supports_extension(const VkExtensionProperties* properties, uint32_t count, std::string_view extension_name) noexcept;
}

#include "vka_instance_inline_impl.inl"
