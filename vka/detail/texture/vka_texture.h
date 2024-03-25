/**
* @file     vka_texture.h
* @brief    Defines helper classes to determine the datatype for an image loading operation
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/


#ifdef VKA_STB_IMAGE_LOAD_ENABLE
namespace vka::detail
{
    enum class ImageTypeID
    {
        NONE,
        UINT8,
        UINT16,
        FLOAT
    };

    template<typename T>
    struct get_image_type_ID { static constexpr ImageTypeID type = ImageTypeID::NONE; };
    template<> struct get_image_type_ID<stbi_uc> { static constexpr ImageTypeID type = ImageTypeID::UINT8; };
    template<> struct get_image_type_ID<stbi_us> { static constexpr ImageTypeID type = ImageTypeID::UINT16; };
    template<> struct get_image_type_ID<float> { static constexpr ImageTypeID type = ImageTypeID::FLOAT; };
}
#endif
