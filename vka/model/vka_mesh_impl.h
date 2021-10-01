/**
* @file     vka_mesh_impl.h
* @brief    Mesh implemenation file.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "vka_model.h"

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::Mesh<_vc, _tc, _nc>::Mesh(const Mesh& other) { *this = other; }

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::Mesh<_vc, _tc, _nc>& vka::Mesh<_vc, _tc, _nc>::operator= (const Mesh& other)
{
    this->_vertices = other._vertices;
    this->_indices = other._indices;
    this->_materials = other._materials;
    this->_name = other._name;
    return *this;
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::Mesh<_vc, _tc, _nc>::Mesh(Mesh&& other) { *this = (Mesh&&)other; }

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::Mesh<_vc, _tc, _nc>& vka::Mesh<_vc, _tc, _nc>::operator= (Mesh&& other)
{
    this->_vertices = other._vertices;
    this->_indices = other._indices;
    this->_materials = other._materials;
    this->_name = other._name;

    this->clear();
    return *this;
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
void vka::Mesh<_vc, _tc, _nc>::create(const tinyobj::attrib_t& attribute, const tinyobj::shape_t& shape)
{
    this->_name = shape.name;
    this->_materials = shape.mesh.material_ids;

    // Hash map to store only unique vertices and their corresponding index.
    // It's only purpose is to deduplicate vertices (to figure out if a vertex already exists or not).
    // Uses a hash map to have direct access to the vertex as this is faster than using a tree or list map.
    std::unordered_map<vertex_t<_vc, _tc, _nc>, uint32_t, typename vertex_t<_vc, _tc, _nc>::hasher> map;
    for (const tinyobj::index_t index : shape.mesh.indices)
    {
        vertex_t<_vc, _tc, _nc> vertex;
        tinyobj::real_t* pos = (tinyobj::real_t*)&vertex.position;
        tinyobj::real_t* texcoord = (tinyobj::real_t*)&vertex.texcoord;
        tinyobj::real_t* normal = (tinyobj::real_t*)&vertex.normal;

        // read positions
        for (uint32_t i = 0; i < _vc; i++)
        {
            if (attribute.vertices.size() > 0)
                pos[i] = attribute.vertices[_vc * index.vertex_index + i];
        }

        // read texture coordinates
        for (uint32_t i = 0; i < _tc; i++)
        {
            if (attribute.texcoords.size() > 0)
                texcoord[i] = attribute.texcoords[_tc * index.texcoord_index + i];
        }

        // read normal vectors
        for (uint32_t i = 0; i < _nc; i++)
        {
            if (attribute.normals.size() > 0)
                normal[i] = attribute.normals[_nc * index.normal_index + i];
        }

        // only store vertex if it doesnt exist in the hash map
        if (map.count(vertex) == 0)
        {
            // the current size of the _vertices vector is the index of the current UNIQUE vertex
            map[vertex] = static_cast<uint32_t>(_vertices.size());
            // ... also store the UNIQUE vertex
            this->_vertices.push_back(vertex);
        }
        // store index of the current vertex
        this->_indices.push_back(map[vertex]);
    }
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
const std::vector< vka::vertex_t<_vc, _tc, _nc> >& vka::Mesh<_vc, _tc, _nc>::vertices(void) const noexcept 
{ 
    return this->_vertices; 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
const std::vector<uint32_t>& vka::Mesh<_vc, _tc, _nc>::indices(void) const noexcept 
{ 
    return this->_indices; 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
const std::vector<int>& vka::Mesh<_vc, _tc, _nc>::material_ids(void) const noexcept 
{ 
    return this->_materials; 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
const std::string& vka::Mesh<_vc, _tc, _nc>::name(void) const noexcept 
{ 
    return this->_name; 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
const void* vka::Mesh<_vc, _tc, _nc>::vertex_data(void) const noexcept 
{ 
    return this->_vertices.data(); 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
const void* vka::Mesh<_vc, _tc, _nc>::index_data(void) const noexcept 
{ 
    return this->_indices.data(); 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
size_t vka::Mesh<_vc, _tc, _nc>::vertex_size(void) const noexcept 
{ 
    return this->_vertices.size() * sizeof(vertex_t<_vc, _tc, _nc>); 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
size_t vka::Mesh<_vc, _tc, _nc>::index_size(void) const noexcept 
{ 
    return this->_indices.size() * sizeof(uint32_t); 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
void vka::Mesh<_vc, _tc, _nc>::clear(void) noexcept
{
    this->_vertices.clear();
    this->_indices.clear();
    this->_materials.clear();
    this->_name.clear();

    this->_vertices.shrink_to_fit();
    this->_indices.shrink_to_fit();
    this->_materials.shrink_to_fit();
}
