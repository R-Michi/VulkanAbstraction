﻿# requiered CMAKE version to build the project
cmake_minimum_required (VERSION 3.8)

# current project
project("VulkanAbstraction")

# use C++ 17
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-mfma -mavx2 -mf16c)

# used include directories for libraries
include_directories(
    "C:/VulkanSDK/1.2.170.0/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_master"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/tiny_obj_loader"
)

# linker search directories for libraries
link_directories(
    "C:/VulkanSDK/1.2.170.0/Lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/lib"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm/glm/lib"
)

# compile and link executable
add_executable(vka "main.cpp" "VulkanApp.cpp")
target_link_libraries(vka PRIVATE 
    "-lglm_static" 
    "-lglfw3" 
    "-lvulkan-1"
)

# custom command to compile shaders while compiling the program
add_custom_command(
    TARGET vka
    PRE_BUILD COMMAND "compile_shader.bat"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# export compiler commands
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
