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
set(VKA_INCLUDE_DIR ${VulkanAbstraction_SOURCE_DIR}) # use: #include <vka/vka.h>

# find vulkan library package
find_package(Vulkan REQUIRED)
find_program(SPIRV_OPT REQUIRED NAMES "spirv-opt" PATHS "$ENV{VULKAN_SDK}/Bin")

# glfw library
if (NOT TARGET glfw)
    find_path(VKA_SUBMODULE_GLFW NAMES glfw PATHS ${VulkanAbstraction_SOURCE_DIR}/lib REQUIRED)
    add_subdirectory(${VulkanAbstraction_SOURCE_DIR}/lib/glfw)
endif ()
set(VKA_GLFW_INCLUDE_DIR "${GLFW_SOURCE_DIR}/include")
message(STATUS "[VKA]: Detected GLFW: ${GLFW_SOURCE_DIR}")

########################################################################################################################
######################################################## FORWARD #######################################################
########################################################################################################################

set(VKA_FORWARD_FILES
        vka/forward/forward.h
        vka/forward/constants.h
        vka/forward/types.h
)

########################################################################################################################
##################################################### DETAIL/BUFFER ####################################################
########################################################################################################################

set(VKA_DETAIL_BUFFER_FILES
        vka/detail/buffer/buffer.h
        vka/detail/buffer/buffer.inl
)

########################################################################################################################
##################################################### DETAIL/COMMON ####################################################
########################################################################################################################

set(VKA_DETAIL_COMMON_FILES
        vka/detail/common/common.h
        vka/detail/common/common.inl
)

########################################################################################################################
################################################### DETAIL/DESCRIPTOR ##################################################
########################################################################################################################

set(VKA_DETAIL_DESCRIPTOR_FILES
        vka/detail/descriptor/descriptor.h
        vka/detail/descriptor/descriptor.inl
)

########################################################################################################################
##################################################### DETAIL/DEVICE ####################################################
########################################################################################################################

set(VKA_DETAIL_DEVICE_FILES
        vka/detail/device/device.h
        vka/detail/device/device.cpp
)

########################################################################################################################
##################################################### DETAIL/ERROR #####################################################
########################################################################################################################

set(VKA_DETAIL_ERROR_FILES
        vka/detail/error/error.h
        vka/detail/error/error.inl
        vka/detail/error/error.cpp
)

########################################################################################################################
##################################################### DETAIL/FORMAT ####################################################
########################################################################################################################

set(VKA_DETAIL_FORMAT_FILES
        vka/detail/format/format.h
        vka/detail/format/format.inl
)

########################################################################################################################
#################################################### DETAIL/INSTANCE ###################################################
########################################################################################################################

set(VKA_DETAIL_INSTANCE_FILES
        vka/detail/instance/instance.h
        vka/detail/instance/instance.cpp
)

########################################################################################################################
################################################# DETAIL/PUSH_CONSTANT #################################################
########################################################################################################################

set(VKA_DETAIL_PUSH_CONSTANT_FILES
        vka/detail/push_constant/push_constant.h
        vka/detail/push_constant/push_constant.inl
        vka/core/push_constant/push_constant.h
)

########################################################################################################################
##################################################### DETAIL/QUEUE #####################################################
########################################################################################################################

set(VKA_DETAIL_QUEUE_FILES
        vka/detail/queue/queue.h
        vka/detail/queue/queue.inl
)

########################################################################################################################
#################################################### DETAIL/TEXTURE ####################################################
########################################################################################################################

set(VKA_DETAIL_TEXTURE_FILES
        vka/detail/texture/texture.h
)

########################################################################################################################
#################################################### DETAIL/HANDLE #####################################################
########################################################################################################################

set(VKA_DETAIL_HANDLE_FILES
        vka/detail/handle/handle.h
)

########################################################################################################################
##################################################### DETAIL/MODEL #####################################################
########################################################################################################################

set(VKA_DETAIL_MODEL_FILES
)

########################################################################################################################
######################################################## DETAIL ########################################################
########################################################################################################################

set(VKA_DETAIL_FILES
        ${VKA_DETAIL_BUFFER_FILES}
        ${VKA_DETAIL_COMMON_FILES}
        ${VKA_DETAIL_DESCRIPTOR_FILES}
        ${VKA_DETAIL_DEVICE_FILES}
        ${VKA_DETAIL_ERROR_FILES}
        ${VKA_DETAIL_FORMAT_FILES}
        ${VKA_DETAIL_INSTANCE_FILES}
        ${VKA_DETAIL_PUSH_CONSTANT_FILES}
        ${VKA_DETAIL_QUEUE_FILES}
        ${VKA_DETAIL_TEXTURE_FILES}
        ${VKA_DETAIL_HANDLE_FILES}
        ${VKA_DETAIL_MODEL_FILES}
        vka/detail/detail.h
)

########################################################################################################################
###################################################### CORE/COMMON #####################################################
########################################################################################################################

set(VKA_CORE_COMMON_FILES
        vka/core/common/common.h
        vka/core/common/common.inl
        vka/core/common/common.cpp
)

########################################################################################################################
###################################################### CORE/ERROR ######################################################
########################################################################################################################

set(VKA_CORE_ERROR_FILES
        vka/core/error/error.h
        vka/core/error/error.inl
)

########################################################################################################################
##################################################### CORE/INSTANCE ####################################################
########################################################################################################################

set(VKA_CORE_INSTANCE_FILES
        vka/core/instance/instance.h
        vka/core/instance/instance.cpp
)

########################################################################################################################
###################################################### CORE/DEVICE #####################################################
########################################################################################################################

set(VKA_CORE_DEVICE_FILES
        vka/core/device/device.h
        vka/core/device/device.cpp
)

########################################################################################################################
###################################################### CORE/QUEUE ######################################################
########################################################################################################################

set(VKA_CORE_QUEUE_FILES
        vka/core/queue/queue.h
        vka/core/queue/queue.cpp
)

########################################################################################################################
##################################################### CORE/SURFACE #####################################################
########################################################################################################################

set(VKA_CORE_SURFACE_FILES
        vka/core/surface/surface.h
        vka/core/surface/surface.cpp
)

########################################################################################################################
#################################################### CORE/SWAPCHAIN ####################################################
########################################################################################################################

set(VKA_CORE_SWAPCHAIN_FILES
        vka/core/swapchain/swapchain.h
        vka/core/swapchain/swapchain.cpp
)

########################################################################################################################
###################################################### CORE/MEMORY #####################################################
########################################################################################################################

set(VKA_CORE_MEMORY_FILES
        vka/core/memory/memory.h
        vka/core/memory/memory.cpp
)

########################################################################################################################
###################################################### CORE/FORMAT #####################################################
########################################################################################################################

set(VKA_CORE_FORMAT_FILES
        vka/core/format/format.h
        vka/core/format/format.inl
        vka/core/format/format.cpp
)

########################################################################################################################
#################################################### CORE/ATTACHMENT ###################################################
########################################################################################################################

set(VKA_CORE_ATTACHMENT_FILES
        vka/core/attachment/attachment.h
        vka/core/attachment/attachment.inl
        vka/core/attachment/attachment.cpp
)

########################################################################################################################
###################################################### CORE/BUFFER #####################################################
########################################################################################################################

set(VKA_CORE_BUFFER_FILES
        vka/core/buffer/buffer.h
        vka/core/buffer/buffer.inl
        vka/core/buffer/buffer.cpp
)

########################################################################################################################
###################################################### CORE/SHADER #####################################################
########################################################################################################################

set(VKA_CORE_SHADER_FILES
        vka/core/shader/shader.h
        vka/core/shader/shader.inl
        vka/core/shader/shader.cpp
)

########################################################################################################################
##################################################### CORE/TEXTURE #####################################################
########################################################################################################################

set(VKA_CORE_TEXTURE_FILES
        vka/core/texture/texture.h
        vka/core/texture/texture.inl
        vka/core/texture/texture.cpp
)

########################################################################################################################
#################################################### CORE/DESCRIPTOR ###################################################
########################################################################################################################

set(VKA_CORE_DESCRIPTOR_FILES
        vka/core/descriptor/descriptor_class.h
        vka/core/descriptor/descriptor_inline.inl
        vka/core/descriptor/descriptor_set_binding_list.inl
        vka/core/descriptor/descriptor_layout_array.inl
        vka/core/descriptor/descriptor_set_array.inl
        vka/core/descriptor/descriptor.h
)

########################################################################################################################
################################################## CORE/PUSH_CONSTANT ##################################################
########################################################################################################################

set(VKA_CORE_PUSH_CONSTANT_FILES
        vka/core/push_constant/push_constant_class.h
        vka/core/push_constant/push_constant.inl
        vka/core/push_constant/push_constant_layout.inl
        vka/core/push_constant/push_constant_view.inl
)

########################################################################################################################
###################################################### CORE/HANDLE #####################################################
########################################################################################################################

set(VKA_CORE_HANDLE_FILES
        vka/core/handle/handle.h
        vka/core/handle/unique_handle.h
        vka/core/handle/unique_handle_parent.inl
        vka/core/handle/unique_handle_no_parent.inl
        vka/core/handle/unique_handle_dynamic_array.inl
)

########################################################################################################################
###################################################### CORE/MODEL ######################################################
########################################################################################################################

set(VKA_CORE_MODEL_FILES
)

########################################################################################################################
######################################################### CORE #########################################################
########################################################################################################################

set(VKA_CORE_FILES
        ${VKA_CORE_COMMON_FILES}
        ${VKA_CORE_ERROR_FILES}
        ${VKA_CORE_INSTANCE_FILES}
        ${VKA_CORE_DEVICE_FILES}
        ${VKA_CORE_QUEUE_FILES}
        ${VKA_CORE_SURFACE_FILES}
        ${VKA_CORE_SWAPCHAIN_FILES}
        ${VKA_CORE_MEMORY_FILES}
        ${VKA_CORE_FORMAT_FILES}
        ${VKA_CORE_ATTACHMENT_FILES}
        ${VKA_CORE_BUFFER_FILES}
        ${VKA_CORE_SHADER_FILES}
        ${VKA_CORE_TEXTURE_FILES}
        ${VKA_CORE_DESCRIPTOR_FILES}
        ${VKA_CORE_PUSH_CONSTANT_FILES}
        ${VKA_CORE_HANDLE_FILES}
        ${VKA_CORE_MODEL_FILES}
        vka/core/core.h
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
)

########################################################################################################################
####################################################### LIBRARIES ######################################################
########################################################################################################################

add_library(vka STATIC ${VKA_FILES})
target_link_libraries(vka Vulkan::Headers)
target_include_directories(vka PUBLIC ${VKA_INCLUDE_DIR})

add_library(vka_glfw STATIC ${VKA_FILES})
target_compile_options(vka_glfw PRIVATE -DVKA_GLFW_ENABLE -DVKA_INCLUDE_GLFW)
target_link_libraries(vka_glfw PRIVATE Vulkan::Headers)
target_include_directories(vka_glfw PRIVATE ${VKA_GLFW_INCLUDE_DIR})
target_include_directories(vka_glfw PUBLIC ${VKA_INCLUDE_DIR})

########################################################################################################################
############################################### VKA GLFW EXTENSION FILES ###############################################
########################################################################################################################

# compile and link executable
add_executable(vka_example
        main.cpp
        vka_example.h
        vka_example.cpp
)
target_link_libraries(vka_example PRIVATE
        Vulkan::Vulkan
        glfw
        vka_glfw
)
target_include_directories(vka_example PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

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
