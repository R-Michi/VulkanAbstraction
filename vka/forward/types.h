/**
 * @brief Includes types used by vka.
 * @author GitHub: R-Michi
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once

namespace vka
{
#ifdef VKA_MODEL_LOADING_ENABLE
    using real_t = tinyobj::real_t;
#endif

    enum class VertexAttributeType : uint32_t // deprecated
    {
        NONE = 0,
        POSITION = 1,
        NORMAL = 3,
        TEXTURE_COORDINATE = 4,
        POSITION_W_EXT = 100,
        TEXTURE_COORDINATE_W_EXT = 101,
        COLOR_EXT = 102,
    };

    enum class ModelLoadOptionFlagBits : uint32_t // deprecated
    {
        DEFAULT = 0x00000000,
        IGNORE_MATERIAL = 0x00000001,
        FORCE_PER_MESH_MATERIAL = 0x00000002,
    };
    using ModelLoadOptionFlags = uint32_t;

    struct VertexAttribute // deprecated
    {
        VertexAttributeType type;
        size_t              spacing;
    };
}
