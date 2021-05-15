# VulkanAbstraction

# version 1.0.0 update notes
    - fixed: class AttachmentImage could not create attachments with VK_IMAGE_USAGE_SAMPLED_BIT
    - fixed: in class Texture it was not possible to set the image view's subresource range's mipmap level count
    - fixed: move constructor of class buffer does not clear the create info, physical device, device, command pool and queue anymore
    - removed: in namespace vka::utility the functions get_supported_color_formats(), get_supported_depth_formats(), 
               get_supported_stencil_formats() and get_supported_depth_stencil_formats()
    - removed: several invalid checks of the class Attachment image, instead the validation layers are used
    - added: in namespace vka::utility the function get_supported_formats()
    - added: method mem_size() in class Buffer which returns the actual size of the allocated memory