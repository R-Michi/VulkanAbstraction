/**
* @file     vka_descriptor.h
* @brief    Descriptor definition file.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../detail/descriptor/vka_descriptor.h"

namespace vka
{
    class DescriptorManager
    {
    private:
        // handles
        VkDescriptorPool _pool;
        std::vector<VkDescriptorSetLayout> set_layouts;
        std::vector<VkDescriptorSet> sets;

        // descriptor structs
        std::vector<VkDescriptorPoolSize> pool_sizes;
        std::vector< std::vector<VkDescriptorSetLayoutBinding> > set_layout_bindings;
        std::vector<VkWriteDescriptorSet> descriptor_writes;

        // requiered
        VkDevice device;

        VkResult init_descriptor_layouts(void);
        VkResult init_descriptor_pool(void);
        VkResult init_descriptor_sets(void);

        void add_to_pool_sizes(VkDescriptorType type, uint32_t count);
        void init_descriptor_write(VkWriteDescriptorSet& write, uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t n_elements);

    public:
        /**
        * @brief device is a VK_NULL_HANDLE per default.
        */
        DescriptorManager(void) noexcept;

        /**
        * @param[in] device logical device
        * @param[in] count  number of descriptor sets to allocate
        */
        explicit DescriptorManager(VkDevice device, uint32_t count);
        virtual ~DescriptorManager(void);

        /**
        * @brief            Set the logical device that is used by the descriptor manager.
        * @param[in] device logical device
        */
        void set_device(VkDevice device) noexcept;

        /**
        * @brief            Set the number of descriptor sets to allocate.
        * @param[in] count  number of descriptor sets
        * @throw            std::runtime_error, if this method is called after 'DescriptorManager::init()'
        */
        void set_descriptor_set_count(uint32_t count);

        /** @return number of allocated descriptor sets */
        uint32_t descriptor_set_count(void) const noexcept;

        /**
        * @brief                Adds a binding to a descriptor set.
        * 
        * @param[in] set        descriptor set ID
        * @param[in] binding    descriptor set binding ID
        * @param[in] type       type of the descriptors within the binding
        * @param[in] count      number of descriptors of the binding
        * @param[in] stage      shader stage where the descriptor binding is present
        * 
        * @throw                std::runtime_error, if this method is called after 'DescriptorManager::init()'
        * @throw                std::out_of_range, if descriptor set ID greather than or equal to the
        *                       allocated descriptor set count
        */
        void add_binding(uint32_t set, uint32_t binding, VkDescriptorType type, uint32_t count, VkShaderStageFlags stage);

        /**
        * @brief                    Sets buffer descriptors.
        * 
        * @param[in] set            descriptor set ID
        * @param[in] binding        descriptor set binding ID
        * @param[in] begin_element  index of the first affected descriptor
        * @param[in] element_count  number of descriptors that are affected
        * @note                     '@param begin_element' + '@param element_count' MUST NOT exeed the descriptor capacity
        *                           of this binding, this will cause validation errors.
        * @param[in] buffer_infos   buffer info structs for every descriptor that is affected
        * 
        * @throw                    std::out_of_range, if descriptor set ID greather than or equal to the
        *                           allocated descriptor set count
        * @throw                    std::runtime_error, if descriptor binding ID of the given could not be found
        */
        void write_buffer_info(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t element_count, const VkDescriptorBufferInfo* buffer_infos);

        /**
        * @brief                    Sets image descriptors.
        *
        * @param[in] set            descriptor set ID
        * @param[in] binding        descriptor set binding ID
        * @param[in] begin_element  index of the first affected descriptor
        * @param[in] element_count  number of descriptors that are affected
        * @note                     '@param begin_element' + '@param element_count' MUST NOT exeed the descriptor capacity
        *                           of this binding, this will cause validation errors.
        * @param[in] buffer_infos   buffer info structs for every descriptor that is affected
        *
        * @throw                    std::out_of_range, if descriptor set ID greather than or equal to the
        *                           allocated descriptor set count
        * @throw                    std::runtime_error, if descriptor binding ID of the given could not be found
        */
        void write_image_info(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t element_count, const VkDescriptorImageInfo* image_infos);

        /**
        * @brief                    Sets texel buffer view descriptors.
        *
        * @param[in] set            descriptor set ID
        * @param[in] binding        descriptor set binding ID
        * @param[in] begin_element  index of the first affected descriptor
        * @param[in] element_count  number of descriptors that are affected
        * @note                     '@param begin_element' + '@param element_count' MUST NOT exeed the descriptor capacity
        *                           of this binding, this will cause validation errors.
        * @param[in] buffer_infos   buffer info structs for every descriptor that is affected
        *
        * @throw                    std::out_of_range, if descriptor set ID greather than or equal to the
        *                           allocated descriptor set count
        * @throw                    std::runtime_error, if descriptor binding ID of the given could not be found
        */
        void write_texel_buffer_view(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t element_count, const VkBufferView* buffer_views);

        void write_acceleration_structure_NV(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t element_count, const VkAccelerationStructureNV* as_nv);

        void write_acceleration_structure_KHR(uint32_t set, uint32_t binding, uint32_t begin_element, uint32_t element_count, const VkAccelerationStructureKHR* as_khr);

        void write_inline_uniform_block_EXT(uint32_t set, uint32_t binding, uint32_t offset, uint32_t size, const void* data);

        /**
        * @brief Initializes the descriptor manager.
        *        After initialization, bindings can no longer be added and the amount of sets
        *        can no longer be canged.
        * 
        * @throw std::runtime_error, if this method is called twice or more
        * @throw std::invalid_argument, if device is a VK_NULL_HANDLE
        */
        VkResult init(void);

        /**
        * @brief Updates descriptor sets.
        * 
        * @throw std::runtime_error, if descriptor manager has not been initialized
        * @throw std::invalid_argument, if device is a VK_NULL_HANDLE
        */
        void update(void);

        /** 
        * @brief Cleares the descriptor manager. 
        * @throw std::invalid_argument, if device is a VK_NULL_HANDLE
        */
        void clear(void);

        /** @return descriptor pool handle */
        VkDescriptorPool pool(void) const noexcept;

        /** @return vector of all descriptor set layout handles */
        const std::vector<VkDescriptorSetLayout>& layouts(void) const noexcept;

        /** @return vector of all descriptor set handles */
        const std::vector<VkDescriptorSet>& descriptor_sets(void) const noexcept;
    };

    /*
     * This is a helper class to create bindings for descriptor sets. Each set of descriptors can have
     * multiple bindings of descriptors. The purpose of this class is to minimize the effort to create
     * bindings for multiple descriptor sets.
     */
    class DescriptorSetBindingList
    {
    private:
        std::vector<std::vector<VkDescriptorSetLayoutBinding>> m_bindings;

    public:
        DescriptorSetBindingList(void) = default;
        ~DescriptorSetBindingList(void) = default;

        /*
         * Adds a binding to the binding-list for a specified descriptor set 'set'. The binding number
         * for every individual set is incremented by 1, and starts at 0, if this function is called.
         * Different descriptor sets have independent bindings and have its own index counter.
         * Therefore, the number of bindings for a given set corresponds to the number of push() calls
         * for this specific set. The number of descriptor sets the binding-list contains corresponds
         * to the highest set-index + 1. 'set' specifies the descriptor set-index which also starts at
         * 0. 'stages' specify the shader stages where the current binding is used. 'type' specifies the
         * descriptor type. 'count' specifies the number of descriptors of the given type. It is set to
         * 1 because it is often required to only have one descriptor per binding. 'immutable_samplers'
         * specifies an array of vulkan sampler handles which is optional and 'nullptr' by default.
         * NOTE: Fore more information see the documentation of VkDescriptorSetLayoutBinding.
         * Additionally, the size of the 'immutable_samplers'-array is not explicitly mentioned in the
         * documentation of VkDescriptorSetLayoutBinding. It might require as many VkSampler handles as
         * the number of descriptors specified for this binding (the value specified by 'count').
         */
        void push(uint32_t set, VkShaderStageFlags stages, VkDescriptorType type, uint32_t count = 1, const VkSampler* immutable_samplers = nullptr);

        /*
         * Returns the number of bindings for the descriptor set index specified by 'set'. If an invalid
         * set is specified, NSIZE32 is returned.
         */
        inline uint32_t binding_count(uint32_t set) const noexcept;

        // Returns the number of descriptor sets.
        inline uint32_t count(void) const noexcept;
    };

    template<typename Info, uint32_t S>
    class DescriptorInfoList
    {
    private:
        Info m_infos[S];
        uint32_t m_idx;

    public:
        inline DescriptorInfoList(void) noexcept;
        ~DescriptorInfoList(void) = default;

        inline void push(Info info) noexcept;

        // Returns the number of buffer infos.
        inline uint32_t count(void) const noexcept;

        // Returns the array of Info handles.
        inline const Info* data(void) const noexcept;
    };

    template<typename Info>
    class DescriptorInfoList<Info, 0>
    {
    private:
        std::vector<Info> m_infos;

    public:
        DescriptorInfoList(void) = default;
        ~DescriptorInfoList(void) = default;

        inline void push(Info info);

        // Returns the number of buffer infos.
        inline uint32_t count(void) const noexcept;

        // Returns the array of Info handles.
        inline const Info* data(void) const noexcept;
    };

    template<uint32_t S>
    class DescriptorInfoList<VkDescriptorBufferInfo, S>
    {
    private:
        VkDescriptorBufferInfo m_infos[S];
        uint32_t m_idx;

    public:
        inline DescriptorInfoList(void) noexcept;
        ~DescriptorInfoList(void) = default;

        /*
         * Adds a buffer info to the list. 'buffer' specifies a vulkan buffer handle. 'offset' specifies
         * an offset in bytes in the buffer that is referenced by the descriptor. 'range' specifies the
         * memory size in bytes that is referenced by the descriptor.  By default, 'offset' is 0 and
         * 'range' is VK_WHOLE_SIZE. This implies that the whole buffer is referenced by the descriptor
         * by default. If the push limit has been exceeded, a std::out_of_range exception is thrown.
         * NOTE: See the documentation of VkDescriptorBufferInfo for more information.
         */
        inline void push(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;

        /*
         * Adds a vka::Buffer object to the list. The VkBuffer handle is implicitly specified by the
         * buffer object. 'offset' specifies an offset in bytes in the buffer that is referenced by
         * the descriptor. 'range' specifies the memory size in bytes that is referenced by the
         * descriptor.  By default, 'offset' is 0 and 'range' is VK_WHOLE_SIZE. If the push limit has
         * been exceeded, a std::out_of_range exception is thrown.
         */
        inline void push(const Buffer& buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;

        // Returns the number of buffer infos.
        inline uint32_t count(void) const noexcept;

        // Returns the array of VkDescriptorBufferInfo structures.
        inline const VkDescriptorBufferInfo* data(void) const noexcept;
    };

    template<>
    class DescriptorInfoList<VkDescriptorBufferInfo, 0>
    {
    private:
        std::vector<VkDescriptorBufferInfo> m_infos;

    public:
        DescriptorInfoList(void) = default;
        ~DescriptorInfoList(void) = default;

        /*
         * Adds a buffer info to the list. 'buffer' specifies a vulkan buffer handle. 'offset' specifies
         * an offset in bytes in the buffer that is referenced by the descriptor. 'range' specifies the
         * memory size in bytes that is referenced by the descriptor.  By default, 'offset' is 0 and
         * 'range' is VK_WHOLE_SIZE. This implies that the whole buffer is referenced by the descriptor
         * by default.
         * NOTE: See the documentation of VkDescriptorBufferInfo for more information.
         */
        inline void push(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);

        /*
         * Adds a vka::Buffer object to the list. The VkBuffer handle is implicitly specified by the
         * buffer object. 'offset' specifies an offset in bytes in the buffer that is referenced by
         * the descriptor. 'range' specifies the memory size in bytes that is referenced by the
         * descriptor.  By default, 'offset' is 0 and 'range' is VK_WHOLE_SIZE.
         */
        inline void push(const Buffer& buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);

        // Returns the number of buffer infos.
        inline uint32_t count(void) const noexcept;

        // Returns the array of VkDescriptorBufferInfo structures.
        inline const VkDescriptorBufferInfo* data(void) const noexcept;
    };

    template<uint32_t S>
    class DescriptorInfoList<VkDescriptorImageInfo, S>
    {
    private:
        VkDescriptorImageInfo m_infos[S];
        uint32_t m_idx;

    public:
        inline DescriptorInfoList(void) noexcept;
        ~DescriptorInfoList(void) = default;

        /*
         * Adds an image info to the list. 'view' specifies the vulkan image view handle. 'layout'
         * specifies the layout for the image. 'sampler' specifies a sampler handle associated with
         * the image view. By default, 'sampler' is set to VK_NULL_HANDLE. If the push limit has been
         * exceeded, a std::out_of_range exception is thrown.
         * NOTE: See the documentation of VkDescriptorImageInfo for more information.
         */
        inline void push(VkImageView view, VkImageLayout layout, VkSampler sampler = VK_NULL_HANDLE) noexcept;

        /*
         * Adds a vka::Texture object to the list. The image info is implicitly specified by the
         * texture object. As the texture can contain multiple image views, the index of the used
         * image view is specified by 'view_index'. Therefore, the sampler, layout and the actual
         * image view handle are evaluated automatically. If the view index is invalid a
         * std::out_of_range exception is thrown.
         */
        inline void push(const Texture& texture, uint32_t view_index);

        // Returns the number of buffer infos.
        inline uint32_t count(void) const noexcept;

        // Returns the array of VkDescriptorBufferInfo structures.
        inline const VkDescriptorImageInfo* data(void) const noexcept;
    };

    template<>
    class DescriptorInfoList<VkDescriptorImageInfo, 0>
    {
    private:
        std::vector<VkDescriptorImageInfo> m_infos;

    public:
        DescriptorInfoList(void) = default;
        ~DescriptorInfoList(void) = default;

        /*
         * Adds a buffer info to the list. 'buffer' specifies a vulkan buffer handle. 'offset' specifies
         * an offset in bytes in the buffer that is referenced by the descriptor. 'range' specifies the
         * memory size in bytes that is referenced by the descriptor.  By default, 'offset' is 0 and
         * 'range' is VK_WHOLE_SIZE. This implies that the whole buffer is referenced by the descriptor
         * by default.
         * NOTE: See the documentation of VkDescriptorBufferInfo for more information.
         */
        inline void push(VkImageView view, VkImageLayout layout, VkSampler sampler = VK_NULL_HANDLE);

        /*
         * Adds a vka::Buffer object to the list. The VkBuffer handle is implicitly specified by the
         * buffer object. 'offset' specifies an offset in bytes in the buffer that is referenced by
         * the descriptor. 'range' specifies the memory size in bytes that is referenced by the
         * descriptor.  By default, 'offset' is 0 and 'range' is VK_WHOLE_SIZE.
         */
        inline void push(const Texture& texture, uint32_t view_index);

        // Returns the number of buffer infos.
        inline uint32_t count(void) const noexcept;

        // Returns the array of VkDescriptorBufferInfo structures.
        inline const VkDescriptorImageInfo* data(void) const noexcept;
    };

    template<>
    class DescriptorInfoList<const void*, 0>
    {
    private:
        const void* m_data;
        uint32_t m_size;

    public:
        inline explicit DescriptorInfoList(const void* data = nullptr, uint32_t size = 0) noexcept;
        ~DescriptorInfoList(void) = default;

        inline void set(const void* data, uint32_t size) noexcept;

        // Returns the size of the inline uniform block.
        inline uint32_t count(void) const noexcept;

        // Returns the data of the inline uniform block.
        inline const void* data(void) const noexcept;
    };

    template<uint32_t S>
    class DescriptorWriteList
    {
        template<typename Info, uint32_t S2>
        using DescriptorInfoList = DescriptorInfoList<Info, S2>;
        using ExtendedWriteSet = detail::descriptor::ExtendedWriteSet;

        enum class InfoID
        {
            NONE,
            BUFFER,
            IMAGE,
            BUFFER_VIEW,
            ASNV,
            ASKHR,
            INLINE_UB
        };

        template<typename Info>
        struct infoID_of { static_assert("[DescriptorWriteList::infoID_of]: Invalid info-type."); };
        template<> struct infoID_of<VkDescriptorBufferInfo>                         { constexpr static InfoID ID = InfoID::BUFFER;      };
        template<> struct infoID_of<VkDescriptorImageInfo>                          { constexpr static InfoID ID = InfoID::IMAGE;       };
        template<> struct infoID_of<VkBufferView>                                   { constexpr static InfoID ID = InfoID::BUFFER_VIEW; };
        template<> struct infoID_of<VkWriteDescriptorSetAccelerationStructureNV>    { constexpr static InfoID ID = InfoID::ASNV;        };
        template<> struct infoID_of<VkWriteDescriptorSetAccelerationStructureKHR>   { constexpr static InfoID ID = InfoID::ASKHR;       };
        template<> struct infoID_of<const void*>                                    { constexpr static InfoID ID = InfoID::INLINE_UB;   };

    private:
        VkWriteDescriptorSet m_writes[S];
        ExtendedWriteSet m_writes_ext[S];
        uint32_t m_idx;

    public:
        inline DescriptorWriteList(void) noexcept;
        ~DescriptorWriteList(void) = default;

        /*
         * Adds any available descriptor-info-list to the descriptor-write-list. 'set' specifies the
         * target descriptor set of the write operation. 'binding' specifies the binding of the target
         * descriptor set. 'offset' specifies the index of the first descriptor that is affected by
         * the write operation. 'list' specifies the descriptor-info-list that should be added to the
         * write-list. 'list' also specifies the number of affected descriptors which is the number of
         * written descriptor infos and corresponds to the function 'DescriptorInfoList::count()'.
         * If the write limit has been exceeded, a std::out_of_range exception is thrown.
         */
        template<typename Info, uint32_t S2>
        inline void write(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, const DescriptorInfoList<Info, S2>& list) noexcept;
    };

    /*
     * This is a special implementation of DescriptorWriteList which uses a dynamic number of
     * descriptor writes. The list has no fixed size and can grow at runtime.
     * NOTE: The template argument 'S = 0' is chosen because it would be an invalid input for 'S'.
     */
    template<>
    class DescriptorWriteList<0>
    {
        template<typename Info, uint32_t S2>
        using DescriptorInfoList = DescriptorInfoList<Info, S2>;
        using ExtendedWriteSet = detail::descriptor::ExtendedWriteSet;

    private:
        std::vector<VkWriteDescriptorSet> m_writes;
        std::vector<ExtendedWriteSet> m_writes_ext;

    public:
        DescriptorWriteList(void) = default;
        ~DescriptorWriteList(void) = default;

        /*
         * Adds any available descriptor-info-list to the descriptor-write-list. 'set' specifies the
         * target descriptor set of the write operation. 'binding' specifies the binding of the target
         * descriptor set. 'offset' specifies the index of the first descriptor that is affected by
         * the write operation. 'list' specifies the descriptor-info-list that should be added to the
         * write-list. 'list' also specifies the number of affected descriptors which is the number of
         * written descriptor infos and corresponds to the function 'DescriptorInfoList::count()'.
         */
        template<typename Info, uint32_t S2>
        inline void write(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, const DescriptorInfoList<Info, S2>& list);
    };

#if 0
    /*
     * This is a helper class to specify descriptor buffer infos. The number of buffer infos must be
     * known at compile-time and is specified by the template argument 'S'. 'S' is also the referred
     * to "the push limit" in function descriptions.
     */
    template<uint32_t S>
    class BufferInfoList : public detail::descriptor::DescriptorInfoList<VkDescriptorBufferInfo, S>
    {
    public:
        BufferInfoList(void) = default; // calls the default constructor of DescriptorInfoList
        ~BufferInfoList(void) = default;

        /*
         * Adds a buffer info to the list. 'buffer' specifies a vulkan buffer handle. 'offset' specifies
         * an offset in bytes in the buffer that is referenced by the descriptor. 'range' specifies the
         * memory size in bytes that is referenced by the descriptor.  By default, 'offset' is 0 and
         * 'range' is VK_WHOLE_SIZE. This implies that the whole buffer is referenced by the descriptor
         * by default. If the push limit has been exceeded, a std::out_of_range exception is thrown.
         * NOTE: See the documentation of VkDescriptorBufferInfo for more information.
         */
        inline void push(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);

        /*
         * Adds a vka::Buffer object to the list. The VkBuffer handle is implicitly specified by the
         * buffer object. 'offset' specifies an offset in bytes in the buffer that is referenced by
         * the descriptor. 'range' specifies the memory size in bytes that is referenced by the
         * descriptor.  By default, 'offset' is 0 and 'range' is VK_WHOLE_SIZE. If the push limit has
         * been exceeded, a std::out_of_range exception is thrown.
         */
        inline void push(const Buffer& buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);

        /*
         * These functions have the same functionality as 'push'. However, they no not throw an
         * exception, if the push limit has been exceeded. Instead, those functions do nothing in
         * that particular case.
         */
        inline void push2(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;
        inline void push2(const Buffer& buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE) noexcept;
    };

    /*
     * This is a special implementation of BufferInfoList which uses a dynamic number of descriptor
     * buffer infos. The list has no fixed size and can grow at runtime.
     * NOTE: The template argument 'S = 0' is chosen because it would be an invalid input for 'S'.
     */
    template<>
    class BufferInfoList<0> : public detail::descriptor::DescriptorInfoList<VkDescriptorBufferInfo, 0>
    {
    public:
        BufferInfoList(void) = default;
        ~BufferInfoList(void) = default;

        /*
         * Adds a buffer info to the list. 'buffer' specifies a vulkan buffer handle. 'offset' specifies
         * an offset in bytes in the buffer that is referenced by the descriptor. 'range' specifies the
         * memory size in bytes that is referenced by the descriptor.  By default, 'offset' is 0 and
         * 'range' is VK_WHOLE_SIZE. This implies that the whole buffer is referenced by the descriptor
         * by default.
         * NOTE: See the documentation of VkDescriptorBufferInfo for more information.
         */
        inline void push(VkBuffer buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);

        /*
         * Adds a vka::Buffer object to the list. The VkBuffer handle is implicitly specified by the
         * buffer object. 'offset' specifies an offset in bytes in the buffer that is referenced by
         * the descriptor. 'range' specifies the memory size in bytes that is referenced by the
         * descriptor.  By default, 'offset' is 0 and 'range' is VK_WHOLE_SIZE.
         */
        inline void push(const Buffer& buffer, VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE);
    };

    /*
     * This is a helper class to specify descriptor image infos. The number of image infos must be
     * known at compile-time and is specified by the template argument 'S'. 'S' is also the referred
     * to "the push limit" in function descriptions.
     */
    template<uint32_t S>
    class ImageInfoList : public detail::descriptor::DescriptorInfoList<VkDescriptorImageInfo, S>
    {
    public:
        ImageInfoList(void) = default; // calls the default constructor of DescriptorInfoList
        ~ImageInfoList(void) = default;

        /*
         * Adds an image info to the list. 'view' specifies the vulkan image view handle. 'layout'
         * specifies the layout for the image. 'sampler' specifies a sampler handle associated with
         * the image view. By default, 'sampler' is set to VK_NULL_HANDLE. If the push limit has been
         * exceeded, a std::out_of_range exception is thrown.
         * NOTE: See the documentation of VkDescriptorImageInfo for more information.
         */
        inline void push(VkImageView view, VkImageLayout layout, VkSampler sampler = VK_NULL_HANDLE);

        /*
         * Adds a vka::Texture object to the list. The image info is implicitly specified by the
         * texture object. As the texture can contain multiple image views, the index of the used
         * image view is specified by 'view_index'. Therefore, the sampler, layout and the actual
         * image view handle are evaluated automatically. If the view index is invalid a
         * std::out_of_range exception is thrown.
         */
        inline void push(const Texture& texture, uint32_t view_index);

        /*
         * These functions have the same functionality as 'push'. However, they do not throw an
         * exception, if the push limit has been exceeded. Instead, those functions do nothing in
         * that particular case.
         */
        inline void push2(VkImageView view, VkImageLayout layout, VkSampler sampler = VK_NULL_HANDLE) noexcept;

        // This function does also not throw an exception, if the view index is invalid.
        inline void push2(const Texture& texture, uint32_t view_index) noexcept;
    };

    /*
     * This is a special implementation of ImageInfoList which uses a dynamic number of descriptor
     * image infos. The list has no fixed size and can grow at runtime.
     * NOTE: The template argument 'S = 0' is chosen because it would be an invalid input for 'S'.
     */
    template<>
    class ImageInfoList<0> : public detail::descriptor::DescriptorInfoList<VkDescriptorImageInfo, 0>
    {
    public:
        ImageInfoList(void) = default;
        ~ImageInfoList(void) = default;

        /*
         * Adds an image info to the list. 'view' specifies the vulkan image view handle. 'layout'
         * specifies the layout for the image. 'sampler' specifies a sampler handle associated with
         * the image view. By default, 'sampler' is set to VK_NULL_HANDLE.
         * NOTE: See the documentation of VkDescriptorImageInfo for more information.
         */
        inline void push(VkImageView view, VkImageLayout layout, VkSampler sampler = VK_NULL_HANDLE);

        /*
         * Adds a vka::Texture object to the list. The image info is implicitly specified by the
         * texture object. As the texture can contain multiple image views, the index of the used
         * image view is specified by 'view_index'. Therefore, the sampler, layout and the actual
         * image view handle are evaluated automatically. If the view index is invalid a
         * std::out_of_range exception is thrown.
         */
        inline void push(const Texture& texture, uint32_t view_index);
    };

    /*
     * This is a helper class to specify descriptor acceleration structure (NV) infos. It also contains
     * the extension structure to the VkWriteDescriptorSet structure. The number of image infos must be
     * known at compile-time and is specified by the template argument 'S'. 'S' is also the referred to
     * "the push limit" in function descriptions.
     */
    template<uint32_t S>
    class AccelerationStructureInfoListNV : public detail::descriptor::DescriptorInfoList<VkAccelerationStructureNV, S>
    {
    public:
        AccelerationStructureInfoListNV(void) = default;
        ~AccelerationStructureInfoListNV(void) = default;

        /*
         * Adds an acceleration structure (NV) to the list. 'as' specifies the acceleration structure.
         * If the push limit has been exceeded, a std::out_of_range exception is thrown.
         */
        inline void push(VkAccelerationStructureNV as);

        /*
         * This function has the same functionality as 'push'. However, it does not throw an exception,
         * if the push limit has been exceeded. Instead, this function does nothing in that particular
         * case.
         */
        inline void push2(VkAccelerationStructureNV as) noexcept;
    };

    /*
     * This is a special implementation of AccelerationStructureInfoListNV which uses a dynamic number
     * of acceleration structures (NV). The list has no fixed size and can grow at runtime.
     * NOTE: The template argument 'S = 0' is chosen because it would be an invalid input for 'S'.
     */
    template<>
    class AccelerationStructureInfoListNV<0> : public detail::descriptor::DescriptorInfoList<VkAccelerationStructureNV, 0>
    {
    public:
        AccelerationStructureInfoListNV(void) = default;
        ~AccelerationStructureInfoListNV(void) = default;

        // Adds an acceleration structure (NV) to the list. 'as' specifies the acceleration structure.
        inline void push(VkAccelerationStructureNV as);
    };

    /*
     * This is a helper class to specify descriptor acceleration structure (KHR) infos. It also contains
     * the extension structure to the VkWriteDescriptorSet structure. The number of image infos must be
     * known at compile-time and is specified by the template argument 'S'. 'S' is also the referred to
     * "the push limit" in function descriptions.
     */
    template<uint32_t S>
    class AccelerationStructureInfoListKHR : public detail::descriptor::DescriptorInfoList<VkAccelerationStructureKHR, S>
    {
    private:
        VkWriteDescriptorSetAccelerationStructureKHR m_write; // extension to VkWriteDescriptorSet

    public:
        AccelerationStructureInfoListKHR(void) = default;
        ~AccelerationStructureInfoListKHR(void) = default;

        /*
         * Adds an acceleration structure (KHR) to the list. 'as' specifies the acceleration structure.
         * If the push limit has been exceeded, a std::out_of_range exception is thrown.
         */
        inline void push(VkAccelerationStructureKHR as);

        /*
         * This function has the same functionality as 'push'. However, it does not throw an exception,
         * if the push limit has been exceeded. Instead, this function does nothing in that particular
         * case.
         */
        inline void push2(VkAccelerationStructureKHR as) noexcept;
    };

    /*
     * This is a special implementation of AccelerationStructureInfoListKHR which uses a dynamic number
     * of acceleration structures (KHR). The list has no fixed size and can grow at runtime.
     * NOTE: The template argument 'S = 0' is chosen because it would be an invalid input for 'S'.
     */
    template<>
    class AccelerationStructureInfoListKHR<0> : public detail::descriptor::DescriptorInfoList<VkAccelerationStructureKHR, 0>
    {
    public:
        AccelerationStructureInfoListKHR(void) = default;
        ~AccelerationStructureInfoListKHR(void) = default;

        // Adds an acceleration structure (KHR) to the list. 'as' specifies the acceleration structure.
        inline void push(VkAccelerationStructureKHR as);
    };

    class InlineUniformBlockInfo : public detail::descriptor::DescriptorInfoList<const void*, 0>
    {
    public:
        InlineUniformBlockInfo(void) = default;
        ~InlineUniformBlockInfo(void) = default;

        // Sets the inline uniform block data.
        inline void set(const void* data) noexcept;
    };

    /*
     * This is a helper class to specify descriptor writes. The number of descriptor writes must be
     * known at compile-time and is specified by the template argument 'S'. 'S' is also the referred
     * to "the write limit" in function descriptions.
     */
    template<uint32_t S>
    class DescriptorWriteList
    {
        template<typename Info, uint32_t S2>
        using DescriptorInfoList = detail::descriptor::DescriptorInfoList<Info, S2>;
        using ExtendedWriteSet = detail::descriptor::ExtendedWriteSet;

    private:
        VkWriteDescriptorSet m_writes[S];
        ExtendedWriteSet m_writes_ext[S];
        uint32_t m_idx;

        template<typename Info, uint32_t S2>
        inline void init_write(
            VkDescriptorSet                     set,
            uint32_t                            binding,
            uint32_t                            offset,
            uint32_t                            count,
            VkDescriptorType                    type,
            const DescriptorInfoList<Info, S2>& list
        );

    public:
        constexpr DescriptorWriteList(void) noexcept;
        ~DescriptorWriteList(void) = default;

        /*
         * Adds any available descriptor-info-list to the descriptor-write-list. 'set' specifies the
         * target descriptor set of the write operation. 'binding' specifies the binding of the target
         * descriptor set. 'offset' specifies the index of the first descriptor that is affected by
         * the write operation. 'list' specifies the descriptor-info-list that should be added to the
         * write-list. 'list' also specifies the number of affected descriptors which is the number of
         * written descriptor infos and corresponds to the function 'DescriptorInfoList::count()'.
         * If the write limit has been exceeded, a std::out_of_range exception is thrown.
         */
        template<typename Info, uint32_t S2>
        inline void write(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, const DescriptorInfoList<Info, S2>& list);

        /*
         * This function has the same functionality as 'write()'. However, the number of affected
         * descriptors is the maximum number that 'list' can store and corresponds to the function
         * 'DescriptorInfoList::size()'.
         */
        template<typename Info, uint32_t S2>
        inline void write_const(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, const DescriptorInfoList<Info, S2>& list);

        /*
         * These functions have the same functionality as 'write' or 'write_const' respectively.
         * However, they do not throw an exception, if the write limit has been exceeded. Instead,
         * those functions do nothing in that particular case.
         */
        template<typename Info, uint32_t S2>
        inline void write2(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, const DescriptorInfoList<Info, S2>& list) noexcept;

        template<typename Info, uint32_t S2>
        inline void write2_const(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, const DescriptorInfoList<Info, S2>& list) noexcept;

    };

    /*
     * This is a special implementation of DescriptorWriteList which uses a dynamic number of
     * descriptor writes. The list has no fixed size and can grow at runtime.
     * NOTE: The template argument 'S = 0' is chosen because it would be an invalid input for 'S'.
     */
    template<>
    class DescriptorWriteList<0>
    {
        template<typename Info, uint32_t S2>
        using DescriptorInfoList = detail::descriptor::DescriptorInfoList<Info, S2>;
        using ExtendedWriteSet = detail::descriptor::ExtendedWriteSet;

    private:
        std::vector<VkWriteDescriptorSet> m_writes;
        std::vector<ExtendedWriteSet> m_writes_ext;

    public:
        DescriptorWriteList(void) = default;
        ~DescriptorWriteList(void) = default;

        /*
         * Adds any available descriptor-info-list to the descriptor-write-list. 'set' specifies the
         * target descriptor set of the write operation. 'binding' specifies the binding of the target
         * descriptor set. 'offset' specifies the index of the first descriptor that is affected by
         * the write operation. 'list' specifies the descriptor-info-list that should be added to the
         * write-list. 'list' also specifies the number of affected descriptors which is the number of
         * written descriptor infos and corresponds to the function 'DescriptorInfoList::count()'.
         */
        template<typename Info, uint32_t S2>
        inline void write(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, const DescriptorInfoList<Info, S2>& list);

        /*
         * This function has the same functionality as 'write()'. However, the number of affected
         * descriptors is the maximum number that 'list' can store and corresponds to the function
         * 'DescriptorInfoList::size()'.
         */
        template<typename Info, uint32_t S2>
        inline void write_const(VkDescriptorSet set, uint32_t binding, uint32_t offset, VkDescriptorType type, const DescriptorInfoList<Info, S2>& list);
    };

    class DescriptorManagerLayout
    {
    private:
        VkDevice m_device;
        std::vector<VkDescriptorSetLayout> m_layouts;

    public:
        // Default initialization, every member contains its default initialization.
        inline DescriptorManagerLayout(void) noexcept;

        /*
         * This constructor creates the descriptor-manager-layout and the internal handles are now
         * valid, if no error occured. If an error occured while creating, a std::runtime_error
         * exception is thrown with an appropriate message about the error. The descriptor-manager-
         * layout is created with a vulkan device handle which is specified by 'device' and a
         * descriptor set binding list which is specified by 'set_bindings'. This constructor has
         * the same functionality as the function 'create()'.
         */
        explicit DescriptorManagerLayout(VkDevice device, const DescriptorSetBindingList& set_bindings);

        // The DescriptorManagerLayout must be unique.
        DescriptorManagerLayout(const DescriptorManagerLayout&) = delete;
        DescriptorManagerLayout& operator= (const DescriptorManagerLayout&) = delete;

        /*
         * Moves another object of DescriptorManagerLayout into 'this'. 'this' now holds the ownership
         * of all the handles of the source object and the source object becomes invalidated. If 'this'
         * was created and is a valid object, 'this' is destroyed and replaced by the handles of the
         * moved object. The destructor on moved objects will not have any effect, unless it is
         * recreated.
         */
        DescriptorManagerLayout(DescriptorManagerLayout&& src) noexcept;
        DescriptorManagerLayout& operator= (DescriptorManagerLayout&& src) noexcept;

        // The destructor destroys all the vulkan handles.
        virtual ~DescriptorManagerLayout(void);

        /*
         * This function creates the descriptor-manager-layout and the internal handles are now valid,
         * if no error occured. If an error occured while creating, a std::runtime_error exception is
         * thrown with an appropriate message about the error. The descriptor-manager-layout is created
         * with a vulkan device handle which is specified by 'device' and a descriptor set binding list
         * which is specified by 'set_bindings'.
         */
        void create(const VkDevice device, const DescriptorSetBindingList& set_bindings);

        /*
         * Destroys all the internal vulkan handles and sets them to VK_NULL_HANDLE except for parent
         * handles. After destroying, the object is reset to default state.
         */
        void destroy(void) noexcept;

        // Returns the number of descriptor set layouts.
        inline uint32_t count(void) const noexcept;

        /*
         * Returns the descriptor set layout at the specified index 'idx'. If the index is greater
         * than or equal to the number of owned descriptor set layouts, a std::out_of_range exception
         * is thrown.
         */
        inline VkDescriptorSetLayout handle(size_t idx) const;

        /*
         * Returns the descriptor set layout at the specified index 'idx'. This function does not
         * perform a range check.
         */
        inline VkDescriptorSetLayout operator[] (size_t idx) const noexcept;

        // Returns true, if the DescriptorManagerLayout is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;
    };

    class DescriptorManager2
    {
    private:
        VkDevice m_device;
        VkDescriptorPool m_pool;
        std::vector<VkDescriptorSet> m_sets;

    public:
        // Default initialization, every member contains its default initialization.
        inline DescriptorManager2(void) noexcept;

        /*
         * This constructor creates the descriptor manager and the internal handles are now valid, if
         * no error occured. If an error occured while creating, a std::runtime_error exception is
         * thrown with an appropriate message about the error. The descriptor manager requires a device
         * and a descriptor pool to be created which are specifies by 'device' and 'pool' respectively.
         * 'layout' specifies the layouts that are used to create the descriptor sets with.
         *  This constructor has the same functionality as the function 'create()'.
         */
        explicit DescriptorManager2(VkDevice device, VkDescriptorPool pool, const DescriptorManagerLayout& layout);

        // The DescriptorManager must be unique.
        DescriptorManager2(const DescriptorManager2&) = delete;
        DescriptorManager2& operator= (const DescriptorManager2&) = delete;

        /*
         * Moves another object of DescriptorManager into 'this'. 'this' now holds the ownership of
         * all the handles of the source object and the source object becomes invalidated. If 'this'
         * was created and is a valid object, 'this' is destroyed and replaced by the handles of the
         * moved object. The destructor on moved objects will not have any effect, unless it is
         * recreated.
         */
        DescriptorManager2(DescriptorManager2&& src) noexcept;
        DescriptorManager2& operator= (DescriptorManager2&& src) noexcept;

        // The destructor destroys all the vulkan handles.
        virtual ~DescriptorManager2(void);

        /*
         * This constructor creates the descriptor manager and the internal handles are now valid, if
         * no error occured. If an error occured while creating, a std::runtime_error exception is
         * thrown with an appropriate message about the error. The descriptor manager requires a device
         * and a descriptor pool to be created which are specifies by 'device' and 'pool' respectively.
         * 'layout' specifies the layouts that are used to create the descriptor sets with.
         */
        void create(VkDevice device, VkDescriptorPool pool, const DescriptorManagerLayout& layout);

        /*
         * Destroys all the internal vulkan handles and sets them to VK_NULL_HANDLE except for parent
         * handles. After destroying, the object is reset to default state.
         */
        void destroy(void) noexcept;

        /*
         * Updates the owned descriptor sets. The descriptor sets are only updated, if the
         * DescriptorManager is a valid object. Otherwise, this function does nothing. The update/write
         * data is given by a DescriptorWriteList and is specified by 'write_list'. 'write_list' also
         * specifies the number of descriptor set writes. The number of descriptor writes corresponds
         * to the function 'DescriptorWriteList::count()'.
         */
        template<uint32_t S>
        void update(const DescriptorWriteList<S>& list) noexcept;

        /*
         * This function has the same functionality as 'update()'. However, the number of descriptor
         * set writes is the maximum number that 'write_list' can store and corresponds to the function
         * 'DescriptorWriteList::size()'.
         */
        template<uint32_t S>
        void update_const(const DescriptorWriteList<S>& list) noexcept;

        // Returns the number of descriptor sets.
        inline uint32_t count(void) const noexcept;

        /*
         * Returns the descriptor set at the specified index 'idx'. If the index is greater than or
         * equal to the number of owned descriptor sets, a std::out_of_range exception is thrown.
         */
        inline VkDescriptorSet handle(size_t idx) const;

        /*
         * Returns the descriptor set at the specified index 'idx'. This function does not perform a
         * range check.
         */
        inline VkDescriptorSet operator[] (size_t idx) const noexcept;

        // Returns true, if the DescriptorManager is a valid object and false otherwise.
        inline bool is_valid(void) const noexcept;
    };
#endif
}

#ifdef VKA_IMPLEMENTATION
    #include "vka_descriptor_manager_impl.inl"
#endif
#include "vka_descriptor_inline_impl.inl"
