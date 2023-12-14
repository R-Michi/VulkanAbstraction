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
}

#include "vka_descriptor_inline_impl.inl"
