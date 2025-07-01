/**
 * @brief Main include-file of the core implementation.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "error/error.h"
#include "memory/memory.h"
#include "attachment/attachment.h"
#include "buffer/buffer.h"
#include "common/common.h"
#include "device/device.h"
#include "format/format.h"
#include "instance/instance.h"
#include "push_constant/push_constant.h"
#include "queue/queue.h"
#include "shader/shader.h"
#include "surface/surface.h"
#include "swapchain/swapchain.h"
#include "texture/texture.h"
#include "descriptor/descriptor.h"

#ifdef VKA_MODEL_LOADING_ENABLE
    #include "model/vka_model.h"
#endif
