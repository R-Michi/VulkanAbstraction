/**
 * @brief Includes macros used by vka.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#pragma once

#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <fstream>
#include <vulkan/vulkan.h>
#include "../lib/stb/stb.h"

#ifdef VKA_GLFW_ENABLE
    #include <GLFW/glfw3.h>
#endif

/// ------------------------------------------------- Hardware features ------------------------------------------------

/// detects X86 64-bit architecture
#if defined(__x86_64__) || defined(_M_X64)
    #define VKA_X86_64
#endif

/// detects X86 32-bit architecture
#if defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    #define VKA_X86_32
#endif

/// detects X86 architecture (32-bit or 64-bit)
#if defined(VKA_X86_32) || defined(VKA_X86_64)
    #define VKA_X86
#endif

/// ---------------------------------------- Hardware feature dependent includes ---------------------------------------

#ifdef VKA_X86
    #include <x86intrin.h>
#endif

/// --------------------------------------------------- User defines ---------------------------------------------------

/**
 * @brief USER DEFINE: VKA_INCLUDE_TINYOBJ includes the tiny-object-loader library\n
 * @brief USER DEFINE: VKA_MODEL_LOADING_ENABLE enables the model loading module. The tiny-object-loader library must be
 * included externally before including vka.h or by defining VKA_INCLUDE_TINYOBJ.
 * TODO: This is old code
 */
#if defined(VKA_MODEL_LOADING_ENABLE) && defined(VKA_INCLUDE_TINYOBJ)
    #ifdef VKA_MODEL_USE_DOUBLE
        #define TINYOBJLOADER_USE_DOUBLE
    #endif
    #include <tiny_obj_loader.h>
#endif

/**
 * @brief VKA_MODEL_USE_DOUBLE forces the tiny-object library to use double as the default data type instead of float.
 * Can only be activated if the tiny-object-loader library is included and model loading is activated.
 * TODO: This is old code
 */
// USE: #define VKA_MODEL_USE_DOUBLE
