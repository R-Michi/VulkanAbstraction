# requiered CMAKE version to build the project
cmake_minimum_required (VERSION 3.8)

# current project
project("VulkanAbstraction")

set(COMPILE_OPTIONS -mf16c -mfma -mavx2 -save-temps -masm=intel)
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

# test soruces
set(TEST_SOURCES
        test/format_sizeof_asm.cpp
        #vka/detail/descriptor/vka_descriptor.h
        #vka/detail/descriptor/vka_descriptor_inline_impl.inl
        #vka/core/descriptor/vka_descriptor_inline_impl.inl
)

# compile and link executable
add_executable(vka
        main.cpp
        VulkanApp.cpp
        vka/core/push_constant/vka_push_constant.h
        #vka/core/common/vka_common.h
        #vka/core/common/vka_common_impl.inl
        #vka/core/common/vka_common_inline_impl.inl
        #vka/core/format/vka_format.h
        #vka/core/format/vka_format_inline_impl.inl
        #vka/core/format/vka_format_impl.inl
        #vka/detail/common/vka_common.h
        #vka/detail/common/vka_common_inline_impl.inl
        #vka/detail/format/vka_format.h
        #vka/detail/format/vka_format_inline_impl.inl
        ${TEST_SOURCES}
        vka/detail/descriptor/vka_descriptor.h
        vka/core/push_constant/vka_push_constant_inline_impl.inl
        vka/detail/push_constant/vka_push_constant.h
        vka/detail/push_constant/vka_push_constant_impl.inl
        vka/detail/push_constant/vka_push_constant_inline_impl.inl
        vka/core/swapchain/vka_swapchain_impl.inl
        vka/core/common/vka_common_impl.inl
)
target_link_libraries(vka Vulkan::Vulkan ${GLFW_LIBRARY})

# custom command to compile shaders while compiling the program
#add_custom_command(
#    TARGET vka
#    PRE_BUILD COMMAND "compile_shader.bat"
#    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#)
