/**
* @file     vka.h
* @brief    Main include-file for the vulkan abstraction.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
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

#ifndef GLFW_INCLUDE_VULKAN
    #define GLFW_INCLUDE_VULKAN
#endif

// definition files
#include "instance/vka_instance.h"
#include "device/vka_device.h"
#include "queue/vka_queue.h"
#include "swapchain/vka_swapchain.h"
#include "shader/vka_shader.h"
#include "buffer/vka_buffer.h"
#include "texture/vka_texture.h"
#include "attachment/vka_attachment.h"
#include "utility/vka_utility.h"
#include "descriptor/vka_descriptor.h"

#ifdef VKA_EXPERIMENTAL
#include "model/vka_model.h"
#endif

// implementation files
#ifdef VKA_IMPLEMENTATION

#include "instance/vka_instance_impl.h"
#include "device/vka_device_impl.h"
#include "queue/vka_queue_impl.h"
#include "swapchain/vka_swapchain_impl.h"
#include "shader/vka_shader_impl.h"
#include "shader/vka_shader_program_impl.h"
#include "buffer/vka_buffer_impl.h"
#include "texture/vka_texture_impl.h"
#include "attachment/vka_attachment_impl.h"
#include "utility/vka_utility_impl.h"
#include "descriptor/vka_descriptor_manager_impl.h"

#endif



// template implementation files
#ifdef VKA_TEMPLATE_IMPLEMENTATION
#ifdef VKA_EXPERIMENTAL

#include "model/vka_vertex_impl.h"
#include "model/vka_mesh_impl.h"
#include "model/vka_model_impl.h"

#endif
#endif
