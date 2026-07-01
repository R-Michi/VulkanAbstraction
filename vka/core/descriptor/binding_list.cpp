#include <vka/vka.h>

void vka::DescriptorBindingList::push(VkDescriptorType type, VkShaderStageFlags stages, uint32_t count, const VkSampler* immutable_samplers)
{
    const VkDescriptorSetLayoutBinding binding = {
        .binding = (uint32_t)this->m_bindings.back().size(),
        .descriptorType = type,
        .descriptorCount = count,
        .stageFlags = stages,
        .pImmutableSamplers = immutable_samplers
    };
    this->m_bindings.back().push_back(binding);
}

void vka::DescriptorBindingList::next_set()
{
    this->m_bindings.emplace_back();
}