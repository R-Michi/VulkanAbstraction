/**
* @file     vka_utility.h
* @brief    Detailed declaration of common functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#define VKA_CONVERT_EXPLICIT explicit

namespace vka::detail::common
{
    /*
     * is_handle::value stores 'true', if the type T is a Vulkan handle and 'false' otherwise.
     * All Vulkan handles with all extensions are supported. Developed for Vulkan version 1.3.
     */
    template<typename T>
    struct is_handle                                             { static constexpr bool value  = false; };
    template<> struct is_handle<VkInstance>                      { static constexpr bool value = true; };
    template<> struct is_handle<VkPhysicalDevice>                { static constexpr bool value = true; };
    template<> struct is_handle<VkDevice>                        { static constexpr bool value = true; };
    template<> struct is_handle<VkDeviceMemory>                  { static constexpr bool value = true; };
    template<> struct is_handle<VkBuffer>                        { static constexpr bool value = true; };
    template<> struct is_handle<VkImage>                         { static constexpr bool value = true; };
    template<> struct is_handle<VkQueue>                         { static constexpr bool value = true; };
    template<> struct is_handle<VkSemaphore>                     { static constexpr bool value = true; };
    template<> struct is_handle<VkCommandBuffer>                 { static constexpr bool value = true; };
    template<> struct is_handle<VkFence>                         { static constexpr bool value = true; };
    template<> struct is_handle<VkEvent>                         { static constexpr bool value = true; };
    template<> struct is_handle<VkQueryPool>                     { static constexpr bool value = true; };
    template<> struct is_handle<VkBufferView>                    { static constexpr bool value = true; };
    template<> struct is_handle<VkImageView>                     { static constexpr bool value = true; };
    template<> struct is_handle<VkShaderModule>                  { static constexpr bool value = true; };
    template<> struct is_handle<VkPipelineCache>                 { static constexpr bool value = true; };
    template<> struct is_handle<VkPipelineLayout>                { static constexpr bool value = true; };
    template<> struct is_handle<VkPipeline>                      { static constexpr bool value = true; };
    template<> struct is_handle<VkRenderPass>                    { static constexpr bool value = true; };
    template<> struct is_handle<VkDescriptorSetLayout>           { static constexpr bool value = true; };
    template<> struct is_handle<VkSampler>                       { static constexpr bool value = true; };
    template<> struct is_handle<VkDescriptorSet>                 { static constexpr bool value = true; };
    template<> struct is_handle<VkDescriptorPool>                { static constexpr bool value = true; };
    template<> struct is_handle<VkFramebuffer>                   { static constexpr bool value = true; };
    template<> struct is_handle<VkCommandPool>                   { static constexpr bool value = true; };

    // Vulkan version 1.1
    template<> struct is_handle<VkSamplerYcbcrConversion>        { static constexpr bool value = true; };
    template<> struct is_handle<VkDescriptorUpdateTemplate>      { static constexpr bool value = true; };

    // Vulkan version 1.3
    template<> struct is_handle<VkPrivateDataSlot>               { static constexpr bool value = true; };

    // Extension VK_KHR_surface
#if defined(VK_KHR_surface) && defined(VKA_ENABLE_KHR_SURFACE)
    template<> struct is_handle<VkSurfaceKHR>                    { static constexpr bool value = true; };
#endif

    // Extension VK_KHR_swapchain
#if defined(VK_KHR_swapchain) && defined(VKA_ENABLE_KHR_SWAPCHAIN)
    template<> struct is_handle<VkSwapchainKHR>                  { static constexpr bool value = true; };
#endif

    // Extension VK_KHR_display
#if defined(VK_KHR_display) && defined(VKA_ENABLE_KHR_DISPLAY)
    template<> struct is_handle<VkDisplayKHR>                    { static constexpr bool value = true; };
    template<> struct is_handle<VkDisplayModeKHR>                { static constexpr bool value = true; };
#endif

    // Extension VK_KHR_deferred_host_operations
#if defined(VK_KHR_deferred_host_operations) && defined(VKA_ENABLE_KHR_DEFERRED_HOST_OPERATIONS)
    template<> struct is_handle<VkDeferredOperationKHR>          { static constexpr bool value = true; };
#endif

    // Extension VK_EXT_debug_report
#if defined(VK_EXT_debug_report) && defined(VKA_ENABLE_EXT_DEBUG_REPORT)
    template<> struct is_handle<VkDebugReportCallbackEXT>        { static constexpr bool value = true; };
#endif

    // Extension VK_NVX_binary_import
#if defined(VK_NVX_binary_import) && defined(VKA_ENABLE_NVX_BINARY_IMPORT)
    template<> struct is_handle<VkCuModuleNVX>                   { static constexpr bool value = true; };
    template<> struct is_handle<VkCuFunctionNVX>                 { static constexpr bool value = true; };
#endif

    // Extension VK_EXT_debug_utils
#if defined(VK_EXT_debug_utils) && defined(VKA_ENABLE_EXT_DEBUG_UTILS)
    template<> struct is_handle<VkDebugUtilsMessengerEXT>        { static constexpr bool value = true; };
#endif

    // Extension VK_EXT_validation_cache
#if defined(VK_EXT_validation_cache) && defined(VKA_ENABLE_VALIDATION_CACHE)
    template<> struct is_handle<VkValidationCacheEXT>            { static constexpr bool value = true; };
#endif

    // Extension VK_NV_ray_tracing
#if defined(VK_NV_ray_tracing) && defined(VKA_ENABLE_NV_RAY_TRACING)
    template<> struct is_handle<VkAccelerationStructureNV>       { static constexpr bool value = true; };
#endif

    // Extension VK_INTEL_performance_query
#if defined(VK_INTEL_performance_query) && defined(VKA_ENABLE_INTEL_PERFORMANCE_QUERY)
    template<> struct is_handle<VkPerformanceConfigurationINTEL> { static constexpr bool value = true; };
#endif

    // Extension VK_NV_device_generated_commands
#if defined(VK_NV_device_generated_commands) && defined(VKA_ENABLE_NV_DEVICE_GENERATED_COMMANDS)
    template<> struct is_handle<VkIndirectCommandsLayoutNV>      { static constexpr bool value = true; };
#endif

    // Extension VK_EXT_descriptor_buffer
#if defined(VK_EXT_descriptor_buffer) && defined(VKA_ENABLE_EXT_DESCRIPTOR_BUFFER)
    template<> struct is_handle<VkAccelerationStructureKHR>      { static constexpr bool value = true; };
#endif

    // Extension VK_EXT_opacity_micromap
#if defined(VK_EXT_opacity_micromap) && defined(VKA_ENABLE_EXT_OPACITY_MICIOMAP)
    template<> struct is_handle<VkMicromapEXT>                   { static constexpr bool value = true; };
#endif

    // Extension VK_NV_optical_flow
#if defined(VK_NV_optical_flow) && defined(VKA_ENABLE_NV_OPTICAL_FLOW)
    template<> struct is_handle<VkOpticalFlowSessionNV>          { static constexpr bool value = true; };
#endif

    /*
     * is_deletable::value stores 'true', if T is a Vulkan handle which is deletable.
     * Sores 'false', if the type T is not a Vulkan handle or is not deletable.
     */
    template<typename T>
    struct is_deletable                                                 { static constexpr bool value = false; };
    template<> struct is_deletable<VkInstance>                          { static constexpr bool value = true; };
    template<> struct is_deletable<VkDevice>                            { static constexpr bool value = true; };
    template<> struct is_deletable<VkDeviceMemory>                      { static constexpr bool value = true; };
    template<> struct is_deletable<VkFence>                             { static constexpr bool value = true; };
    template<> struct is_deletable<VkSemaphore>                         { static constexpr bool value = true; };
    template<> struct is_deletable<VkEvent>                             { static constexpr bool value = true; };
    template<> struct is_deletable<VkQueryPool>                         { static constexpr bool value = true; };
    template<> struct is_deletable<VkBuffer>                            { static constexpr bool value = true; };
    template<> struct is_deletable<VkBufferView>                        { static constexpr bool value = true; };
    template<> struct is_deletable<VkImage>                             { static constexpr bool value = true; };
    template<> struct is_deletable<VkImageView>                         { static constexpr bool value = true; };
    template<> struct is_deletable<VkShaderModule>                      { static constexpr bool value = true; };
    template<> struct is_deletable<VkPipelineCache>                     { static constexpr bool value = true; };
    template<> struct is_deletable<VkPipeline>                          { static constexpr bool value = true; };
    template<> struct is_deletable<VkPipelineLayout>                    { static constexpr bool value = true; };
    template<> struct is_deletable<VkSampler>                           { static constexpr bool value = true; };
    template<> struct is_deletable<VkDescriptorSetLayout>               { static constexpr bool value = true; };
    template<> struct is_deletable<VkDescriptorPool>                    { static constexpr bool value = true; };
    template<> struct is_deletable<VkDescriptorSet>                     { static constexpr bool value = true; };
    template<> struct is_deletable<VkFramebuffer>                       { static constexpr bool value = true; };
    template<> struct is_deletable<VkRenderPass>                        { static constexpr bool value = true; };
    template<> struct is_deletable<VkCommandPool>                       { static constexpr bool value = true; };
    template<> struct is_deletable<VkCommandBuffer>                     { static constexpr bool value = true; };

    // Vulkan version 1.1
    template<> struct is_deletable<VkSamplerYcbcrConversion>            { static constexpr bool value = true; };
    template<> struct is_deletable<VkDescriptorUpdateTemplate>          { static constexpr bool value = true; };

    // Vulkan version 1.3
    template<> struct is_deletable<VkPrivateDataSlot>                   { static constexpr bool value = true; };

    // Extension VK_KHR_surface
#if defined(VK_KHR_surface) && defined(VKA_ENABLE_KHR_SURFACE)
    template<> struct is_deletable<VkSurfaceKHR>                        { static constexpr bool value = true; };
#endif

    // Extension VK_KHR_swapchain
#if defined(VK_KHR_swapchain) && defined(VKA_ENABLE_KHR_SWAPCHAIN)
    template<> struct is_deletable<VkSwapchainKHR>                      { static constexpr bool value = true; };
#endif

    // Extension VK_KHR_deferred_host_operations
#if defined(VK_KHR_deferred_host_operations) && defined(VKA_ENABLE_KHR_DEFERRED_HOST_OPERATIONS)
    template<> struct is_deletable<VkDeferredOperationKHR>              { static constexpr bool value = true; };
#endif

    // Extension VK_EXT_debug_report
#if defined(VK_EXT_debug_report) && defined(VKA_ENABLE_EXT_DEBUG_REPORT)
    template<> struct is_deletable<VkDebugReportCallbackEXT>            { static constexpr bool value = true; };
#endif

    // Extension VK_NVX_binary_import
#if defined(VK_NVX_binary_import) && defined(VKA_ENABLE_NVX_BINARY_IMPORT)
    template<> struct is_deletable<VkCuModuleNVX>                       { static constexpr bool value = true; };
    template<> struct is_deletable<VkCuFunctionNVX>                     { static constexpr bool value = true; };
#endif

    // Extension VK_EXT_debug_utils
#if defined(VK_EXT_debug_utils) && defined(VKA_ENABLE_EXT_DEBUG_UTILS)
    template<> struct is_deletable<VkDebugUtilsMessengerEXT>            { static constexpr bool value = true; };
#endif

    // Extension VK_EXT_validation_cache
#if defined(VK_EXT_validation_cache) && defined(VKA_ENABLE_VALIDATION_CACHE)
    template<> struct is_deletable<VkValidationCacheEXT>                { static constexpr bool value = true; };
#endif

    // Extension VK_NV_ray_tracing
#if defined(VK_NV_ray_tracing) && defined(VKA_ENABLE_NV_RAY_TRACING)
    template<> struct is_deletable<VkAccelerationStructureNV>           { static constexpr bool value = true; };
#endif

    // Extension VK_INTEL_performance_query
#if defined(VK_INTEL_performance_query) && defined(VKA_ENABLE_INTEL_PERFORMANCE_QUERY)
    template<> struct is_deletable<VkPerformanceConfigurationINTEL>     { static constexpr bool value = true; };
#endif

    // Extension VK_NV_device_generated_commands
#if defined(VK_NV_device_generated_commands) && defined(VKA_ENABLE_NV_DEVICE_GENERATED_COMMANDS)
    template<> struct is_deletable<VkIndirectCommandsLayoutNV>          { static constexpr bool value = true; };
#endif

    // Extension VK_EXT_opacity_micromap
#if defined(VK_EXT_opacity_micromap) && defined(VKA_ENABLE_EXT_OPACITY_MICIOMAP)
    template<> struct is_deletable<VkMicromapEXT>                       { static constexpr bool value = true; };
#endif

    // Extension VK_NV_optical_flow
#if defined(VK_NV_optical_flow) && defined(VKA_ENABLE_NV_OPTICAL_FLOW)
    template<> struct is_deletable<VkOpticalFlowSessionNV>              { static constexpr bool value = true; };
#endif

    // Extension VK_KHR_acceleration_structure
#if defined(VK_KHR_acceleration_structure) && defined(VK_EXT_descriptor_buffer) && defined(VKA_ENABLE_KHR_ACCELERATION_STRUCTURE)
    template<> struct is_deletable<VkAccelerationStructureKHR>          { static constexpr bool value = true; };
#endif

    /*
     * is_array_deletable::value stores 'true', if T is a Vulkan handle and can be deleted as an array.
     * Stores 'false', if T is not a Vulkan handle or can not be deleted as an array.
     */
    template<typename T>
    struct is_array_deletable { static constexpr bool value = false; };
    template<> struct is_array_deletable<VkDescriptorSet>   { static constexpr bool value = true; };
    template<> struct is_array_deletable<VkCommandBuffer>   { static constexpr bool value = true; };

    /*
     * is_owner::value stores 'true', if T is a Vulkan handle and is an owner class of other handles.
     * Stores 'false', if T is not a Vulkan handle, or if the handle is no owner class.
     */
    template<typename T>
    struct is_owner                         { static constexpr bool value = false; };
    template<> struct is_owner<VkInstance>  { static constexpr bool value = true; };
    template<> struct is_owner<VkDevice>    { static constexpr bool value = true; };
    template<> struct is_owner<void>        { static constexpr bool value = true; };

    /*
     * is_secondary_owner::value stores 'true', if T is a Vulkan handle and is a secondary owner class
     * of other handles. Stores 'false', if T is not a Vulkan handle, or if the handle is no secondary
     * owner class. Secondary owner classes are handles which are owned by another handle and are itself
     * owner handles. An example for such an owner class would be VkCommandPool. VkCommandPool is owned
     * by VkDevice and is itself owner of VkCommandBuffer handles.
     */
    template<typename T>
    struct is_secondary_owner { static constexpr bool value = false; };
    template<> struct is_secondary_owner<VkDescriptorPool>  { static constexpr bool value = true; };
    template<> struct is_secondary_owner<VkCommandPool>     { static constexpr bool value = true; };
    template<> struct is_secondary_owner<void>     { static constexpr bool value = true; };

    /*
     * Maps a deletable Vulkan handle to its owner class. For the most handles it will be VkDevice,
     * therefore this is the default. If a handle has no owner class, like owner handles itself, the
     * defined owner class will be of type 'void'.
     */
    template<typename T>
    struct get_owner
    {
        static_assert(is_deletable<T>::value, "[vka::get_owner]: Template type must be a deletable Vulkan handle.");
        using Owner = VkDevice;
    };
    template<> struct get_owner<VkInstance>                 { using Owner = void; };
    template<> struct get_owner<VkDevice>                   { using Owner = void; };
    template<> struct get_owner<VkSurfaceKHR>               { using Owner = VkInstance; };
    template<> struct get_owner<VkDebugReportCallbackEXT>   { using Owner = VkInstance; };
    template<> struct get_owner<VkDebugUtilsMessengerEXT>   { using Owner = VkInstance; };

    /*
     * Maps a deletable Vulkan handle to its secondary owner class. For the most handles it will be
     * 'void' as they have none, therefore this is the default.
     */
    template<typename T>
    struct get_secondary_owner
    {
        static_assert(is_deletable<T>::value, "[vka::get_secondary_owner]: Template type must be a deletable Vulkan handle.");
        using Owner = void;
    };
    template<> struct get_secondary_owner<VkDescriptorSet>  { using Owner = VkDescriptorPool; };
    template<> struct get_secondary_owner<VkCommandBuffer>  { using Owner = VkCommandPool; };

    // A wrapper class which stores the deleter function for a specific vulkan handle.
    template<typename T>
    struct HandleDeleter { static_assert(false, "[vka]: Invalid handle deleter!"); };
    template<> struct HandleDeleter<VkInstance>                         { static constexpr auto deleter = vkDestroyInstance; };
    template<> struct HandleDeleter<VkDevice>                           { static constexpr auto deleter = vkDestroyDevice; };
    template<> struct HandleDeleter<VkDeviceMemory>                     { static constexpr auto deleter = vkFreeMemory; };
    template<> struct HandleDeleter<VkFence>                            { static constexpr auto deleter = vkDestroyFence; };
    template<> struct HandleDeleter<VkSemaphore>                        { static constexpr auto deleter = vkDestroySemaphore; };
    template<> struct HandleDeleter<VkEvent>                            { static constexpr auto deleter = vkDestroyEvent; };
    template<> struct HandleDeleter<VkQueryPool>                        { static constexpr auto deleter = vkDestroyQueryPool; };
    template<> struct HandleDeleter<VkBuffer>                           { static constexpr auto deleter = vkDestroyBuffer; };
    template<> struct HandleDeleter<VkBufferView>                       { static constexpr auto deleter = vkDestroyBufferView; };
    template<> struct HandleDeleter<VkImage>                            { static constexpr auto deleter = vkDestroyImage; };
    template<> struct HandleDeleter<VkImageView>                        { static constexpr auto deleter = vkDestroyImageView; };
    template<> struct HandleDeleter<VkShaderModule>                     { static constexpr auto deleter = vkDestroyShaderModule; };
    template<> struct HandleDeleter<VkPipelineCache>                    { static constexpr auto deleter = vkDestroyPipelineCache; };
    template<> struct HandleDeleter<VkPipeline>                         { static constexpr auto deleter = vkDestroyPipeline; };
    template<> struct HandleDeleter<VkPipelineLayout>                   { static constexpr auto deleter = vkDestroyPipelineLayout; };
    template<> struct HandleDeleter<VkSampler>                          { static constexpr auto deleter = vkDestroySampler; };
    template<> struct HandleDeleter<VkDescriptorSetLayout>              { static constexpr auto deleter = vkDestroyDescriptorSetLayout; };
    template<> struct HandleDeleter<VkDescriptorPool>                   { static constexpr auto deleter = vkDestroyDescriptorPool; };
    template<> struct HandleDeleter<VkDescriptorSet>                    { static constexpr auto deleter = vkFreeDescriptorSets; };
    template<> struct HandleDeleter<VkFramebuffer>                      { static constexpr auto deleter = vkDestroyFramebuffer; };
    template<> struct HandleDeleter<VkRenderPass>                       { static constexpr auto deleter = vkDestroyRenderPass; };
    template<> struct HandleDeleter<VkCommandPool>                      { static constexpr auto deleter = vkDestroyCommandPool; };
    template<> struct HandleDeleter<VkCommandBuffer>                    { static constexpr auto deleter = vkFreeCommandBuffers; };

    // Vulkan version 1.1
    template<> struct HandleDeleter<VkSamplerYcbcrConversion>           { static constexpr auto deleter = vkDestroySamplerYcbcrConversion; };
    template<> struct HandleDeleter<VkDescriptorUpdateTemplate>         { static constexpr auto deleter = vkDestroyDescriptorUpdateTemplate; };

    // Vulkan version 1.3
    template<> struct HandleDeleter<VkPrivateDataSlot>                  { static constexpr auto deleter = vkDestroyPrivateDataSlot; };

    // Extension VK_KHR_surface
#if defined(VK_KHR_surface) && defined(VKA_ENABLE_KHR_SURFACE)
    template<> struct HandleDeleter<VkSurfaceKHR>                       { static constexpr auto deleter = vkDestroySurfaceKHR; };
#endif

    // Extension VK_KHR_swapchain
#if defined(VK_KHR_swapchain) && defined(VKA_ENABLE_KHR_SWAPCHAIN)
    template<> struct HandleDeleter<VkSwapchainKHR>                     { static constexpr auto deleter = vkDestroySwapchainKHR; };
#endif

    // Extension VK_KHR_deferred_host_operations
#if defined(VK_KHR_deferred_host_operations) && defined(VKA_ENABLE_KHR_DEFERRED_HOST_OPERATIONS)
    template<> struct HandleDeleter<VkDeferredOperationKHR>             { static constexpr auto deleter = vkDestroyDeferredOperationKHR; };
#endif

    // Extension VK_EXT_debug_report
#if defined(VK_EXT_debug_report) && defined(VKA_ENABLE_EXT_DEBUG_REPORT)
    template<> struct HandleDeleter<VkDebugReportCallbackEXT>           { static constexpr auto deleter = vkDestroyDebugReportCallbackEXT; };
#endif

    // Extension VK_NVX_binary_import
#if defined(VK_NVX_binary_import) && defined(VKA_ENABLE_NVX_BINARY_IMPORT)
    template<> struct HandleDeleter<VkCuModuleNVX>                      { static constexpr auto deleter = vkDestroyCuModuleNVX; };
    template<> struct HandleDeleter<VkCuFunctionNVX>                    { static constexpr auto deleter = vkDestroyCuFunctionNVX; };
#endif

    // Extension VK_EXT_debug_utils
#if defined(VK_EXT_debug_utils) && defined(VKA_ENABLE_EXT_DEBUG_UTILS)
    template<> struct HandleDeleter<VkDebugUtilsMessengerEXT>           { static constexpr auto deleter = vkDestroyDebugUtilsMessengerEXT; };
#endif

    // Extension VK_EXT_validation_cache
#if defined(VK_EXT_validation_cache) && defined(VKA_ENABLE_VALIDATION_CACHE)
    template<> struct HandleDeleter<VkValidationCacheEXT>               { static constexpr auto deleter = vkDestroyValidationCacheEXT; };
#endif

    // Extension VK_NV_ray_tracing
#if defined(VK_NV_ray_tracing) && defined(VKA_ENABLE_NV_RAY_TRACING)
    template<> struct HandleDeleter<VkAccelerationStructureNV>          { static constexpr auto deleter = vkDestroyAccelerationStructureNV; };
#endif

    // Extension VK_INTEL_performance_query
#if defined(VK_INTEL_performance_query) && defined(VKA_ENABLE_INTEL_PERFORMANCE_QUERY)
    template<> struct HandleDeleter<VkPerformanceConfigurationINTEL>    { static constexpr auto deleter = vkReleasePerformanceConfigurationINTEL; };
#endif

    // Extension VK_NV_device_generated_commands
#if defined(VK_NV_device_generated_commands) && defined(VKA_ENABLE_NV_DEVICE_GENERATED_COMMANDS)
    template<> struct HandleDeleter<VkIndirectCommandsLayoutNV>         { static constexpr auto deleter = vkDestroyIndirectCommandsLayoutNV; };
#endif

    // Extension VK_EXT_opacity_micromap
#if defined(VK_EXT_opacity_micromap) && defined(VKA_ENABLE_EXT_OPACITY_MICIOMAP)
    template<> struct HandleDeleter<VkMicromapEXT>                      { static constexpr auto deleter = vkDestroyMicromapEXT; };
#endif

    // Extension VK_NV_optical_flow
#if defined(VK_NV_optical_flow) && defined(VKA_ENABLE_NV_OPTICAL_FLOW)
    template<> struct HandleDeleter<VkOpticalFlowSessionNV>             { static constexpr auto deleter = vkDestroyOpticalFlowSessionNV; };
#endif

    // Extension VK_KHR_acceleration_structure
#if defined(VK_KHR_acceleration_structure) && defined(VK_EXT_descriptor_buffer) && defined(VKA_ENABLE_KHR_ACCELERATION_STRUCTURE)
    template<> struct HandleDeleter<VkAccelerationStructureKHR>         { static constexpr auto deleter = vkDestroyAccelerationStructureKHR; };
#endif

    /**
     * Note all unique_handle-classes are implemented in line as external implemented functions of
     * big template-classes are very confusing. It is much cleaner and easier to read, if they are
     * implemented in line. Additionally, in line implemented functions are implicit declared as
     * inline. Therefore, all functions of unique_handle and unique_handle_base are inline-functions.
     */

    /*
     * This class serves as a collection of functions that are equal over all template specializations
     * of the subclass unique_handle. However, this class stores the actual vulkan handle, but it does
     * not delete it, in the destructor. This is managed by the subclass and its corresponding template
     * specializations. For that reason this class cannot be instanced, and it must not exist a pointer
     * to the base class as the destructors are not virtual.
     */
    template<typename Handle>
    class unique_handle_base
    {
    protected:
        Handle m_handle;

        // Constructors are protected as this class must not be instanced from outside.
        explicit unique_handle_base(Handle handle) noexcept
        : m_handle(handle)
        {}

        unique_handle_base(unique_handle_base&& src) noexcept
        : m_handle(src.m_handle)
        {
            src.m_handle = VK_NULL_HANDLE;
        }

    public:

        // must not be copied
        unique_handle_base(const unique_handle_base&) = delete;
        unique_handle_base& operator= (const unique_handle_base&) = delete;

        // The subclass manages deleting the handle.
        ~unique_handle_base(void) = default;

        // Returns the vulkan handle which is owned by the unique handle object.
        Handle handle(void) const noexcept
        {
            return this->m_handle;
        }

        // Converts a unique handle object to the respective vulkan handle.
        VKA_CONVERT_EXPLICIT operator Handle(void) const noexcept
        {
            return this->m_handle;
        }

        // Compares 2 handles for equality and returns 'true', if they are equal.
        bool operator== (const unique_handle_base& src) const noexcept
        {
            return this->m_handle == src.m_handle;
        }
        bool operator== (Handle src) const noexcept
        {
            return this->m_handle == src;
        }

        // Compares 2 handles for not equal and returns 'true', if they are not equal.
        bool operator!= (const unique_handle_base& src) const noexcept
        {
            return this->m_handle != src.m_handle;
        }
        bool operator!= (Handle src) const noexcept
        {
            return this->m_handle != src;
        }

        // Compares 2 handles for less than.
        bool operator< (const unique_handle_base& src) const noexcept
        {
            return this->m_handle < src.m_handle;
        }
        bool operator< (Handle src) const noexcept
        {
            return this->m_handle < src;
        }

        // Compares 2 handles for less than or equal.
        bool operator<= (const unique_handle_base& src) const noexcept
        {
            return this->m_handle <= src.m_handle;
        }
        bool operator<= (Handle src) const noexcept
        {
            return this->m_handle <= src;
        }

        // Compares 2 handles for greater than.
        bool operator> (const unique_handle_base& src) const noexcept
        {
            return this->m_handle > src.m_handle;
        }
        bool operator> (Handle src) const noexcept
        {
            return this->m_handle > src;
        }

        // Compares 2 handles for greater than or equal.
        bool operator>= (const unique_handle_base& src) const noexcept
        {
            return this->m_handle >= src.m_handle;
        }
        bool operator>= (Handle src) const noexcept
        {
            return this->m_handle >= src;
        }
    };

    /*
     * This is the fallback and at the same time the default implementation of unique_handle.
     * It manages those handles which require an owner class and a secondary owner class in
     * order to be deleted.
     * TODO: There is no support for custom allocation callbacks and storing it for every instance
     * TODO: of unique handle would be overkill.
     */
    template<typename Handle, auto deleter, typename Owner, typename Owner2>
    class unique_handle final : public unique_handle_base<Handle>
    {
        static_assert(is_deletable<Handle>::value, "[vka::unique_handle]: unique_handle can only be used with deletable Vulkan handles.");
        static_assert(is_owner<Owner>::value, "[vka::unique_handle]: Template argument 'Owner' is not a valid owner class.");
        static_assert(is_secondary_owner<Owner2>::value, "[vka::unique_handle]: Template argument 'Owner2' is not a valid secondary owner class.");

    private:
        Owner m_primary;
        Owner2 m_secondary;

    public:
        /*
         * Initializes this unique handle with a device and a handle which should be deleted, if 'this'
         * is destroyed.
         */
        explicit unique_handle(Handle handle, Owner primary, Owner2 secondary) noexcept
        : unique_handle_base<Handle>(handle), m_primary(primary), m_secondary(secondary)
        {}

        // must not be copied, equivalent deleted operator is inherited
        unique_handle(const unique_handle&) = delete;

        /*
         * Moves another object of unique handle from 'src' into 'this'. After moving, the source
         * unique handle becomes invalidated and its handle value stores VK_NULL_HANDLE. Therefore,
         * the moved unique handle cannot be used in any operation.
         */
        unique_handle(unique_handle&& src) noexcept
        : unique_handle_base<Handle>(std::move(src)), m_primary(src.m_primary), m_secondary(src.m_secondary)
        {}
        unique_handle& operator= (unique_handle&& src) noexcept
        {
            this->m_handle = src.m_handle;
            this->m_primary = src.m_primary;
            this->m_secondary = src.m_secondary;
            src.m_handle = VK_NULL_HANDLE;
            return *this;
        }

        // Handle is deleted in the destructor.
        ~unique_handle(void)
        {
            if (this->m_handle != VK_NULL_HANDLE)
                deleter(this->m_primary, this->m_secondary, 1, &this->m_handle);
        }
    };

     // This specialization of unique_handle only requires one owner class in order to be deleted.
    template<typename Handle, auto deleter, typename Owner>
    class unique_handle<Handle, deleter, Owner, void> final : public unique_handle_base<Handle>
    {
        // TODO: check if they are really required
        //static_assert(is_deletable<Handle>::value, "[vka::unique_handle]: unique_handle can only be used with deletable Vulkan handles.");
        //static_assert(is_owner<Owner>::value, "[vka::unique_handle]: Template argument 'Owner' is not a valid owner class.");

    private:
        Owner m_primary;

    public:
        explicit unique_handle(Handle handle, Owner primary) noexcept
        : unique_handle_base<Handle>(handle), m_primary(primary)
        {}

        // must not be copied, equivalent deleted operator is inherited
        unique_handle(const unique_handle&) = delete;

        /*
         * Moves another object of unique handle from 'src' into 'this'. After moving, the source
         * unique handle becomes invalidated and its handle value stores VK_NULL_HANDLE. Therefore,
         * the moved unique handle cannot be used in any operation.
         */
        unique_handle(unique_handle&& src) noexcept
        : unique_handle_base<Handle>(std::move(src)), m_primary(src.m_primary)
        {}
        unique_handle& operator= (unique_handle&& src) noexcept
        {
            this->m_handle = src.m_handle;
            this->m_primary = src.m_primary;
            src.m_handle = VK_NULL_HANDLE;
            return *this;
        }

        // Handle is deleted in the destructor.
        ~unique_handle(void)
        {
            if (this->m_handle != VK_NULL_HANDLE)
                deleter(this->m_primary, this->m_handle, nullptr);
        }
    };

    // This specialization of unique_handle does not require any owner class in order to be deleted.
    template<typename Handle, auto deleter>
    class unique_handle<Handle, deleter, void, void> final : public unique_handle_base<Handle>
    {
        //static_assert(is_deletable<Handle>::value, "[vka::unique_handle]: unique_handle can only be used with deletable Vulkan handles.");
    public:
        // Initializes this unique handle with a handle which should be deleted, if 'this' is destroyed.
        explicit unique_handle(Handle handle) noexcept
        : unique_handle_base<Handle>(handle)
        {}

        // must not be copied, equivalent deleted operator is inherited
        unique_handle(const unique_handle&) = delete;

        /*
         * Moves another object of unique handle from 'src' into 'this'. After moving, the source
         * unique handle becomes invalidated and its handle value stores VK_NULL_HANDLE. Therefore,
         * the moved unique handle cannot be used in any operation.
         */
        unique_handle(unique_handle&& src) noexcept
        : unique_handle_base<Handle>(std::move(src))
        {}
        unique_handle& operator= (unique_handle&& src) noexcept
        {
            this->m_handle = src.m_handle;
            src.m_handle = VK_NULL_HANDLE;
            return *this;
        }

        // Handle is deleted in the destructor.
        ~unique_handle(void)
        {
            if (this->m_handle != VK_NULL_HANDLE)
                deleter(this->m_handle, nullptr);
        }
    };

    /*
     * This class does the same thing as unique_handle but for an array of handles. Only handles are
     * valid which are array-deletable. Some handles can directly be allocated and deleted as an array
     * like for example VkCommandBuffer. Those types of handles are array-deletable and thus valid for
     * unique_handle_array. Additionally, such handles are also owned another handle which, if the owner
     * is deleted, all handles created/allocated from it also get deleted. Example: VkCommandPool is the
     * owner of VkCommandBuffer-s and, if the VkCommandPool is deleted, it deletes all VkCommandBuffer
     * handles allocated from it. Therefore, use this class wisely because there is no check, if the
     * handles owned by this unique_handle_array have already been deleted implicitly by the owner
     * handle. As this class is meant to be directly initialized with an array of handles which were
     * created / allocated all at once, it only checks, if the first handle is VK_NULL_HANDLE at
     * destruction. Consequently, use multiple objects of unique_handle_array, if you have multiple
     * batches of handles.
     * NOTE: The owner handles mentioned here are secondary owner classes. Deleting primary owner
     * classes would result in an error, if the subsequent handles created from it have not been deleted
     * before.
     */
    template<typename Handle, size_t N, auto deleter, typename Owner, typename Owner2>
    class unique_handle_array
    {
        static_assert(is_array_deletable<Handle>::value, "[vka::unique_handle_array]: unique_handle_array can only be used with array-deletable Vulkan handles.");
        static_assert(is_owner<Owner>::value, "[vka::unique_handle_array]: Template argument 'Owner' is not a valid owner class.");
        static_assert(is_secondary_owner<Owner2>::value, "[vka::unique_handle_array]: Template argument 'Owner2' is not a valid secondary owner class.");
        static_assert(N > 0, "[vka::unique_handle_array]: Template argument 'N' must be >= 1");

    private:
        Handle m_handles[N];
        Owner m_primary;
        Owner m_secondary;

        void copy_handles(const Handle* handles) noexcept
        {
            for (size_t i = 0; i < N; i++)
                this->m_handles[i] = handles[i];
        }

    public:
        explicit unique_handle_array(const Handle* handles, Owner primary, Owner2 secondary) noexcept
        : m_handles(copy_handles(handles)), m_primary(primary), m_secondary(secondary)
        {}

        // unique_handle_array must not be copied
        unique_handle_array(const unique_handle_array&) = delete;
        unique_handle_array& operator= (const unique_handle_array&) = delete;

        unique_handle_array(unique_handle_array&& src) noexcept
        : m_handles(copy_handles(src.m_handles)), m_primary(src.m_primary), m_secondary(src.m_secondary)
        {
            // It is enough to set the first handle of source to VK_NULL_HANDLE as this will already
            // invalidate it.
            src.m_handles[0] = VK_NULL_HANDLE;
        }
        unique_handle_array& operator= (unique_handle_array&& src) noexcept
        {
            this->copy_handles(src.m_handles);
            this->m_primary = src.m_primary;
            this->m_secondary = src.m_secondary;
            src.m_handles[0] = VK_NULL_HANDLE;
            return *this;
        }

        ~unique_handle_array(void)
        {
            // Normally, if the creation of the handle array failed, every handle is VK_NULL_HANDLE.
            // Otherwise, every handle is not a VK_NULL_HANDLE, if the creation of the handle array
            // was successful. Therefore, it is expected that every owned handle is either VK_NULL_HANDLE
            // or a valid handle.
            if (this->m_handles[0] != VK_NULL_HANDLE)
                deleter(this->m_primary, this->m_secondary, N, this->m_handles);
        }

        Handle handle(size_t idx) const
        {
            if (idx >= N) [[unlikely]]
                throw std::out_of_range("[vka::unique_handle_array::handle]: Accessing handle out of range.");
            return this->m_handles[idx];
        }

        Handle operator[] (size_t idx) const noexcept
        {
            return this->m_handles[idx];
        }

        const Handle* array(void) const noexcept
        {
            return this->m_handles;
        }

        VKA_CONVERT_EXPLICIT operator const Handle*(void) const noexcept
        {
            return this->m_handles;
        }
    };

    constexpr VkImageUsageFlagBits ff2iu_bit(VkFormatFeatureFlagBits format_feature) noexcept;
    constexpr VkFormatFeatureFlagBits iu2ff_bit(VkImageUsageFlagBits image_usage) noexcept;
    void init_format_sizeof(std::unordered_map<VkFormat, size_t>& f2s);
} // namespace vka::detail::common

#ifdef VKA_IMPLEMENTATION
    #include "vka_common_impl.inl"
#endif
#include "vka_common_inline_impl.inl"
