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
    /*
     * This is the base class for every kind descriptor info list. 'Info' specifies the descriptor
     * info type and 'S' the maximum number of descriptor infos.
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
        DescriptorInfoList(void) = default;
        ~DescriptorInfoList(void) = default;

        // Returns the written number of descriptor infos.
        inline uint32_t count(void) const noexcept;

        // Returns the raw info list.
        inline const Info* data(void) const noexcept;
    };
}

#include "vka_descriptor_inline_impl.inl"
