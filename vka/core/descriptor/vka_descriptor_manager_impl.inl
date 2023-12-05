/**
* @file     vka_descriptor_impl.inl
* @brief    Descriptor implementation file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

vka::DescriptorManager::DescriptorManager(void) noexcept
{
    this->_pool = VK_NULL_HANDLE;
    this->device = VK_NULL_HANDLE;
}

vka::DescriptorManager::DescriptorManager(VkDevice device, uint32_t n_sets)
{
    this->_pool = VK_NULL_HANDLE;
    this->device = device;
    this->set_descriptor_set_count(n_sets);
}

vka::DescriptorManager::~DescriptorManager(void)
{
    this->clear();
}


void vka::DescriptorManager::add_to_pool_sizes(VkDescriptorType type, uint32_t count)
{
    // find descriptor type
    for (VkDescriptorPoolSize& pool_size : this->pool_sizes)
    {
        if (pool_size.type == type)
        {
            // sum up the count of this descriptor type
            pool_size.descriptorCount += count;
            return;
        }
    }

    // if pool size with this descriptor type does not exist, create it
    VkDescriptorPoolSize pool_size = {};
    pool_size.type = type;
    pool_size.descriptorCount = count;
    this->pool_sizes.push_back(pool_size);
}


void vka::DescriptorManager::set_device(VkDevice device) noexcept
{
    this->device = device;
}

void vka::DescriptorManager::set_descriptor_set_count(uint32_t n_sets)
{
    if (this->_pool != VK_NULL_HANDLE)
        throw std::runtime_error("[vka::DescriptorManager::set_descriptor_set_count]: DescriptorManager::set_descriptor_set_count MUST NOT be called after DescriptorManager::init!");

    this->set_layout_bindings.resize(n_sets);
}

uint32_t vka::DescriptorManager::descriptor_set_count(void) const noexcept
{
    return this->set_layout_bindings.size();
}

void vka::DescriptorManager::add_binding(uint32_t set, uint32_t binding, VkDescriptorType type, uint32_t count, VkShaderStageFlags stage)
{
    if (this->_pool != VK_NULL_HANDLE)
        throw std::runtime_error("[vka::DescriptorManager::add_binding]: DescriptorManager::add_binding MUST NOT be called after DescriptorManager::init!");
    if (set >= this->set_layout_bindings.size())
        throw std::out_of_range("[vka::DescriptorManager::add_binding]: Descriptor set ID of DescriptorManager::add_binding is greather than or equal to the amount of sets in the DescriptorManager!");

    // add binding to descriptor set
    VkDescriptorSetLayoutBinding set_binding = {};
    set_binding.binding = binding;
    set_binding.descriptorType = type;
    set_binding.descriptorCount = count;
    set_binding.stageFlags = stage;
    set_binding.pImmutableSamplers = nullptr;
    this->set_layout_bindings.at(set).push_back(set_binding);

    // add the descriptors of the binding to the pool sizes
    this->add_to_pool_sizes(type, count);
}


void vka::DescriptorManager::init_descriptor_write(VkWriteDescriptorSet& write, uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t n_elements)
{
    if (set >= this->set_layout_bindings.size())
        throw std::out_of_range("[vka::DescriptorManager::init_descriptor_writes]: Descriptor set ID of DescriptorManager::init_descriptor_write is greather than or equal to the amount of sets in the DescriptorManager!");

    // search the vector index for this binding
    int64_t index = -1;
    for (size_t i = 0; i < this->set_layout_bindings.at(set).size(); i++)
    {
        if (this->set_layout_bindings.at(set).at(i).binding == binding)
            index = i;
    }

    // binding number does not correspond to the index of the vector
    if (index == -1)
        throw std::runtime_error("[vka::DescriptorManager::init_descriptor_writes]: Unable to find binding ID in DescriptorManager::init_descriptor_write.");

    write = {};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.pNext = nullptr;
    write.dstSet = this->sets.at(set);
    write.dstBinding = binding;
    write.dstArrayElement = begin_element;
    write.descriptorCount = n_elements;
    write.descriptorType = this->set_layout_bindings.at(set).at(index).descriptorType;
    write.pBufferInfo = nullptr;
    write.pImageInfo = nullptr;
    write.pTexelBufferView = nullptr;
}

void vka::DescriptorManager::write_buffer_info(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t n_elements, const VkDescriptorBufferInfo* buffer_infos)
{
    VkWriteDescriptorSet write;
    this->init_descriptor_write(write, set, binding, begin_element, n_elements);
    write.pBufferInfo = buffer_infos;
    this->descriptor_writes.push_back(write);
}

void vka::DescriptorManager::write_image_info(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t n_elements, const VkDescriptorImageInfo* image_infos)
{
    VkWriteDescriptorSet write;
    this->init_descriptor_write(write, set, binding, begin_element, n_elements);
    write.pImageInfo = image_infos;
    this->descriptor_writes.push_back(write);
}

void vka::DescriptorManager::write_texel_buffer_view(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t n_elements, const VkBufferView* buffer_views)
{
    VkWriteDescriptorSet write;
    this->init_descriptor_write(write, set, binding, begin_element, n_elements);
    write.pTexelBufferView = buffer_views;
    this->descriptor_writes.push_back(write);
}

void vka::DescriptorManager::write_acceleration_structure_NV(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t n_elements, const VkAccelerationStructureNV* as_nv)
{
    VkWriteDescriptorSetAccelerationStructureNV* as_write = new VkWriteDescriptorSetAccelerationStructureNV{};
    as_write->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV;
    as_write->pNext = nullptr;
    as_write->accelerationStructureCount = n_elements;
    as_write->pAccelerationStructures = as_nv;

    VkWriteDescriptorSet write;
    this->init_descriptor_write(write, set, binding, begin_element, n_elements);
    write.pNext = as_write;
    this->descriptor_writes.push_back(write);
}

void vka::DescriptorManager::write_acceleration_structure_KHR(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t n_elements, const VkAccelerationStructureKHR* as_khr)
{
    VkWriteDescriptorSetAccelerationStructureKHR* as_write = new VkWriteDescriptorSetAccelerationStructureKHR{};
    as_write->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
    as_write->pNext = nullptr;
    as_write->accelerationStructureCount = n_elements;
    as_write->pAccelerationStructures = as_khr;

    VkWriteDescriptorSet write;
    this->init_descriptor_write(write, set, binding, begin_element, n_elements);
    write.pNext = as_write;
    this->descriptor_writes.push_back(write);
}

void vka::DescriptorManager::write_inline_uniform_block_EXT(uint32_t set, uint32_t binding, uint32_t offset, uint32_t size, const void* data)
{
    VkWriteDescriptorSetInlineUniformBlockEXT* uniform_block = new VkWriteDescriptorSetInlineUniformBlockEXT{};
    uniform_block->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT;
    uniform_block->pNext = nullptr;
    uniform_block->dataSize = size;
    uniform_block->pData = data;

    VkWriteDescriptorSet write;
    this->init_descriptor_write(write, set, binding, offset, size);
    write.pNext = uniform_block;
    this->descriptor_writes.push_back(write);
}


VkResult vka::DescriptorManager::init_descriptor_layouts(void)
{
    VkDescriptorSetLayoutCreateInfo layout_ci = {};
    layout_ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_ci.pNext = nullptr;
    layout_ci.flags = 0;

    VkResult result = VK_SUCCESS;
    for (size_t i = 0; i < this->set_layout_bindings.size(); i++)
    {
        layout_ci.bindingCount = this->set_layout_bindings.at(i).size();
        layout_ci.pBindings = this->set_layout_bindings.at(i).data();

        VkDescriptorSetLayout layout;
        if ((result = vkCreateDescriptorSetLayout(this->device, &layout_ci, nullptr, &layout)) != VK_SUCCESS)
            return result;
        this->set_layouts.push_back(layout);
    }
    return result;
}

VkResult vka::DescriptorManager::init_descriptor_pool(void)
{
    VkDescriptorPoolCreateInfo pool_ci = {};
    pool_ci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_ci.pNext = nullptr;
    pool_ci.flags = 0;
    pool_ci.maxSets = this->set_layouts.size();
    pool_ci.poolSizeCount = this->pool_sizes.size();
    pool_ci.pPoolSizes = this->pool_sizes.data();

    return vkCreateDescriptorPool(this->device, &pool_ci, nullptr, &this->_pool);
}

VkResult vka::DescriptorManager::init_descriptor_sets(void)
{
    VkDescriptorSetAllocateInfo set_alloc_info = {};
    set_alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    set_alloc_info.pNext = nullptr;
    set_alloc_info.descriptorPool = this->_pool;
    set_alloc_info.descriptorSetCount = this->set_layouts.size();
    set_alloc_info.pSetLayouts = this->set_layouts.data();

    this->sets.resize(this->set_layouts.size());
    return vkAllocateDescriptorSets(this->device, &set_alloc_info, this->sets.data());
}


VkResult vka::DescriptorManager::init(void)
{
    if (this->_pool != VK_NULL_HANDLE)
        throw std::runtime_error("[vka::DescriptorManager::init]: DescriptorManager has already been initialized.");
    if (this->device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::DescriptorManager::init]: Device of DescriptorManager is a VK_NULL_HANDLE.");

    VkResult result = VK_SUCCESS;
    if ((result = this->init_descriptor_layouts()) != VK_SUCCESS)    return result;
    if ((result = this->init_descriptor_pool()) != VK_SUCCESS)       return result;
    if ((result = this->init_descriptor_sets()) != VK_SUCCESS)       return result;

    return result;
}

void vka::DescriptorManager::update(void)
{
    if (this->_pool == VK_NULL_HANDLE)
        throw std::runtime_error("[vka::DescriptorManager::update]: To update the descriptor sets, the DescriptorManager must be initialized.");
    if (this->device == VK_NULL_HANDLE)
        throw std::invalid_argument("[vka::DescriptorManager::update]: Device of DescriptorManager is a VK_NULL_HANDLE.");

    vkUpdateDescriptorSets(this->device, this->descriptor_writes.size(), this->descriptor_writes.data(), 0, nullptr);
}

void vka::DescriptorManager::clear(void)
{
    if (this->_pool != VK_NULL_HANDLE)
    {
        if (this->device == VK_NULL_HANDLE)
            throw std::invalid_argument("[vka::DescriptorManager::clear]: Device of DescriptorManager is a VK_NULL_HANDLE.");

        // destroy layouts
        for (VkDescriptorSetLayout layout : this->set_layouts)
            vkDestroyDescriptorSetLayout(this->device, layout, nullptr);
        vkDestroyDescriptorPool(this->device, this->_pool, nullptr);

        // delete pNext pointers of descriptor writes
        for (VkWriteDescriptorSet& write : this->descriptor_writes)
        {
            if (write.pNext != nullptr)
            {
                // get structure-type
                VkStructureType structure_type;
                memcpy(&structure_type, write.pNext, sizeof(structure_type));   // the structure-type stands at the begin of every vulkan structure

                // delete according to the structure-type
                switch (structure_type)
                {
                case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV:
                    delete(reinterpret_cast<const VkWriteDescriptorSetAccelerationStructureNV*>(write.pNext));
                    write.pNext = nullptr;
                    break;
                case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR:
                    delete(reinterpret_cast<const VkWriteDescriptorSetAccelerationStructureKHR*>(write.pNext));
                    write.pNext = nullptr;
                    break;
                case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT:
                    delete(reinterpret_cast<const VkWriteDescriptorSetInlineUniformBlockEXT*>(write.pNext));
                    write.pNext = nullptr;
                    break;
                default:
                    break;
                }
            }
        }

        this->set_layouts.clear();
        this->sets.clear();
        this->pool_sizes.clear();
        this->set_layout_bindings.clear();
        this->descriptor_writes.clear();

        this->_pool = VK_NULL_HANDLE;
    }
}


VkDescriptorPool vka::DescriptorManager::pool(void) const noexcept
{
    return this->_pool;
}

const std::vector<VkDescriptorSetLayout>& vka::DescriptorManager::layouts(void) const noexcept 
{
    return this->set_layouts;
}

const std::vector<VkDescriptorSet>& vka::DescriptorManager::descriptor_sets(void) const noexcept
{
    return this->sets;
}



/********************************************** DescriptorSetBindingList *********************************************/

void vka::DescriptorSetBindingList::push(uint32_t set, VkShaderStageFlags stages, VkDescriptorType type, uint32_t count, const VkSampler* immutable_samplers)
{
    // the highest index must be the highest set number.
    // -> size = max_index + 1 = max_set_number + 1
    if (set >= this->m_bindings.size())
        this->m_bindings.resize(set + 1);

    const VkDescriptorSetLayoutBinding binding = {
        .binding = (uint32_t)this->m_bindings[set].size(),
        .descriptorType = type,
        .descriptorCount = count,
        .stageFlags = stages,
        .pImmutableSamplers = immutable_samplers
    };
    this->m_bindings[set].push_back(binding);
}
