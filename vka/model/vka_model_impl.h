/**
* @file     vka_model_impl.h
* @brief    Model implementation file.
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
vka::Model<_vc, _tc, _nc>::Model(const Model& other) 
{ 
    *this = other; 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::Model<_vc, _tc, _nc>& vka::Model<_vc, _tc, _nc>::operator= (const Model& other)
{
    this->_meshes = other._meshes;
    this->_materials = other._materials;
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::Model<_vc, _tc, _nc>::Model(Model&& other) 
{ 
    *this = (Model&&)other; 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::Model<_vc, _tc, _nc>& vka::Model<_vc, _tc, _nc>::operator= (Model&& other)
{
    this->_meshes = other._meshes;
    this->_materials = other._materials;
    this->clear();
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
void vka::Model<_vc, _tc, _nc>::load(const std::string& path)
{
    tinyobj::ObjReaderConfig config = {};
    config.vertex_color = false;
    config.triangulate = true;	// always triangulate faces

    tinyobj::ObjReader obj_file;
    if (!obj_file.ParseFromFile(path, config))
        throw std::invalid_argument("Failed to load object file \"" + path + "\".");

    for (const tinyobj::shape_t& shape : obj_file.GetShapes())
    {
        Mesh<_vc, _tc, _nc> mesh;
        mesh.create(obj_file.GetAttrib(), shape);
        this->_meshes.push_back(mesh);
    }
    this->_materials = obj_file.GetMaterials();
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
void vka::Model<_vc, _tc, _nc>::merge(std::vector< vertex_t<_vc, _tc, _nc> >& vertices, std::vector<uint32_t>& indices)
{
    vertices.clear();
    indices.clear();

    uint32_t base_index = 0;
    for (const Mesh<_vc, _tc, _nc>& cur_mesh : this->_meshes)
    {
        for (const vertex_t<_vc, _tc, _nc>& v : cur_mesh.vertices())
            vertices.push_back(v);
        for (uint32_t i : cur_mesh.indices())
            indices.push_back(base_index + i);
        base_index += cur_mesh.vertices().size();
    }
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
const std::vector< vka::Mesh<_vc, _tc, _nc> >& vka::Model<_vc, _tc, _nc>::meshes(void) const noexcept 
{ 
    return this->_meshes; 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
const std::vector<tinyobj::material_t> vka::Model<_vc, _tc, _nc>::materials(void) const noexcept 
{ 
    return this->_materials; 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
void vka::Model<_vc, _tc, _nc>::clear(void) noexcept
{
    this->_meshes.clear();
    this->_materials.clear();
    this->_meshes.shrink_to_fit();
    this->_materials.shrink_to_fit();
}
