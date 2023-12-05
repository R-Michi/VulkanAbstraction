/**
* @file     vka_descriptor.h
* @brief    Detailed declaration of descriptor-manager classes and functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka::detail::descriptor
{
    union ExtendedWriteSet
    {
        VkWriteDescriptorSetAccelerationStructureNV asNV;
        VkWriteDescriptorSetAccelerationStructureKHR asKHR;
        VkWriteDescriptorSetInlineUniformBlock iub;
    };

    enum class DescriptorInfoType
    {
        BUFFER_INFO,
        IMAGE_INFO,
        ACCELERATION_STRUCTURE_NV,
        ACCELERATION_STRUCTURE_KHR,
        INLINE_UNIFORM_BLOCK
    };

    template<typename T>
    struct get_descriptor_info_type
    {
        static_assert("[vka::detail::descriptor::get_descriptor_info_type]: Invalid info type.");
    };
    template<> struct get_descriptor_info_type<VkDescriptorBufferInfo>      { constexpr static DescriptorInfoType type = DescriptorInfoType::BUFFER_INFO; };
    template<> struct get_descriptor_info_type<VkDescriptorImageInfo>       { constexpr static DescriptorInfoType type = DescriptorInfoType::IMAGE_INFO; };
    template<> struct get_descriptor_info_type<VkAccelerationStructureNV>   { constexpr static DescriptorInfoType type = DescriptorInfoType::ACCELERATION_STRUCTURE_NV; };
    template<> struct get_descriptor_info_type<VkAccelerationStructureKHR>  { constexpr static DescriptorInfoType type = DescriptorInfoType::ACCELERATION_STRUCTURE_KHR; };
    template<> struct get_descriptor_info_type<const void*>                 { constexpr static DescriptorInfoType type = DescriptorInfoType::INLINE_UNIFORM_BLOCK; };

    /*
     * This is the base class for every kind of descriptor info list. 'Info' specifies the descirptor
     * info type which does not have to be a structure, it can also be a vulkan handle or void.
     * 'Extension' specifies an extension structure to VkWriteDescriptorSet. 'S' specifies the number
     * of descriptor info structures/handles. The Info-type structure stores the actual data for the
     * descriptor write/update.
     */
    template<typename Info, uint32_t S>
    class DescriptorInfoList
    {
    protected:
        Info m_infos[S];
        uint32_t m_idx;

    public:
        constexpr DescriptorInfoList(void) noexcept;
        ~DescriptorInfoList(void) = default;

        // Returns the number of descriptor infos which is specified by the template argument 'S'.
        // TODO: make consteval for >= C++20
        constexpr uint32_t size(void) const noexcept;

        // Returns the written number of descriptor infos.
        inline uint32_t count(void) const noexcept;

        // Returns the raw info list.
        inline const Info* data(void) const noexcept;
    };

    template<typename Info>
    class DescriptorInfoList<Info, 0>
    {
    protected:
        std::vector<Info> m_infos;

    public:
        constexpr DescriptorInfoList(void) noexcept;
        ~DescriptorInfoList(void) = default;

        // Returns the written number of descriptor infos.
        inline uint32_t count(void) const noexcept;

        // Returns the raw info list.
        inline const Info* data(void) const noexcept;
    };

    // special template for inline uniform block
    template<>
    class DescriptorInfoList<const void*, 0>
    {
    protected:
        const void* m_data;

    public:
        constexpr DescriptorInfoList(void) noexcept;
        ~DescriptorInfoList(void) = default;

        // Returns always 0.
        // TODO: make consteval for >= C++20
        constexpr uint32_t size(void) const noexcept;

        // Returns always 0.
        // TODO: make consteval for >= C++20
        constexpr uint32_t count(void) const noexcept;

        // Returns the raw info list.
        inline const void* data(void) const noexcept;
    };
}

#include "vka_descriptor_inline_impl.inl"
