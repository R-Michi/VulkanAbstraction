#include <vka/vka.h>

vka::DescriptorLayouts::DescriptorLayouts(VkDevice device, const DescriptorBindingList& bindings, VkDescriptorSetLayoutCreateFlags flags) :
    m_layouts(create_layouts(device, bindings, flags))
{}

vka::unique_handle<VkDescriptorSetLayout[]> vka::DescriptorLayouts::create_layouts(VkDevice device, const DescriptorBindingList& bindings, VkDescriptorSetLayoutCreateFlags flags)
{
    unique_handle<VkDescriptorSetLayout[]> layouts(device, new VkDescriptorSetLayout[bindings.count()]{ VK_NULL_HANDLE }, bindings.count());
    for (uint32_t i = 0; i < bindings.count(); ++i)
    {
        const VkDescriptorSetLayoutCreateInfo create_info = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = flags,
            .bindingCount = bindings.binding_count(i),
            .pBindings = bindings.bindings(i)
        };
        check_result(vkCreateDescriptorSetLayout(device, &create_info, nullptr, layouts.get() + i), MSG_CREATE_FAILED);
    }
    return layouts;
}