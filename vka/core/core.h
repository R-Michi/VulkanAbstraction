/**
 * @brief Main include-file of the core implementation.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "error/error.inl"
#include "memory/memory.h"
#include "attachment/attachment.inl"
#include "buffer/buffer.inl"
#include "common/common.inl"
#include "device/device.h"
#include "format/format.inl"
#include "instance/instance.h"
#include "push_constant/push_constant.h"
#include "queue/queue.h"
#include "shader/shader.inl"
#include "surface/surface.h"
#include "swapchain/swapchain.h"
#include "texture/texture.inl"
#include "descriptor/descriptor.h"

#ifdef VKA_MODEL_LOADING_ENABLE
    #include "model/vka_model.h"
#endif
