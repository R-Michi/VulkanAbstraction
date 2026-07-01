#include <vka/vka.h>

vka::DescriptorSets::DescriptorSets(VkDescriptorPool pool, const DescriptorLayouts& layouts) :
    m_sets(create_sets(pool, layouts))
{}

vka::unique_handle<vka::DescriptorSets::Handle> vka::DescriptorSets::create_sets(VkDescriptorPool pool, const DescriptorLayouts& layouts)
{
    const VkDescriptorSetAllocateInfo allocate_info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = nullptr,
        .descriptorPool = pool,
        .descriptorSetCount = layouts.count(),
        .pSetLayouts = layouts.handles()
    };

    VkDescriptorSet* sets = new VkDescriptorSet[layouts.count()];
    check_result(vkAllocateDescriptorSets(layouts.parent(), &allocate_info, sets), MSG_CREATE_FAILED);
    const Parent parent = { layouts.parent(), pool };
    const Handle handle = { sets, layouts.count() };
    return unique_handle(parent, handle);
}