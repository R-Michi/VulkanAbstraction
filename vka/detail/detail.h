/**
 * @brief Main include-file of the detail implementation.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

#include "common/common.inl"
#include "device/device.h"
#include "error/error.inl"
#include "format/format.inl"
#include "instance/instance.h"
#include "push_constant/push_constant.inl"
#include "queue/queue.inl"
#include "texture/texture.h"
#include "descriptor/descriptor.inl"
#include "guard/guard.h"

#ifdef VKA_MODEL_LOADING_ENABLE
    #include "model/vka_model.h"
#endif
