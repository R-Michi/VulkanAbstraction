/**
* @file     vka.h
* @brief    Main include-file for the vulkan abstraction.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/*
* Checklist:
*   - descriptor
*   - NEW: push constants
*   - model
*/

#pragma once

// VKA_INCLUDE_GLFW includes glfw.
// VKA_INCLUDE_TINYOBJ includes the tiny-object-loader library.
// VKA_INCLUDE_STB_IMAGE includes stb_image.h.
// 
// VKA_GLFW_ENABLE enables all functions that use GLFW. GLFW must be included externally before
// including vka.h or by defining VKA_INCLUDE_GLFW.
// 
// VKA_MODEL_LOADING_ENABLE enables the model loading module. The tiny-object-loader library must
// be included externally before including vka.h or by defining VKA_INCLUDE_TINYOBJ.
// 
// VKA_STB_IMAGE_LOAD_ENABLE enables texture functions to load images into memory. stb_image.h must
// be included externally before including vka.h or by defining VKA_INCLUDE_STB_IMAGE.
// 
// VKA_MODEL_USE_DOUBLE forces tiny-object to use double as data type instead of float. Can only be
// activated, if the tiny-object-loader library is included and model loading is activated.
//

#ifdef VKA_DEBUG
    #ifdef _MSVC_VER
        #define VULKAN_ASSERT(result)\
        if(result != VK_SUCCESS)\
            __debugbreak();
    #else
        #define VULKAN_ASSERT(result)\
        if(result != VK_SUCCESS)\
            asm("int $3")   // causes a debug break (sort of)
    #endif
#else
    #define VULKAN_ASSERT(result)
#endif

#ifndef VULKAN_H_
    #error [VKA]: vulkan.h must be included before including vka.h.
#endif

#if defined(VKA_GLFW_ENABLE) && defined(VKA_INCLUDE_GLFW)
    #include <GLFW/glfw3.h>
#endif

#if defined(VKA_MODEL_LOADING_ENABLE) && defined(VKA_INCLUDE_TINYOBJ)
    #ifdef VKA_MODEL_USE_DOUBLE
        #define TINYOBJLOADER_USE_DOUBLE
    #endif
    #include <tiny_obj_loader.h>
#endif

#if defined(VKA_STB_IMAGE_LOAD_ENABLE) && defined(VKA_INCLUDE_STB_IMAGE)
    #ifdef __clang__
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wempty-body"
            #include <stb/stb_image.h>
        #pragma clang diagnostic pop
    #else
        #include <stb/stb_image.h>
    #endif
#endif
 
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <stdexcept>
#include <cinttypes>
#include <initializer_list>
#include <algorithm>

#include "core/vka_global.h"
#include "core/vka_types.h"

#include "detail/error/vka_error.h"

#include "core/instance/vka_instance.h"
#include "core/device/vka_device.h"
#include "core/queue/vka_queue.h"
#include "core/common/vka_common.h"
#include "core/memory/vka_memory.h"
#include "core/format/vka_format.h"
#include "core/surface/vka_surface.h"
#include "core/swapchain/vka_swapchain.h"
#include "core/shader/vka_shader.h"
#include "core/buffer/vka_buffer.h"
#include "core/texture/vka_texture.h"
#include "core/attachment/vka_attachment.h"
#include "core/descriptor/vka_descriptor.h"

#ifdef VKA_MODEL_LOADING_ENABLE
    #include "core/model/vka_model.h"
#endif
