# requiered CMAKE version to build the project
cmake_minimum_required (VERSION 3.8)

# current project
project("FirstVulkan")

include_directories("C:/VulkanSDK/1.2.170.0/Include"
					"${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/include"
					"${CMAKE_CURRENT_SOURCE_DIR}/lib/glm"
					"${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_master")

link_directories("C:/VulkanSDK/1.2.170.0/Lib"
				 "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.3/lib"
				 "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm/glm/lib")

add_executable(first_vulkan "main.cpp" "VulkanApp.cpp" "src/vka_instance.cpp" "src/vka_device.cpp" "src/vka_queue.cpp" "src/vka_swapchain.cpp" "src/vka_shader.cpp" "src/vka_buffer.cpp" "src/vka_utility.cpp" "src/vka_texture.cpp" "src/vka_attachment.cpp")
target_link_libraries(first_vulkan PRIVATE "-lglm_static" "-lglfw3" "-lvulkan-1")

add_custom_command(TARGET first_vulkan 
				   PRE_BUILD COMMAND "compile_shader.bat"
				   WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# additional work
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
