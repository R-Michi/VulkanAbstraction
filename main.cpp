/**
* @file     main.cpp
* @brief    Main file.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#define STB_IMAGE_IMPLEMENTATION
#define VKA_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include "VulkanApp.h"

#include <string.h>
#include <iostream>

int main()
{
    VulkanApp app;
    try
    {
        app.init();
        app.run();
        app.shutdown();
    }
    catch (std::exception& e)
    {
        std::cerr << "Runtime error ocurred!\nWhat: " << e.what() << std::endl;
    }
    return 0;
}