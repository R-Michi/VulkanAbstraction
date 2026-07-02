# required CMAKE version to build the project
cmake_minimum_required (VERSION 3.25)

# current project
project("VulkanAbstraction")

# This lib is built for C++ 20 and above
# It is not made backwards compatible to older versions.
set(CMAKE_CXX_STANDARD 20)

# Global compile warnings for all targets.
set(VKA_COMPILE_WARNINGS -Walloca -Wall)

# Global compile options for all targets.
add_compile_options(${VKA_COMPILE_WARNINGS})

# Useful variables
set(VKA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}) # use: #include <vka/vka.h>
set(VKA_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
message(STATUS "[VKA]: Include directory: ${VKA_INCLUDE_DIR}")
message(STATUS "[VKA]: Library directory: ${VKA_LIBRARY_DIR}")

# find vulkan library package
find_package(Vulkan COMPONENTS glslangValidator REQUIRED)
find_program(SPIRV_OPT REQUIRED NAMES "spirv-opt" PATHS "$ENV{VULKAN_SDK}/Bin")

# GLFW may already exist. If this is the case, it is not included a second time.
if (NOT TARGET glfw)
    # If GLFW does not exist already, it is included by this project. However, GLFW is not a requirement because this
    # library has two versions: one with GLFW and one without GLFW enabled. If GLFW still cannot be found, the version
    # of the library with GLFW enabled is not available.
    find_path(VKA_GLFW_PATH NAMES CMakeLists.txt PATHS ${VKA_LIBRARY_DIR}/glfw)
    if (VKA_GLFW_PATH)
        add_subdirectory(${VKA_GLFW_PATH})
        message(STATUS "[VKA]: Included GLFW as a submodule.")
    endif ()
endif ()

if (TARGET glfw)
    message(STATUS "[VKA]: Detected GLFW: ${GLFW_SOURCE_DIR}")
else ()
    message(WARNING "[VKA]: GLFW was not detected. Library \"vka_glfw\" is not available.")
endif ()

# Include STB image. STB image is a header-only library which has an implementation part. It must be compiled into one
# translation unit (.c / .cpp) unsing #define STB_IMAGE_IMPLEMENTATION. Since this is not its own project where all its
# files compile into one static library, we have to embed it in the VulkanAbstraction library. Otherwise, we would have
# an external dependency when using the VulkanAbstraction library.
set(VKA_STB_FILES
        lib/stb/stb.h
        lib/stb/stb_image.h
        lib/stb/implementation.cpp
)

########################################################################################################################
######################################################## FORWARD #######################################################
########################################################################################################################

set(VKA_FORWARD_FILES
        vka/forward/forward.h
        vka/forward/constants.h
        vka/forward/types.h
)

########################################################################################################################
######################################################## DETAIL ########################################################
########################################################################################################################

set(VKA_DETAIL_FILES
        vka/detail/detail.h
        vka/detail/common/common.h
        vka/detail/common/common.inl
        vka/detail/error/error.h
        vka/detail/error/error.inl
        vka/detail/error/error.cpp
        vka/detail/format/format.h
        vka/detail/format/format.inl
        vka/detail/instance/instance.h
        vka/detail/instance/instance.cpp
        vka/detail/device/device.h
        vka/detail/device/device.cpp
        vka/detail/queue/queue.h
        vka/detail/queue/queue.inl
        vka/detail/buffer/buffer.h
        vka/detail/buffer/buffer.inl
        vka/detail/attachment/attachment.h
        vka/detail/attachment/attachment.inl
        vka/detail/texture/texture.h
        vka/detail/texture/texture.inl
        vka/detail/handle/handle.h
        vka/detail/push_constant/push_constant.h
        vka/detail/push_constant/push_constant.inl
        vka/core/push_constant/push_constant.h
        vka/detail/descriptor/descriptor.h
        vka/detail/descriptor/descriptor.inl
        vka/detail/window/window.h
)

########################################################################################################################
######################################################### CORE #########################################################
########################################################################################################################

set(VKA_CORE_FILES
        vka/core/core.h
        vka/core/common/common.h
        vka/core/common/common.inl
        vka/core/common/common.cpp
        vka/core/error/error.h
        vka/core/error/error.inl
        vka/core/instance/instance.h
        vka/core/instance/instance.cpp
        vka/core/device/device.h
        vka/core/device/device.cpp
        vka/core/queue/queue.h
        vka/core/queue/queue.cpp
        vka/core/surface/surface.h
        vka/core/surface/surface.cpp
        vka/core/swapchain/swapchain.h
        vka/core/swapchain/swapchain.cpp
        vka/core/memory/memory.h
        vka/core/memory/memory.cpp
        vka/core/format/format.h
        vka/core/format/format.inl
        vka/core/format/format.cpp
        vka/core/attachment/attachment.h
        vka/core/attachment/attachment.inl
        vka/core/attachment/attachment.cpp
        vka/core/buffer/buffer.h
        vka/core/buffer/buffer.inl
        vka/core/buffer/buffer.cpp
        vka/core/shader/shader.h
        vka/core/shader/shader.inl
        vka/core/shader/shader.cpp
        vka/core/texture/top.h
        vka/core/texture/texture.h
        vka/core/texture/merger.inl
        vka/core/texture/loader.inl
        vka/core/texture/texture.inl
        vka/core/texture/texture.cpp
        vka/core/descriptor/top.h
        vka/core/descriptor/descriptor.h
        vka/core/descriptor/binding_list.inl
        vka/core/descriptor/binding_list.cpp
        vka/core/descriptor/layout.inl
        vka/core/descriptor/layout.cpp
        vka/core/descriptor/set.inl
        vka/core/descriptor/set.cpp
        vka/core/descriptor/update.inl
        vka/core/descriptor/update.cpp
        vka/core/push_constant/top.h
        vka/core/push_constant/push_constant.inl
        vka/core/push_constant/layout.inl
        vka/core/push_constant/view.inl
        vka/core/handle/handle.h
        vka/core/handle/unique_handle.h
        vka/core/handle/parent.inl
        vka/core/handle/no_parent.inl
        vka/core/handle/dynamic_array.inl
        vka/core/window/window.h
        vka/core/window/window.inl
        vka/core/window/window.cpp
)

########################################################################################################################
####################################################### VKA-FILES ######################################################
########################################################################################################################

set(VKA_FILES
        vka/vka.h
        vka/defs.h
        ${VKA_FORWARD_FILES}
        ${VKA_DETAIL_FILES}
        ${VKA_CORE_FILES}
        ${VKA_STB_FILES}
)

########################################################################################################################
####################################################### LIBRARIES ######################################################
########################################################################################################################

# Version of the library without GLFW enabled.
add_library(vka STATIC ${VKA_FILES})
target_link_libraries(vka PUBLIC Vulkan::Vulkan)
target_include_directories(vka PUBLIC ${VKA_INCLUDE_DIR})

# Version of the library with GLFW enabled.
# To remove dependencies GLFW and Vulkan are automatically linked when linking this target.
if (TARGET glfw)
    add_library(vka_glfw STATIC ${VKA_FILES})
    target_compile_options(vka_glfw PUBLIC -DVKA_GLFW_ENABLE)
    target_link_libraries(vka_glfw PUBLIC Vulkan::Vulkan glfw)
    target_include_directories(vka_glfw PUBLIC ${VKA_INCLUDE_DIR})
endif ()

########################################################################################################################
######################################################## EXAMPLE #######################################################
########################################################################################################################

set(VKA_EXAMPLE_FILES
        main.cpp
        vka_example.h
        vka_example.cpp
)

# compile and link executable
add_executable(vka_example ${VKA_EXAMPLE_FILES})
target_link_libraries(vka_example PRIVATE vka_glfw)
target_include_directories(vka_example PRIVATE ${VKA_LIBRARY_DIR}/glm ${VKA_LIBRARY_DIR})

# find all shader files that end with ".vert" and ".frag"
# More shader stages can be added, if required.
file(GLOB_RECURSE SHADER_FILES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        assets/shaders/*.vert
        assets/shaders/*.frag
)

# add custom compile command for each shader file
foreach(SHADER IN ITEMS ${SHADER_FILES})
    message(STATUS "[VKA]: Found shader file: ${SHADER}")  # print shader file location
    set(INPUT_SHADER "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}")   # input file path
    set(OUTPUT_SPV "${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.spv") # output file path
    add_custom_command(             # compile command
            OUTPUT ${OUTPUT_SPV}    # generates the output file (-path), in fact this is also done by glslangValidator.exe
            COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${INPUT_SHADER} -o ${OUTPUT_SPV}  # calls the glsl compiler
            COMMAND ${SPIRV_OPT} -O ${OUTPUT_SPV} -o ${OUTPUT_SPV}                              # calls the glsl optimizer
            DEPENDS ${INPUT_SHADER} # creates the dependency, if the file has changed
            COMMENT "Building shader ${SHADER}"
    )
    list(APPEND SHADER_BINARY_FILES ${OUTPUT_SPV})
endforeach()

# Create the custom target for a build dependency.
# This target depends on the built shader binaries.
# Meaning, to create the 'Shaders' target, the binaries must have been created.
add_custom_target(Shaders DEPENDS ${SHADER_BINARY_FILES})
add_dependencies(vka_example Shaders) # Add dependency to targets
