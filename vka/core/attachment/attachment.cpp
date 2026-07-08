/**
 * @brief Implementation for the attachment image class.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <vka/vka.h>

vka::AttachmentImage::AttachmentImage(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info) :
    m_image(create_attachment(device, properties, create_info)),
    m_extent(create_info.imageExtent)
{}

vka::unique_handle<vka::AttachmentImage::Handle> vka::AttachmentImage::create_attachment(VkDevice device, const VkPhysicalDeviceMemoryProperties& properties, const AttachmentImageCreateInfo& create_info)
{
    // create image
    const VkImageCreateInfo image_ci = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .imageType = VK_IMAGE_TYPE_2D,
        .format = create_info.imageFormat,
        .extent = { create_info.imageExtent.width, create_info.imageExtent.height, 1 },
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = create_info.imageSamples,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = create_info.imageUsage,
        .sharingMode = create_info.imageSharingMode,
        .queueFamilyIndexCount = create_info.imageQueueFamilyIndexCount,
        .pQueueFamilyIndices = create_info.imageQueueFamilyIndices,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };
    VkImage image;
    check_result(vkCreateImage(device, &image_ci, nullptr, &image), IMAGE_CREATE_FAILED);
    unique_handle image_guard(device, image);

    // query memory requirements
    VkMemoryRequirements requirements;
    vkGetImageMemoryRequirements(device, image, &requirements);

    // allocate memory for the image
    const VkMemoryAllocateInfo memory_ai = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = requirements.size,
        .memoryTypeIndex = memory::find_type_index(properties, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    };
    VkDeviceMemory memory;
    check_result(vkAllocateMemory(device, &memory_ai, nullptr, &memory), ALLOC_MEMORY_FAILED);
    unique_handle memory_guard(device, memory);
    check_result(vkBindImageMemory(device, image, memory, 0), BIND_MEMORY_FAILED);

    // create image view from image
    const VkImageViewCreateInfo view_ci = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = create_info.viewFormat,
        .components = create_info.viewComponentMapping,
        .subresourceRange = {
            .aspectMask = create_info.viewAspectMask,
            .baseMipLevel = 0,
            .levelCount = 1,        // we don't use mip-mapping on attachment images
            .baseArrayLayer = 0,
            .layerCount = 1
        }
    };
    VkImageView view;
    check_result(vkCreateImageView(device, &view_ci, nullptr, &view), VIEW_CREATE_FAILED);

    const Handle handle = {
        .image = image_guard.release(),
        .memory = memory_guard.release(),
        .view = view
    };
    return unique_handle(device, handle);
}
