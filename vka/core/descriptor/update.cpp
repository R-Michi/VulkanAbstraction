#include <vka/vka.h>

void vka::DescriptorUpdateOP::write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos)
{
    this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->operator[](set), binding, offset, count, type, infos));
}

void vka::DescriptorUpdateOP::write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos)
{
    this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->operator[](set), binding, offset, count, type, infos));
}

void vka::DescriptorUpdateOP::write(uint32_t set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views)
{
    this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->operator[](set), binding, offset, count, type, views));
}

void vka::DescriptorUpdateOP::write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as)
{
    this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->operator[](set), binding, offset, as));
}

void vka::DescriptorUpdateOP::write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as)
{
    this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->operator[](set), binding, offset, as));
}

void vka::DescriptorUpdateOP::write(uint32_t set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& block)
{
    this->m_writes.push_back(detail::descriptor::make_write(this->m_sets->operator[](set), binding, offset, block));
}