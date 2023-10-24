# requiered CMAKE version to build the project
cmake_minimum_required (VERSION 3.8)

# current project
project("VulkanAbstraction")

set(COMPILE_OPTIONS -mfma -mavx2 -mf16c)
set(COMPILE_WARNINGS -Walloca -Wall)

# use C++ 20
set(CMAKE_CXX_STANDARD 20)
add_compile_options(${COMPILE_OPTIONS} ${COMPILE_WARNINGS})

# export compiler commands
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# find vulkan library
find_package(Vulkan)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan not found but required.")
endif()
message(STATUS "Vulkan found. Version: ${Vulkan_VERSION}")

# find GLFW library
find_library(GLFW_LIBRARY NAMES "glfw3.lib" PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/lib")
if(NOT GLFW_LIBRARY)
    message(FATAL_ERROR "GLFW3 not found but required.")
endif()
message(STATUS "GLFW3 found: ${GLFW_LIBRARY}")

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_master"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/tiny_obj_loader"
)

# compile and link executable
add_executable(vka "main.cpp" "VulkanApp.cpp")
target_link_libraries(vka Vulkan::Vulkan ${GLFW_LIBRARY})

# custom command to compile shaders while compiling the program
#add_custom_command(
#    TARGET vka
#    PRE_BUILD COMMAND "compile_shader.bat"
#    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#)
