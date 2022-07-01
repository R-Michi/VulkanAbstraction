/**
* @file     vka_mesh_impl.inl
* @brief    Implementation of the Mesh class.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

vka::Mesh::Mesh(const Mesh& src)
{
    *this = src;
}
vka::Mesh& vka::Mesh::operator= (const Mesh& src)
{
    this->_vertices = src._vertices;
    this->_normals = src._normals;
    this->_texcoords = src._texcoords;
    this->_indices = src._indices;
    this->_materials = src._materials;
    return *this;
}

vka::Mesh::Mesh(Mesh&& src)
{
    *this = std::move(src);
}
vka::Mesh& vka::Mesh::operator= (Mesh&& src)
{
    this->_vertices = src._vertices;
    this->_normals = src._normals;
    this->_texcoords = src._texcoords;
    this->_indices = src._indices;
    this->_materials = src._materials;
    src.clear();
    return *this;
}

void vka::Mesh::create(const tinyobj::attrib_t& attribute, const tinyobj::shape_t& shape, const std::vector<uint32_t>& mtlIDs)
{
    this->clear();

    uint32_t cur_idx = 0;
    std::unordered_map<detail::model::vec_t, uint32_t, detail::model::Hasher> vertex_map;
    for(const tinyobj::index_t& idx : shape.mesh.indices)
    {
        detail::model::vec_t tmp(this->total_component_count());

        // get vertex posiion
        tmp[VERTEX_COMPONENT_OFFSET + 0] = attribute.vertices[idx.vertex_index * VERTEX_COMPONENT_COUNT + 0];
        tmp[VERTEX_COMPONENT_OFFSET + 1] = attribute.vertices[idx.vertex_index * VERTEX_COMPONENT_COUNT + 1];
        tmp[VERTEX_COMPONENT_OFFSET + 2] = attribute.vertices[idx.vertex_index * VERTEX_COMPONENT_COUNT + 2];

        // get normal vector
        tmp[NORMAL_COMPONENT_OFFSET + 0] = attribute.normals[idx.normal_index * NORMAL_COMPONENT_COUNT + 0];
        tmp[NORMAL_COMPONENT_OFFSET + 1] = attribute.normals[idx.normal_index * NORMAL_COMPONENT_COUNT + 1];
        tmp[NORMAL_COMPONENT_OFFSET + 2] = attribute.normals[idx.normal_index * NORMAL_COMPONENT_COUNT + 2];

        // get texture coordinates
        tmp[TEXCOORD_COMPONENT_OFFSET + 0] = attribute.texcoords[idx.texcoord_index * TEXCOORD_COMPONENT_COUNT + 0];
        tmp[TEXCOORD_COMPONENT_OFFSET + 1] = attribute.texcoords[idx.texcoord_index * TEXCOORD_COMPONENT_COUNT + 1];

        if(vertex_map.count(tmp) == 0)
        {
            vertex_map[tmp] = cur_idx++;
            this->_vertices.insert(this->_vertices.end(),   tmp.begin() + VERTEX_COMPONENT_OFFSET,      tmp.begin() + (VERTEX_COMPONENT_OFFSET      + VERTEX_COMPONENT_COUNT));
            this->_normals.insert(this->_normals.end(),     tmp.begin() + NORMAL_COMPONENT_OFFSET,      tmp.begin() + (NORMAL_COMPONENT_OFFSET      + NORMAL_COMPONENT_COUNT));
            this->_texcoords.insert(this->_texcoords.end(), tmp.begin() + TEXCOORD_COMPONENT_OFFSET,    tmp.begin() + (TEXCOORD_COMPONENT_OFFSET    + TEXCOORD_COMPONENT_COUNT));
        }

        this->_indices.push_back(vertex_map[tmp]);
    }
    this->_materials = mtlIDs;
}

void vka::Mesh::merge(std::vector<real_t>& data, const std::vector<VertexAttributeType>& attributes) const
{
    const uint32_t vc = this->vertex_count();

    for(uint32_t i = 0; i < vc; i++)
    {
        for(VertexAttributeType a : attributes)
        {
            switch(a)
            {
            case VKA_VERTEX_ATTRIBUTE_TYPE_POSITION:
            {
                const size_t begin  = i * VERTEX_COMPONENT_COUNT;
                const size_t end    = begin + VERTEX_COMPONENT_COUNT;
                data.insert(data.end(), this->_vertices.begin() + begin, this->_vertices.begin() + end);
                break;
            }
            case VKA_VERTEX_ATTRIBUTE_TYPE_NORMAL:
            {
                const size_t begin  = i * NORMAL_COMPONENT_COUNT;
                const size_t end    = begin + NORMAL_COMPONENT_COUNT;
                data.insert(data.end(), this->_normals.begin() + begin, this->_normals.begin() + end);
                break;
            }
            case VKA_VERTEX_ATTRIBUTE_TYPE_TEXTURE_COORDINATE:
            {
                const size_t begin  = i * TEXCOORD_COMPONENT_COUNT;
                const size_t end    = begin + TEXCOORD_COMPONENT_COUNT;
                data.insert(data.end(), this->_texcoords.begin() + begin, this->_texcoords.begin() + end);
                break;
            }
            default:
                break;
            }
        }
    }
}

void vka::Mesh::clear(void) noexcept
{
    this->_vertices.clear();
    this->_normals.clear();
    this->_texcoords.clear();
    this->_indices.clear();
    this->_materials.clear();
}
