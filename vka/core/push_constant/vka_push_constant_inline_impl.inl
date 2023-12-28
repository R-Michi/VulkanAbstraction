/**
* @file     vka_push_constant.h
* @brief    Push constant implementation file of inline functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

/************************************************** PushConstantView **************************************************/

constexpr vka::PushConstantView::PushConstantView(const VkPushConstantRange& range, void* data) noexcept :
    m_range(range), m_data(detail::common::add_vp(data, range.offset))
{}

constexpr VkShaderStageFlags vka::PushConstantView::stages(void) const noexcept
{
    return this->m_range.stageFlags;
}

constexpr uint32_t vka::PushConstantView::size(void) const noexcept
{
    return this->m_range.size;
}

constexpr uint32_t vka::PushConstantView::offset(void) const noexcept
{
    return this->m_range.offset;
}

constexpr void* vka::PushConstantView::data(void) noexcept
{
    return this->m_data;
}

inline void vka::PushConstantView::write(uint32_t offset, uint32_t size, const void* data)
{
    detail::error::check_range(offset, size, this->m_range.size, MSG_INVALID_RANGE);
    memcpy(detail::common::add_vp(this->m_data, offset), data, size);
}

inline void vka::PushConstantView::push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept
{
    vkCmdPushConstants(cbo, layout, this->m_range.stageFlags, this->m_range.offset, this->m_range.size, this->m_data);
}

/************************************************* PushConstantLayout *************************************************/

template<uint32_t N>
constexpr vka::PushConstantLayout<N>::PushConstantLayout(void) noexcept :
    m_ranges{}, m_total_size(0), m_max_size(MIN_SIZE), m_idx(0)
{}

template<uint32_t N>
inline vka::PushConstantLayout<N>::PushConstantLayout(const VkPhysicalDeviceProperties& properties) noexcept :
    m_ranges{}, m_total_size(0), m_max_size(properties.limits.maxPushConstantsSize), m_idx(0)
{}

template<uint32_t N>
constexpr vka::PushConstantLayout<N>::PushConstantLayout(PushConstantLayout&& src) noexcept :
    m_ranges(src.m_ranges), m_total_size(src.m_total_size), m_max_size(src.m_max_size), m_idx(src.m_idx)
{
    src.m_ranges.fill({});
    src.m_total_size = 0;
    src.m_max_size = MIN_SIZE;
    src.m_idx = 0;
}

template<uint32_t N>
constexpr vka::PushConstantLayout<N>& vka::PushConstantLayout<N>::operator= (PushConstantLayout&& src) noexcept
{
    this->m_ranges = src.m_ranges;
    this->m_total_size = src.m_total_size;
    this->m_max_size = src.m_max_size;
    this->m_idx = src.m_idx;
    src.m_ranges.fill({});
    src.m_total_size = 0;
    src.m_max_size = MIN_SIZE;
    src.m_idx = 0;
    return *this;
}

template<uint32_t N>
inline void vka::PushConstantLayout<N>::init(const VkPhysicalDeviceProperties& properties) noexcept
{
    // can only be modified, if no ranges have been added yet
    if (this->m_idx == 0)
        this->m_max_size = properties.limits.maxPushConstantsSize;
}

template<uint32_t N>
constexpr uint32_t vka::PushConstantLayout<N>::round_size(uint32_t s) const noexcept
{
    // equivalent to '(s - 1) / 4 * 4 + 4'
    return s == 0 ? 0 : (((s - 1) >> 2) << 2) + 4;
}

template<uint32_t N>
constexpr void vka::PushConstantLayout<N>::add(uint32_t size, VkShaderStageFlags stages)
{
    if (this->m_idx >= N) [[unlikely]]
        detail::error::throw_out_of_range(MSG_RANGE);

    size = round_size(size);
    const uint32_t new_size = this->m_total_size + size;
    if (new_size > this->m_max_size)
        detail::error::throw_runtime_error(MSG_SIZE);

    m_ranges[this->m_idx++] = { stages, this->m_total_size, size };
    this->m_total_size += size;
}

template<uint32_t N>
inline vka::PushConstants<N> vka::PushConstantLayout<N>::create_push_constants(void)
{
    return PushConstants<N>(*this);
}

template<uint32_t N>
constexpr uint32_t vka::PushConstantLayout<N>::count(void) const noexcept
{
    return N;
}

template<uint32_t N>
constexpr uint32_t vka::PushConstantLayout<N>::size(void) const noexcept
{
    return this->m_total_size;
}

template<uint32_t N>
constexpr const VkPushConstantRange* vka::PushConstantLayout<N>::data(void) const noexcept
{
    return this->m_ranges.data();
}

template<uint32_t N>
constexpr const VkPushConstantRange& vka::PushConstantLayout<N>::at(uint32_t idx) const
{
    return this->m_ranges.at(idx);
}

template<uint32_t N>
constexpr const VkPushConstantRange& vka::PushConstantLayout<N>::operator[] (uint32_t idx) const noexcept
{
    return this->m_ranges[idx];
}

template<uint32_t N>
constexpr void vka::PushConstantLayout<N>::validate(void) const
{
    if (this->idx < N) [[unlikely]]
        detail::error::throw_runtime_error(MSG_UNUSED);
}

/**************************************************** PushConstants ***************************************************/

template<uint32_t N>
inline vka::PushConstants<N>::PushConstants(void) noexcept :
    m_layout(nullptr)
{}

template<uint32_t N>
inline vka::PushConstants<N>::PushConstants(const PushConstantLayout<N>& layout) :
    m_layout(&layout), m_buff(layout.size())
{}

template<uint32_t N>
inline vka::PushConstants<N>::PushConstants(PushConstants&& src) noexcept :
    m_layout(src.m_layout), m_buff(std::move(src.m_buff))
{}

template<uint32_t N>
inline vka::PushConstants<N>& vka::PushConstants<N>::operator= (PushConstants&& src) noexcept
{
    this->m_layout = src.m_layout;
    this->m_buff = std::move(src.m_buff);
    return *this;
}

template<uint32_t N>
inline void vka::PushConstants<N>::create(const PushConstantLayout<N>& layout)
{
    if (this->m_buff.empty())
    {
        this->m_layout = &layout;
        this->m_buff.allocate(layout.size());
    }
}

template<uint32_t N>
inline void vka::PushConstants<N>::destroy(void) noexcept
{
    this->m_buff.free();
}

template<uint32_t N>
inline void vka::PushConstants<N>::push(VkCommandBuffer cbo, VkPipelineLayout layout) const noexcept
{
    for (uint32_t i = 0; i < N; i++)
        vkCmdPushConstants(cbo, layout, *(this->m_layout)[i].stageFlags, *(this->m_layout)[i].offset, *(this->m_layout)[i].size);
}

template<uint32_t N>
inline vka::PushConstantView vka::PushConstants<N>::at(uint32_t idx) const
{
    if (this->m_buff.empty()) [[unlikely]]
        detail::error::throw_runtime_error(MSG_ACCESS);
    return PushConstantView(this->m_layout->at(idx), this->m_buff.data());
}

template<uint32_t N>
inline vka::PushConstantView vka::PushConstants<N>::operator[] (uint32_t idx) const noexcept
{
    return PushConstantView(*(this->m_layout)[idx], this->m_buff.data());
}

template<uint32_t N>
inline bool vka::PushConstants<N>::is_valid(void) const noexcept
{
    return !this->m_buff.empty();
}
