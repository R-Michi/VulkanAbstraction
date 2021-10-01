# VulkanAbstraction
The aim of this library is to minimize the code that must be written in order to setup a vulkan renderer.
It abstracts and simplifies general duties, like finding a device, creating buffers and textures, setting up shaders and descriptors, etc.

# External libraries
1) vulkan library: You must have the vulkan SDK installed. (no na ned)
2) GLFW 3.3.3: Vulkan is included via glfw. GLFW must be complied into a static library.
3) GLM: GLM must be compiled into a static library.
5) tiny_obj_loader: Tiny object loader library is used for the experimental model loading implemenation. Header only library.
4) STB-MASTER: From stb-master, only the "stb_image.h" file is used by the testing application. Header only library.

NOTE: You must specify include/search directories for the 'Tiny object loader' libary (and the 'STB-MASTER' library if you want to build
the testing sources).
GLFW in included via #include <GLFW/glfw3.h>

IMPORTANT: YOU HAVE TO BUILD THE STATIC LIBRARIES BY YOURDELF!!!

# Building the testing sources
This project is a CMAKE project and can be build with CMAKE.
There is one folder named "lib" that is not dowloaded.
This folder contains all precompiled and header only libraries to be able to build the testing sources.
Now you have to set up following directory structure:

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
    |       +-- <all glm related files>
    +-- stb_master
    |   +-- stb
    |       +-- stb_image.h
    +-- tiny_obj_loader
        +-- tiny_obj_loader.h

# version 1.0.0 update notes
    - fixed: class AttachmentImage could not create attachments with VK_IMAGE_USAGE_SAMPLED_BIT
    - fixed: in class Texture it was not possible to set the image view's subresource range's mipmap level count
    - fixed: move constructor of class buffer does not clear the create info, physical device, device, command pool and queue anymore
    - removed: in namespace vka::utility the functions get_supported_color_formats(), get_supported_depth_formats(), 
               get_supported_stencil_formats() and get_supported_depth_stencil_formats()
    - removed: several invalid checks of the class Attachment image, instead the validation layers are used
    - added: in namespace vka::utility the function get_supported_formats()
    - added: method mem_size() in class Buffer which returns the actual size of the allocated memory

# version 1.1.0 update noes
    - general: Library is now implemenated as header-only.
    - added: descriptor manager