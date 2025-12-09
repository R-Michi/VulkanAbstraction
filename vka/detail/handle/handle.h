#pragma once

namespace vka::detail::handle
{
    /// ################################################################################################################
    /// ################################################# Deleter-types ################################################
    /// ################################################################################################################

    template<typename Handle>
    using ParentDestroyFunc = void(*)(Handle, const VkAllocationCallbacks*);

    template<typename Handle>
    using ParentDestroyFuncNX = void(*)(Handle, const VkAllocationCallbacks*) noexcept;

    template<typename Handle, typename Parent>
    using DestroyFunc = void(*)(Parent, Handle, const VkAllocationCallbacks*);

    template<typename Handle, typename Parent>
    using DestroyFuncNX = void(*)(Parent, Handle, const VkAllocationCallbacks*) noexcept;

    // -- parent destroy func --

    template<typename Func>
    struct is_parent_destroy_func
    { static constexpr bool value = false; };

    template<typename Handle>
    struct is_parent_destroy_func<ParentDestroyFunc<Handle>>
    { static constexpr bool value = true; };

    template<typename Handle>
    struct is_parent_destroy_func<ParentDestroyFunc<const Handle&>>
    { static constexpr bool value = true; };

    template<typename Handle>
    struct is_parent_destroy_func<ParentDestroyFuncNX<Handle>>
    { static constexpr bool value = true; };

    template<typename Handle>
    struct is_parent_destroy_func<ParentDestroyFuncNX<const Handle&>>
    { static constexpr bool value = true; };

    // -- destroy func --

    template<typename Func>
    struct is_destroy_func
    { static constexpr bool value = false; };

    template<typename Handle, typename Parent>
    struct is_destroy_func<DestroyFunc<Handle, Parent>>
    { static constexpr bool value = true; };

    template<typename Handle, typename Parent>
    struct is_destroy_func<DestroyFunc<const Handle&, Parent>>
    { static constexpr bool value = true; };

    template<typename Handle, typename Parent>
    struct is_destroy_func<DestroyFunc<Handle, const Parent&>>
    { static constexpr bool value = true; };

    template<typename Handle, typename Parent>
    struct is_destroy_func<DestroyFunc<const Handle&, const Parent&>>
    { static constexpr bool value = true; };

    template<typename Handle, typename Parent>
    struct is_destroy_func<DestroyFuncNX<Handle, Parent>>
    { static constexpr bool value = true; };

    template<typename Handle, typename Parent>
    struct is_destroy_func<DestroyFuncNX<const Handle&, Parent>>
    { static constexpr bool value = true; };

    template<typename Handle, typename Parent>
    struct is_destroy_func<DestroyFuncNX<Handle, const Parent&>>
    { static constexpr bool value = true; };

    template<typename Handle, typename Parent>
    struct is_destroy_func<DestroyFuncNX<const Handle&, const Parent&>>
    { static constexpr bool value = true; };

    // -- concepts --

    template<typename Func>
    concept parent_destroy_func_c = is_parent_destroy_func<Func>::value;

    template<typename Func>
    concept destroy_func_c = is_destroy_func<Func>::value;

    /// ################################################################################################################
    /// ################################################## Constraints #################################################
    /// ################################################################################################################

    template<typename T>
    concept copyable_c = requires(T a, T b)
    {
        b = a;
    };

    template<typename T>
    concept bool_convertable_c = requires(T a)
    {
        static_cast<bool>(a);
    };

    /// ################################################################################################################
    /// ################################################# Parent-handle ################################################
    /// ################################################################################################################

    template<typename T>
    struct parent                                           { using type = VkDevice;    };
    template<> struct parent<VkInstance>                    { using type = void;        };
    template<> struct parent<VkDevice>                      { using type = void;        };
    template<> struct parent<VkDebugReportCallbackEXT>      { using type = VkInstance;  };
    template<> struct parent<VkDebugUtilsMessengerEXT>      { using type = VkInstance;  };

    template<typename T>
    using parent_t = parent<T>::type;

    /// ################################################################################################################
    /// ############################################### Destroy function ###############################################
    /// ################################################################################################################

    template<typename T>
    struct destroy_func                                             { static constexpr std::nullptr_t func = nullptr;                       };

    // core handles
    template<> struct destroy_func<VkInstance>                      { static constexpr auto func = vkDestroyInstance;                       };
    template<> struct destroy_func<VkDevice>                        { static constexpr auto func = vkDestroyDevice;                         };
    template<> struct destroy_func<VkFence>                         { static constexpr auto func = vkDestroyFence;                          };
    template<> struct destroy_func<VkSemaphore>                     { static constexpr auto func = vkDestroySemaphore;                      };
    template<> struct destroy_func<VkEvent>                         { static constexpr auto func = vkDestroyEvent;                          };
    template<> struct destroy_func<VkQueryPool>                     { static constexpr auto func = vkDestroyQueryPool;                      };
    template<> struct destroy_func<VkBuffer>                        { static constexpr auto func = vkDestroyBuffer;                         };
    template<> struct destroy_func<VkBufferView>                    { static constexpr auto func = vkDestroyBufferView;                     };
    template<> struct destroy_func<VkImage>                         { static constexpr auto func = vkDestroyImage;                          };
    template<> struct destroy_func<VkImageView>                     { static constexpr auto func = vkDestroyImageView;                      };
    template<> struct destroy_func<VkShaderModule>                  { static constexpr auto func = vkDestroyShaderModule;                   };
    template<> struct destroy_func<VkPipelineCache>                 { static constexpr auto func = vkDestroyPipelineCache;                  };
    template<> struct destroy_func<VkPipeline>                      { static constexpr auto func = vkDestroyPipeline;                       };
    template<> struct destroy_func<VkPipelineLayout>                { static constexpr auto func = vkDestroyPipelineLayout;                 };
    template<> struct destroy_func<VkSampler>                       { static constexpr auto func = vkDestroySampler;                        };
    template<> struct destroy_func<VkDescriptorSetLayout>           { static constexpr auto func = vkDestroyDescriptorSetLayout;            };
    template<> struct destroy_func<VkDescriptorPool>                { static constexpr auto func = vkDestroyDescriptorPool;                 };
    template<> struct destroy_func<VkFramebuffer>                   { static constexpr auto func = vkDestroyFramebuffer;                    };
    template<> struct destroy_func<VkRenderPass>                    { static constexpr auto func = vkDestroyRenderPass;                     };
    template<> struct destroy_func<VkCommandPool>                   { static constexpr auto func = vkDestroyCommandPool;                    };
    template<> struct destroy_func<VkSamplerYcbcrConversion>        { static constexpr auto func = vkDestroySamplerYcbcrConversion;         };
    template<> struct destroy_func<VkDescriptorUpdateTemplate>      { static constexpr auto func = vkDestroyDescriptorUpdateTemplate;       };
    template<> struct destroy_func<VkPrivateDataSlot>               { static constexpr auto func = vkDestroyPrivateDataSlot;                };
    template<> struct destroy_func<VkDeviceMemory>                  { static constexpr auto func = vkFreeMemory;                            };

    // extension handles
    template<> struct destroy_func<VkSurfaceKHR>                    { static constexpr auto func = vkDestroySurfaceKHR;                     };
    template<> struct destroy_func<VkSwapchainKHR>                  { static constexpr auto func = vkDestroySwapchainKHR;                   };
    template<> struct destroy_func<VkVideoSessionKHR>               { static constexpr auto func = vkDestroyVideoSessionKHR;                };
    template<> struct destroy_func<VkVideoSessionParametersKHR>     { static constexpr auto func = vkDestroyVideoSessionParametersKHR;      };
    template<> struct destroy_func<VkDeferredOperationKHR>          { static constexpr auto func = vkDestroyDeferredOperationKHR;           };
    template<> struct destroy_func<VkPipelineBinaryKHR>             { static constexpr auto func = vkDestroyPipelineBinaryKHR;              };
    template<> struct destroy_func<VkDebugReportCallbackEXT>        { static constexpr auto func = vkDestroyDebugReportCallbackEXT;         };
    template<> struct destroy_func<VkCuModuleNVX>                   { static constexpr auto func = vkDestroyCuModuleNVX;                    };
    template<> struct destroy_func<VkCuFunctionNVX>                 { static constexpr auto func = vkDestroyCuFunctionNVX;                  };
    template<> struct destroy_func<VkDebugUtilsMessengerEXT>        { static constexpr auto func = vkDestroyDebugUtilsMessengerEXT;         };
    template<> struct destroy_func<VkValidationCacheEXT>            { static constexpr auto func = vkDestroyValidationCacheEXT;             };
    template<> struct destroy_func<VkAccelerationStructureNV>       { static constexpr auto func = vkDestroyAccelerationStructureNV;        };
    template<> struct destroy_func<VkIndirectCommandsLayoutNV>      { static constexpr auto func = vkDestroyIndirectCommandsLayoutNV;       };
    template<> struct destroy_func<VkMicromapEXT>                   { static constexpr auto func = vkDestroyMicromapEXT;                    };
    template<> struct destroy_func<VkTensorARM>                     { static constexpr auto func = vkDestroyTensorARM;                      };
    template<> struct destroy_func<VkTensorViewARM>                 { static constexpr auto func = vkDestroyTensorViewARM;                  };
    template<> struct destroy_func<VkOpticalFlowSessionNV>          { static constexpr auto func = vkDestroyOpticalFlowSessionNV;           };
    template<> struct destroy_func<VkShaderEXT>                     { static constexpr auto func = vkDestroyShaderEXT;                      };
    template<> struct destroy_func<VkDataGraphPipelineSessionARM>   { static constexpr auto func = vkDestroyDataGraphPipelineSessionARM;    };
    template<> struct destroy_func<VkExternalComputeQueueNV>        { static constexpr auto func = vkDestroyExternalComputeQueueNV;         };
    template<> struct destroy_func<VkIndirectCommandsLayoutEXT>     { static constexpr auto func = vkDestroyIndirectCommandsLayoutEXT;      };
    template<> struct destroy_func<VkIndirectExecutionSetEXT>       { static constexpr auto func = vkDestroyIndirectExecutionSetEXT;        };
    template<> struct destroy_func<VkAccelerationStructureKHR>      { static constexpr auto func = vkDestroyAccelerationStructureKHR;       };

    // custom handles
    template<> struct destroy_func<SwapchainFramebuffer>            { static constexpr auto func = swapchain::destroy_framebuffer;          };

    template<typename T>
    constexpr auto destroy_f = destroy_func<T>::func;

    /// ################################################################################################################
    /// #################################################### Utility ###################################################
    /// ################################################################################################################

    template<typename T>
    struct remove_array_s { using type = T; };

    template<typename T>
    struct remove_array_s<T[]> { using type = T; };

    template<typename T>
    using remove_array = remove_array_s<T>::type;

#ifdef _DEBUG
    constexpr bool debug_noexcept = false;
#else
    constexpr bool debug_noexcept = true;
#endif

    template<typename T, uint32_t N>
    constexpr bool always_false_v = false;

    // unused code but may be useful later
#if 0
    template<typename T>
    struct create_func { static constexpr std::nullptr_t func = nullptr; };

    // core handles
    template<> struct create_func<VkInstanceCreateInfo>                         { static constexpr auto func = vkCreateInstance;                    };  // parent handle
    template<> struct create_func<VkInstanceCreateInfo[]>                       { static constexpr auto func = vkCreateInstance;                    };  // parent handle
    template<> struct create_func<VkDeviceCreateInfo>                           { static constexpr auto func = vkCreateDevice;                      };  // parent handle
    template<> struct create_func<VkDeviceCreateInfo[]>                         { static constexpr auto func = vkCreateDevice;                      };  // parent handle
    template<> struct create_func<VkFenceCreateInfo>                            { static constexpr auto func = vkCreateFence;                       };
    template<> struct create_func<VkFenceCreateInfo[]>                          { static constexpr auto func = vkCreateFence;                       };
    template<> struct create_func<VkSemaphoreCreateInfo>                        { static constexpr auto func = vkCreateSemaphore;                   };
    template<> struct create_func<VkSemaphoreCreateInfo[]>                      { static constexpr auto func = vkCreateSemaphore;                   };
    template<> struct create_func<VkEventCreateInfo>                            { static constexpr auto func = vkCreateEvent;                       };
    template<> struct create_func<VkEventCreateInfo[]>                          { static constexpr auto func = vkCreateEvent;                       };
    template<> struct create_func<VkQueryPoolCreateInfo>                        { static constexpr auto func = vkCreateQueryPool;                   };
    template<> struct create_func<VkQueryPoolCreateInfo[]>                      { static constexpr auto func = vkCreateQueryPool;                   };
    template<> struct create_func<VkBufferCreateInfo>                           { static constexpr auto func = vkCreateBuffer;                      };
    template<> struct create_func<VkBufferCreateInfo[]>                         { static constexpr auto func = vkCreateBuffer;                      };
    template<> struct create_func<VkBufferViewCreateInfo>                       { static constexpr auto func = vkCreateBufferView;                  };
    template<> struct create_func<VkBufferViewCreateInfo[]>                     { static constexpr auto func = vkCreateBufferView;                  };
    template<> struct create_func<VkImageCreateInfo>                            { static constexpr auto func = vkCreateImage;                       };
    template<> struct create_func<VkImageCreateInfo[]>                          { static constexpr auto func = vkCreateImage;                       };
    template<> struct create_func<VkImageViewCreateInfo>                        { static constexpr auto func = vkCreateImageView;                   };
    template<> struct create_func<VkImageViewCreateInfo[]>                      { static constexpr auto func = vkCreateImageView;                   };
    template<> struct create_func<VkShaderModuleCreateInfo>                     { static constexpr auto func = vkCreateShaderModule;                };
    template<> struct create_func<VkShaderModuleCreateInfo[]>                   { static constexpr auto func = vkCreateShaderModule;                };
    template<> struct create_func<VkPipelineCacheCreateInfo>                    { static constexpr auto func = vkCreatePipelineCache;               };
    template<> struct create_func<VkPipelineCacheCreateInfo[]>                  { static constexpr auto func = vkCreatePipelineCache;               };
    template<> struct create_func<VkGraphicsPipelineCreateInfo>                 { static constexpr auto func = vkCreateGraphicsPipelines;           }; // special handling (pipeline)
    template<> struct create_func<VkGraphicsPipelineCreateInfo[]>               { static constexpr auto func = vkCreateGraphicsPipelines;           }; // special handling (pipeline)
    template<> struct create_func<VkComputePipelineCreateInfo>                  { static constexpr auto func = vkCreateComputePipelines;            }; // special handling (pipeline)
    template<> struct create_func<VkComputePipelineCreateInfo[]>                { static constexpr auto func = vkCreateComputePipelines;            }; // special handling (pipeline)
    template<> struct create_func<VkPipelineLayoutCreateInfo>                   { static constexpr auto func = vkCreatePipelineLayout;              };
    template<> struct create_func<VkPipelineLayoutCreateInfo[]>                 { static constexpr auto func = vkCreatePipelineLayout;              };
    template<> struct create_func<VkSamplerCreateInfo>                          { static constexpr auto func = vkCreateSampler;                     };
    template<> struct create_func<VkSamplerCreateInfo[]>                        { static constexpr auto func = vkCreateSampler;                     };
    template<> struct create_func<VkDescriptorSetLayoutCreateInfo>              { static constexpr auto func = vkCreateDescriptorSetLayout;         };
    template<> struct create_func<VkDescriptorSetLayoutCreateInfo[]>            { static constexpr auto func = vkCreateDescriptorSetLayout;         };
    template<> struct create_func<VkDescriptorPoolCreateInfo>                   { static constexpr auto func = vkCreateDescriptorPool;              };
    template<> struct create_func<VkDescriptorPoolCreateInfo[]>                 { static constexpr auto func = vkCreateDescriptorPool;              };
    template<> struct create_func<VkFramebufferCreateInfo>                      { static constexpr auto func = vkCreateFramebuffer;                 };
    template<> struct create_func<VkFramebufferCreateInfo[]>                    { static constexpr auto func = vkCreateFramebuffer;                 };
    template<> struct create_func<VkRenderPassCreateInfo>                       { static constexpr auto func = vkCreateRenderPass;                  };
    template<> struct create_func<VkRenderPassCreateInfo[]>                     { static constexpr auto func = vkCreateRenderPass;                  };
    template<> struct create_func<VkCommandPoolCreateInfo>                      { static constexpr auto func = vkCreateCommandPool;                 };
    template<> struct create_func<VkCommandPoolCreateInfo[]>                    { static constexpr auto func = vkCreateCommandPool;                 };
    template<> struct create_func<VkSamplerYcbcrConversionCreateInfo>           { static constexpr auto func = vkCreateSamplerYcbcrConversion;      };
    template<> struct create_func<VkSamplerYcbcrConversionCreateInfo[]>         { static constexpr auto func = vkCreateSamplerYcbcrConversion;      };
    template<> struct create_func<VkDescriptorUpdateTemplateCreateInfo>         { static constexpr auto func = vkCreateDescriptorUpdateTemplate;    };
    template<> struct create_func<VkDescriptorUpdateTemplateCreateInfo[]>       { static constexpr auto func = vkCreateDescriptorUpdateTemplate;    };
    template<> struct create_func<VkRenderPassCreateInfo2>                      { static constexpr auto func = vkCreateRenderPass2;                 };
    template<> struct create_func<VkRenderPassCreateInfo2[]>                    { static constexpr auto func = vkCreateRenderPass2;                 };
    template<> struct create_func<VkPrivateDataSlotCreateInfo>                  { static constexpr auto func = vkCreatePrivateDataSlot;             };
    template<> struct create_func<VkPrivateDataSlotCreateInfo[]>                { static constexpr auto func = vkCreatePrivateDataSlot;             };
    template<> struct create_func<VkMemoryAllocateInfo>                         { static constexpr auto func = vkAllocateMemory;                    };
    template<> struct create_func<VkMemoryAllocateInfo[]>                       { static constexpr auto func = vkAllocateMemory;                    };

    // extension handles
    template<> struct create_func<VkDisplaySurfaceCreateInfoKHR>                { static constexpr auto func = vkCreateDisplayPlaneSurfaceKHR;      };
    template<> struct create_func<VkDisplaySurfaceCreateInfoKHR[]>              { static constexpr auto func = vkCreateDisplayPlaneSurfaceKHR;      };
    template<> struct create_func<VkHeadlessSurfaceCreateInfoEXT>               { static constexpr auto func = vkCreateHeadlessSurfaceEXT;          };
    template<> struct create_func<VkHeadlessSurfaceCreateInfoEXT[]>             { static constexpr auto func = vkCreateHeadlessSurfaceEXT;          };
    template<> struct create_func<VkSwapchainCreateInfoKHR>                     { static constexpr auto func = vkCreateSwapchainKHR;                }; // special handling for vkCreateSharedSwapchainsKHR
    template<> struct create_func<VkSwapchainCreateInfoKHR[]>                   { static constexpr auto func = vkCreateSwapchainKHR;                }; // special handling for vkCreateSharedSwapchainsKHR
    template<> struct create_func<VkVideoSessionCreateInfoKHR>                  { static constexpr auto func = vkCreateVideoSessionKHR;             };
    template<> struct create_func<VkVideoSessionCreateInfoKHR[]>                { static constexpr auto func = vkCreateVideoSessionKHR;             };
    template<> struct create_func<VkVideoSessionParametersCreateInfoKHR>        { static constexpr auto func = vkCreateVideoSessionParametersKHR;   };
    template<> struct create_func<VkVideoSessionParametersCreateInfoKHR[]>      { static constexpr auto func = vkCreateVideoSessionParametersKHR;   };
    template<> struct create_func<VkAllocationCallbacks>                        { static constexpr auto func = vkCreateDeferredOperationKHR;        }; // special handling (has no create-info)
    template<> struct create_func<VkAllocationCallbacks[]>                      { static constexpr auto func = vkCreateDeferredOperationKHR;        }; // special handling (has no create-info)
    template<> struct create_func<VkPipelineBinaryCreateInfoKHR>                { static constexpr auto func = vkCreatePipelineBinariesKHR;         };
    template<> struct create_func<VkPipelineBinaryCreateInfoKHR[]>              { static constexpr auto func = vkCreatePipelineBinariesKHR;         };
    template<> struct create_func<VkDebugReportCallbackCreateInfoEXT>           { static constexpr auto func = vkCreateDebugReportCallbackEXT;      };
    template<> struct create_func<VkDebugReportCallbackCreateInfoEXT[]>         { static constexpr auto func = vkCreateDebugReportCallbackEXT;      };
    template<> struct create_func<VkCuModuleCreateInfoNVX>                      { static constexpr auto func = vkCreateCuModuleNVX;                 };
    template<> struct create_func<VkCuModuleCreateInfoNVX[]>                    { static constexpr auto func = vkCreateCuModuleNVX;                 };
    template<> struct create_func<VkCuFunctionCreateInfoNVX>                    { static constexpr auto func = vkCreateCuFunctionNVX;               };
    template<> struct create_func<VkCuFunctionCreateInfoNVX[]>                  { static constexpr auto func = vkCreateCuFunctionNVX;               };
    template<> struct create_func<VkDebugUtilsMessengerCreateInfoEXT>           { static constexpr auto func = vkCreateDebugUtilsMessengerEXT;      };
    template<> struct create_func<VkDebugUtilsMessengerCreateInfoEXT[]>         { static constexpr auto func = vkCreateDebugUtilsMessengerEXT;      };
    template<> struct create_func<VkValidationCacheCreateInfoEXT>               { static constexpr auto func = vkCreateValidationCacheEXT;          };
    template<> struct create_func<VkValidationCacheCreateInfoEXT[]>             { static constexpr auto func = vkCreateValidationCacheEXT;          };
    template<> struct create_func<VkAccelerationStructureCreateInfoNV>          { static constexpr auto func = vkCreateAccelerationStructureNV;     };
    template<> struct create_func<VkAccelerationStructureCreateInfoNV[]>        { static constexpr auto func = vkCreateAccelerationStructureNV;     };
    template<> struct create_func<VkRayTracingPipelineCreateInfoNV>             { static constexpr auto func = vkCreateRayTracingPipelinesNV;       }; // special handling (pipeline)
    template<> struct create_func<VkRayTracingPipelineCreateInfoNV[]>           { static constexpr auto func = vkCreateRayTracingPipelinesNV;       }; // special handling (pipeline)
    template<> struct create_func<VkIndirectCommandsLayoutCreateInfoNV>         { static constexpr auto func = vkCreateIndirectCommandsLayoutNV;    };
    template<> struct create_func<VkIndirectCommandsLayoutCreateInfoNV[]>       { static constexpr auto func = vkCreateIndirectCommandsLayoutNV;    };
    template<> struct create_func<VkMicromapCreateInfoEXT>                      { static constexpr auto func = vkCreateMicromapEXT;                 };
    template<> struct create_func<VkMicromapCreateInfoEXT[]>                    { static constexpr auto func = vkCreateMicromapEXT;                 };
    template<> struct create_func<VkTensorCreateInfoARM>                        { static constexpr auto func = vkCreateTensorARM;                   };
    template<> struct create_func<VkTensorCreateInfoARM[]>                      { static constexpr auto func = vkCreateTensorARM;                   };
    template<> struct create_func<VkTensorViewCreateInfoARM>                    { static constexpr auto func = vkCreateTensorViewARM;               };
    template<> struct create_func<VkTensorViewCreateInfoARM[]>                  { static constexpr auto func = vkCreateTensorViewARM;               };
    template<> struct create_func<VkOpticalFlowSessionCreateInfoNV>             { static constexpr auto func = vkCreateOpticalFlowSessionNV;        };
    template<> struct create_func<VkOpticalFlowSessionCreateInfoNV[]>           { static constexpr auto func = vkCreateOpticalFlowSessionNV;        };
    template<> struct create_func<VkShaderCreateInfoEXT>                        { static constexpr auto func = vkCreateShadersEXT;                  }; // special handling (has count)
    template<> struct create_func<VkShaderCreateInfoEXT[]>                      { static constexpr auto func = vkCreateShadersEXT;                  }; // special handling (has count)
    template<> struct create_func<VkDataGraphPipelineCreateInfoARM>             { static constexpr auto func = vkCreateDataGraphPipelinesARM;       }; // special handling (pipeline)
    template<> struct create_func<VkDataGraphPipelineCreateInfoARM[]>           { static constexpr auto func = vkCreateDataGraphPipelinesARM;       }; // special handling (pipeline)
    template<> struct create_func<VkDataGraphPipelineSessionCreateInfoARM>      { static constexpr auto func = vkCreateDataGraphPipelineSessionARM; };
    template<> struct create_func<VkDataGraphPipelineSessionCreateInfoARM[]>    { static constexpr auto func = vkCreateDataGraphPipelineSessionARM; };
    template<> struct create_func<VkExternalComputeQueueNV>                     { static constexpr auto func = vkCreateExternalComputeQueueNV;      };
    template<> struct create_func<VkExternalComputeQueueNV[]>                   { static constexpr auto func = vkCreateExternalComputeQueueNV;      };
    template<> struct create_func<VkIndirectCommandsLayoutCreateInfoEXT>        { static constexpr auto func = vkCreateIndirectCommandsLayoutEXT;   };
    template<> struct create_func<VkIndirectCommandsLayoutCreateInfoEXT[]>      { static constexpr auto func = vkCreateIndirectCommandsLayoutEXT;   };
    template<> struct create_func<VkIndirectExecutionSetEXT>                    { static constexpr auto func = vkCreateIndirectExecutionSetEXT;     };
    template<> struct create_func<VkIndirectExecutionSetEXT[]>                  { static constexpr auto func = vkCreateIndirectExecutionSetEXT;     };
    template<> struct create_func<VkAccelerationStructureCreateInfoKHR>         { static constexpr auto func = vkCreateAccelerationStructureKHR;    };
    template<> struct create_func<VkAccelerationStructureCreateInfoKHR[]>       { static constexpr auto func = vkCreateAccelerationStructureKHR;    };
    template<> struct create_func<VkRayTracingPipelineCreateInfoKHR>            { static constexpr auto func = vkCreateRayTracingPipelinesKHR;      }; // special handling (pipeline)
    template<> struct create_func<VkRayTracingPipelineCreateInfoKHR[]>          { static constexpr auto func = vkCreateRayTracingPipelinesKHR;      }; // special handling (pipeline)

    template<typename T>
    constexpr auto create_f = create_func<T>::func;
#endif
}
