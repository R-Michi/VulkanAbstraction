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

#pragma once

#ifdef VKA_DEBUG
    #ifdef _MSVC_VER
        #define VULKAN_ASSERT(result)\
        if(result != VK_SUCCESS)\
            __debugbreak();
    #else
        #define VULKAN_ASSERT(result)\
        if(result != VK_SUCCESS) \
            asm("int $3")   // causes a debug break (sort of)
    #endif
#else
    #define VULKAN_ASSERT(result)
#endif

#ifndef VULKAN_H_
    #error [VKA]: vulkan.h must be included before including vka.h.
#endif

#ifndef VKA_GLFW_DISABLE
    #include <GLFW/glfw3.h>
#endif

#include <tiny_obj_loader.h>
 
#include <vector>
#include <string>
#include <stdexcept>
#include <cinttypes>
#include <initializer_list>
#include <algorithm>
#include <unordered_map>

#include "core/vka_global.h"
#include "core/vka_types.h"

#include "core/instance/vka_instance.h"
#include "core/device/vka_device.h"
#include "core/queue/vka_queue.h"
#include "core/swapchain/vka_swapchain.h"
#include "core/utility/vka_utility.h"
#include "core/shader/vka_shader.h"
#include "core/descriptor/vka_descriptor.h"
#include "core/buffer/vka_buffer.h"
#include "core/attachment/vka_attachment.h"
#include "core/texture/vka_texture.h"
#include "core/model/vka_model.h"
