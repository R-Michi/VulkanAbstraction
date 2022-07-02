/**
* @file     vka_model_impl.inl
* @brief    Implementation of the Model class.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

vka::Model::Model(const Model& src) : Model()
{
    *this = src;
}
vka::Model& vka::Model::operator= (const Model& src)
{
    this->_meshes = src._meshes;
    this->_materials = src._materials;
    return *this;
}

vka::Model::Model(Model&& src) : Model()
{
    *this = std::move(src);
}
vka::Model& vka::Model::operator= (Model&& src)
{
    this->_meshes = src._meshes;
    this->_materials = src._materials;
    src.clear();
    return *this;
}

void vka::Model::load(const std::string& path, ModelLoadOptionFlags flags)
{
    this->clear();

    tinyobj::ObjReaderConfig cfg = {};
    cfg.triangulate = true;
    cfg.vertex_color = false;
    cfg.mtl_search_path = "";

    tinyobj::ObjReader reader;
    reader.ParseFromFile(path, cfg);
    tinyobj::attrib_t attribute = reader.GetAttrib();

    if(!reader.Valid())
        throw std::runtime_error("[vka::Model::load]: Failed to load model.");

    std::map<uint32_t, uint32_t> mtlID_lookup;

    // create all meshes
    for(const tinyobj::shape_t& shape : reader.GetShapes())
    {
        std::vector<uint32_t> mtlIDs;
        // if VKA_MODEL_LOAD_OPTION_IGNORE_MATERIAL is set, ignore material loading
        if(!this->isset(flags, VKA_MODEL_LOAD_OPTION_IGNORE_MATERIAL))
        {
            if(this->isset(flags, VKA_MODEL_LOAD_OPTION_FORCE_PER_MESH_MATERIAL))
            {
                // Materials are forced to be loaded per mesh.
                // As materials are loaded per face in the tinyobj loader library,
                // we have to manage those material ID's by ourselves.
                // There can still be the case that multiple materials per
                // mesh exist, but we don't want to store them, so we have to sort them out.
                // Moreover, we don't want to store the same material multiple times.
                // Therefore every occured material is mapped to our own material index
                // that is counted per mesh instead of per face. If it is not contained within
                // our map, there was a new material found and so it will get stored into the map.
                // The actual material structure that is stored per mesh, is the first
                // material offered by the tinyobj loader library. If the material is already contained
                // within our map, only its per-mesh ID is stored in the material ID array.
                // With that algorithm all unused material ID's are sorted out, all material
                // structures are unique and only ID's referencing those material structures
                // are stored multiple times.
                const uint32_t mesh_mtlID = shape.mesh.material_ids.front();
                if(mesh_mtlID == VKA_MTL_ID_INVALID)
                    throw std::runtime_error("[vka::Model::load]: If VKA_MODEL_LOAD_OPTION_IGNORE_MATERIAL is not set, all meshes must at least have one valid material.");

                if(mtlID_lookup.count(mesh_mtlID) == 0)
                {
                    const uint32_t new_mtlID = this->_materials.size();
                    mtlID_lookup[mesh_mtlID] = new_mtlID;
                    this->_materials.push_back(reader.GetMaterials().at(mesh_mtlID));
                }
                mtlIDs.push_back(mtlID_lookup[mesh_mtlID]);
            }
            else
            {
                // Materials are loaded per face.
                // As tinyobj loader also loads materaials per face,
                // the material ID array can be copied 1:1.
                // However, the indices must be checked if they are invalid.
                // It is may be the case that a mesh is loaded but it does not reference
                // any materials in the material library.
                // If the VKA_MODEL_LOAD_OPTION_IGNORE_MATERIAL is not set however,
                // the mesh must reference a valid material.
                mtlIDs.reserve(shape.mesh.material_ids.size());
                for(int i : shape.mesh.material_ids)
                {
                    const uint32_t ui = static_cast<uint32_t>(i);
                    if(ui == VKA_MTL_ID_INVALID)
                        throw std::runtime_error("[vka::Model::load]: If VKA_MODEL_LOAD_OPTION_IGNORE_MATERIAL is not set, all meshes must at least have one valid material.");
                    mtlIDs.push_back(ui);
                }
                this->_materials = reader.GetMaterials();
            }
        }
        
        this->_meshes.resize(this->_meshes.size() + 1);
        Mesh& mesh = this->_meshes.back();
        mesh.create(attribute, shape, mtlIDs);
    }
}

void vka::Model::clear(void) noexcept
{
    this->_meshes.clear();
    this->_materials.clear();
}
