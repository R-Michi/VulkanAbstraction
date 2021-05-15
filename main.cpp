#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "VulkanApp.h"
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
    catch (std::runtime_error& e)
    {
        std::cerr << "Runtime error ocurred!\nWhat: " << e.what() << std::endl;
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << "Invalid argument error occured!\nWhat: " << e.what() << std::endl;
    }
    return 0;
}