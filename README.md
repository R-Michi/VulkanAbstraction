# VulkanAbstraction
The aim of this library is to minimize the code that must be written in order to set up a vulkan renderer.
It abstracts and simplifies general duties, like finding a device, creating buffers and textures, setting up shaders and descriptors, etc.

# Features
- GLFW <br>
  This feature can be enabled via define VKA_GLFW_ENABLE.

- Model loading <br>
  This feature can be enabled via define VKA_MODEL_LOADING_ENABLE.

- Image loading <br>
  This feature can be enabled via define VKA_STB_IMAGE_LOAD_ENABLE.

# External libraries
- vulkan library: <br>
  The vulkan library is always included externally.

- GLFW 3.3.3: <br>
  This library is optional and is only requiered, if the GLFW feature is enabled. This library can be included externally or internally via
  define VKA_INCLUDE_GLFW. If the GLFW feature is not enabled, defining VKA_INCLUDE_GLFW has no effect.

- tiny obj loader: <br>
  This library is optional and is only requiered, if the model loading feature is enabled. This library can be included externally or
  internally via define VKA_INCLUDE_TINYOBJ. If the model loading feature is not enabled, defining VKA_INCLUDE_TINYOBJ has no effect.
  This library is header only.

- STB image: <br>
  This library is optional and is only requiered, if the image loading feature is enabled. This library can be included externally or
  internally via define VKA_INCLUDE_STB_IMAGE. If the image loading feature is not enabled, defining VKA_INCLUDE_STB_IMAGE has no effect.
  This library is header only and only the include file "stb_image.h" is requiered.

# Building the testing sources
This project is built with CMAKE. The requiered libraries are not included in this repository and must be installed manually.
Following directory structure is requiered for a successful build. <br>
NOTE: GLM and GLFW are requiered as static libraries (glfw3.lib and glm_static.lib) and you have to build them yourself.

<pre>
[project directory]
+-- lib
    +-- glfw-3.3.3
    |   +-- include
    |   |   +-- GLFW
    |   |       +-- glfw3.h
    |   +-- lib
    |       +-- glfw3.lib
    +-- glm
    |   +-- glm
    |       +-- lib
    |       |   +-- glm_static.lib
    |       +-- [all GLM files]
    +-- stb_master
    |   +-- stb
    |       +-- stb_image.h
    +-- tiny_obj_loader
        +-- tiny_obj_loader.h
</pre>
