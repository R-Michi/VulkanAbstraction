/**
* @file     main.cpp
* @brief    Main file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#define STB_IMAGE_IMPLEMENTATION
#define VKA_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include "vka_example.h"
#include <iostream>

int main()
{
    // test for compiler errors
#if 0
    constexpr VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
    constexpr float data[4] = { 0xFF, 0x00, 0x00, 0xFF };
    constexpr VkExtent3D extent3D = { 100, 100, 100 };
    constexpr VkExtent2D extent2D = { 100, 100 };
    constexpr const char* filename = "test.png";

    vka::TextureComponentMerger<format> merger1(data, format, extent3D);
    vka::TextureComponentMerger<format> merger2(data, 3, extent3D);
    vka::TextureComponentMerger<format> merger3(filename);
    vka::TextureComponentMerger merger4 = std::move(merger3);
    merger3 = std::move(merger2);
    const float* tmp1 = merger1.data();
    VkExtent3D tmp2 = merger1.extent();
    merger1.load(data, format);
    merger1.load(data, 3);
    merger1.load(filename);

    vka::Texture3DMerger<format> merger3D1(data, format, extent2D);
    vka::Texture3DMerger<format> merger3D2(data, 3, extent2D);
    vka::Texture3DMerger<format> merger3D3(filename);
    vka::Texture3DMerger merger3D4 = std::move(merger3D3);
    merger3 = std::move(merger2);
    tmp1 = merger1.data();
    tmp2 = merger1.extent();
    merger1.load(data, format);
    merger1.load(data, 3);
    merger1.load(filename);

    vka::TextureLoader<format> loader1(data, format, extent3D);
    vka::TextureLoader<format> loader2(data, 3, extent3D);
    vka::TextureLoader<format> loader3(filename);
    vka::TextureLoader loader4 = std::move(loader3);
    merger3 = std::move(merger2);
    tmp1 = merger1.data();
    tmp2 = merger1.extent();
    merger1.load(data, format);
    merger1.load(data, 3);
    merger1.load(filename);
#endif

    VkaExample app;
    try
    {
        app.init();
        app.run();
        app.shutdown();
    }
    catch (std::exception& e)
    {
        std::cerr << "Runtime error occurred!\nWhat: " << e.what() << std::endl;
    }
    return 0;
}