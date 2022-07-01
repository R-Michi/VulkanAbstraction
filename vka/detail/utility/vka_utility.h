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
            VkImageUsageFlagBits format_feature_to_image_usage_bit(VkFormatFeatureFlagBits format_feature);
            VkFormatFeatureFlagBits image_usage_to_format_feature_bit(VkImageUsageFlagBits image_usage);
            std::map<VkFormat, size_t> init_format_sizeof(void) noexcept;

            class CommandBufferWrapper
            {
            private:
                VkDevice device;
                VkCommandPool pool;
                VkResult last_error;
                std::vector<VkCommandBuffer> cbos;

            public:
                CommandBufferWrapper(void) = delete;
                CommandBufferWrapper(VkDevice device, const VkCommandBufferAllocateInfo& ai);
                virtual ~CommandBufferWrapper(void);

                const std::vector<VkCommandBuffer>& get(void) const noexcept;
                VkResult result(void) const noexcept;
            };
        } // namespace utility
    } // namespace detail
} // namespace vka

#ifdef VKA_IMPLEMENTATION
    #include "vka_utility_impl.inl"
#endif
