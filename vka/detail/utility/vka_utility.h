/**
* @file     vka_utility.h
* @brief    Detailed declaration of utility functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka
{
    namespace detail
    {
        namespace utility
        {
            constexpr VkImageUsageFlagBits ff2iu_bit(VkFormatFeatureFlagBits format_feature) noexcept;
            constexpr VkFormatFeatureFlagBits iu2ff_bit(VkImageUsageFlagBits image_usage) noexcept;
            void init_format_sizeof(std::unordered_map<VkFormat, size_t>& f2s);

            template<size_t N>
            class CommandBufferWrapper final
            {
                static_assert(N >= 1, "[vka::detail::utility::CommandBufferWrapper]: Template argument N must be >= 1");
            private:
                VkCommandBuffer cbos[N];
                VkResult last_result;
                VkDevice device;
                VkCommandPool pool;

            public:
                CommandBufferWrapper(void) = delete;
                explicit inline CommandBufferWrapper(VkDevice device, const VkCommandBufferAllocateInfo& ai) noexcept;
                inline virtual ~CommandBufferWrapper(void); // compiler can inline virtual destructor on non-reference object

                inline VkCommandBuffer operator[] (size_t i) const noexcept;
                inline const VkCommandBuffer* pat(size_t i) const noexcept;
                inline VkResult result(void) const noexcept;
            };

            template<>
            class CommandBufferWrapper<1> final
            {
            private:
                VkCommandBuffer cbo;
                VkResult last_result;
                VkDevice device;
                VkCommandPool pool;

            public:
                CommandBufferWrapper(void) = delete;
                explicit inline CommandBufferWrapper(VkDevice device, const VkCommandBufferAllocateInfo& ai) noexcept;
                inline virtual ~CommandBufferWrapper(void); // compiler can inline virtual destructor on non-reference object

                inline VkCommandBuffer get(void) const noexcept;
                inline const VkCommandBuffer* pget(void) const noexcept;
                inline operator VkCommandBuffer(void) const noexcept;
                inline const VkCommandBuffer* operator& (void) const noexcept;
                inline VkResult result(void) const noexcept;
            };

            using CommandBuffer = CommandBufferWrapper<1>;
        } // namespace utility
    } // namespace detail
} // namespace vka

#ifdef VKA_IMPLEMENTATION
    #include "vka_utility_impl.inl"
#endif
#include "vka_utility_inline_impl.inl"
