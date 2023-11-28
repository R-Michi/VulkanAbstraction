/**
* @file     vka_test.h
* @brief    In this file the generated asm code of unique_handle is analyzed.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include "vka_test.h"

VkResult create(const VkInstanceCreateInfo*, const VkAllocationCallbacks*, VkInstance*);

void destroy(const VkInstance, const VkAllocationCallbacks*);

inline void vka::test::unique_handle_asm(void) noexcept
{
    VkInstanceCreateInfo ci = {};
    VkInstance instance;
    create(&ci, nullptr, &instance);
#if 0
    unique_handle<VkInstance> h(instance);
#else
    //if (instance != VK_NULL_HANDLE)
        destroy(instance, nullptr);
#endif
}
