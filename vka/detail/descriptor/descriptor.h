/**
 * @brief Includes implementation details for descriptors.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka::detail::descriptor
{
    /**
     * @brief Creates the write-structure for a buffer.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorBufferInfo* infos) noexcept;

    /**
     * @brief Creates the write-structure for an image / texture.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkDescriptorImageInfo* infos) noexcept;

    /**
     * @brief Creates the write-structure for a buffer view.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, uint32_t count, VkDescriptorType type, const VkBufferView* views) noexcept;

    /**
     * @brief Creates the write-structure for a NV-acceleration-structure (NV = Nvidia).
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureNV& as_write) noexcept;

    /**
     * @brief Creates the write-structure for a KHR-acceleration-structure.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetAccelerationStructureKHR& as_write) noexcept;

    /**
     * @brief Creates the write-structure for an inline uniform block.
     * @return Returns an initialized descriptor-write structure.
     */
    inline VkWriteDescriptorSet make_write(VkDescriptorSet set, uint32_t binding, uint32_t offset, const VkWriteDescriptorSetInlineUniformBlock& iub_write) noexcept;

#if 0
    template<typename Info>
    struct const_info_type { static_assert("[vka::detail::descriptor::info_const_type]: Invalid type."); };
    template<> struct const_info_type<VkDescriptorBufferInfo>      { using type = const VkDescriptorBufferInfo&; };
    template<> struct const_info_type<VkDescriptorImageInfo>       { using type = const VkDescriptorImageInfo&; };
    template<> struct const_info_type<VkBufferView>                { using type = VkBufferView; };
    template<> struct const_info_type<VkAccelerationStructureNV>   { using type = VkAccelerationStructureNV; };
    template<> struct const_info_type<VkAccelerationStructureKHR>  { using type = VkAccelerationStructureKHR; };

    template<typename Info, uint32_t N>
    class DescriptorInfoArray
    {
        using const_info = const_info_type<Info>::type;
    private:
        Info m_infos[N];

    public:
        DescriptorInfoArray(void) = default;
        ~DescriptorInfoArray(void) = default;

        constexpr Info& operator[] (uint32_t idx) noexcept
        {
            return this->m_infos[idx];
        }

        constexpr const_info operator[] (uint32_t idx) const noexcept
        {
            return this->m_infos[idx];
        }

        constexpr const Info* data(void) const noexcept
        {
            return this->m_infos;
        }

        constexpr uint32_t size(void) const noexcept
        {
            return N;
        }
    };

    template<typename Info>
    class DescriptorInfoArray<Info, 0>
    {
        using const_info = const_info_type<Info>::type;
    private:
        std::vector<Info> m_infos;

    public:
        DescriptorInfoArray(void) = default;
        ~DescriptorInfoArray(void) = default;

        void push_back(const_info info)
        {
            this->m_infos.push_back(info);
        }

        const Info* data(void) const noexcept
        {
            return this->m_infos.data();
        }

        uint32_t size(void) const noexcept
        {
            return (uint32_t)this->m_infos.size();
        }
    };

    template<>
    class DescriptorInfoArray<const void*, 0>
    {
    private:
        const void* m_data;
        uint32_t m_size;

    public:
        constexpr DescriptorInfoArray(void) noexcept : m_data(nullptr), m_size(0) {}
        constexpr DescriptorInfoArray(const void* data, uint32_t size) noexcept : m_data(data), m_size(size) {}
        ~DescriptorInfoArray(void) = default;

        constexpr void set(const void* data, uint32_t size)
        {
            this->m_data = data;
            this->m_size = size;
        }

        constexpr const void* data(void) const noexcept
        {
            return this->m_data;
        }

        constexpr uint32_t size(void) const noexcept
        {
            return this->m_size;
        }
    };

    template<uint32_t N>
    constexpr uint32_t min(uint32_t x, uint32_t y) noexcept
    {
        if constexpr (N == 0)
            return y;
        else
            return (x < y ? x : y);
    }
#endif
}

#include "descriptor.inl"
