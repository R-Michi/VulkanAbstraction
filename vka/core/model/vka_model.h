/**
* @file     vka_model.h
* @brief    Declatation of the Model and Mesh classes.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "../../detail/model/vka_model.h"

namespace vka
{
    class Mesh
    {
    private:
        constexpr static uint32_t VERTEX_COMPONENT_COUNT    = 3;
        constexpr static uint32_t NORMAL_COMPONENT_COUNT    = 3;
        constexpr static uint32_t TEXCOORD_COMPONENT_COUNT  = 2;
        constexpr static uint32_t VERTEX_COMPONENT_OFFSET   = 0;
        constexpr static uint32_t NORMAL_COMPONENT_OFFSET   = VERTEX_COMPONENT_OFFSET + VERTEX_COMPONENT_COUNT;
        constexpr static uint32_t TEXCOORD_COMPONENT_OFFSET = NORMAL_COMPONENT_OFFSET + NORMAL_COMPONENT_COUNT;

        std::vector<real_t> _vertices;      // position data of vertex
        std::vector<real_t> _normals;       // normal vectors
        std::vector<real_t> _texcoords;     // texture coordinates
        std::vector<uint32_t> _indices;     // indices to reference vertices
        std::vector<uint32_t> _materials;   // material ID's of the mesh

        inline bool isset(ModelLoadOptionFlags flags, ModelLoadOptionFlagBits bit) const noexcept
        { return static_cast<bool>(flags & bit); }

    public:
        Mesh(void) = default;
        
        Mesh(const Mesh& src);
        Mesh& operator= (const Mesh& src);

        Mesh(Mesh&& src);
        Mesh& operator= (Mesh&& src);

        virtual ~Mesh(void) {}
        
        /**
        * @brief                Creates and initializes the mesh.
        * @param[in] attribute  Attribute from tiny object loader of current model.
        * @param[in] shape      Shape from tiny object loader of current model.
        * @param[in] mtlIDs     The material IDs of the mesh, they are evaluated inside the vka::Model::load function.
        */
        void create(const tinyobj::attrib_t& attribute, const tinyobj::shape_t& shape, const std::vector<uint32_t>& mtlIDs);

        /**
         * @brief                   Merges multiple vertex attributes together.
         * @param[out] data         output vertex attribute data
         * @param[in] attributes    the vertex attributes to merge together
         * @note                    The vertex attributes can be in any order and be multiple times
         *                          contained in the vertex attribute vector. The total amount of
         *                          components is the sum of all single attribute's components.
         */
        void merge(std::vector<real_t>& data, const std::vector<VertexAttributeType>& attributes) const;

        /**
         * @brief cleares the whole mesh
         */
        void clear(void) noexcept;

        /** 
         * @return  vertex's positions
         * @note    The vertex position consits of a X, Y and Z coordinate.
         *          To get the X,Y,Z components of any vertex following formulas are used:
         *          c = vertex_component_count()
         *          vx = vertices().at(index * c + 0)
         *          vy = vertices().at(index * c + 1)
         *          vz = vertices().at(index * c + 2)
         */
        const std::vector<real_t>& vertices(void) const noexcept
        { return this->_vertices; }

        /** 
         * @return  vertex's normal vectors
         * @note    The vertex normal vector consits of a X, Y and Z coordinate.
         *          To get the X,Y,Z components of any normal vector following formulas are used:
         *          c = normal_component_count()
         *          nx = normals().at(index * c + 0)
         *          ny = normals().at(index * c + 1)
         *          nz = normals().at(index * c + 2)
         */
        const std::vector<real_t>& normals(void) const noexcept
        { return this->_normals; }

        /** 
         * @return  vertex's texture coordinates
         * @note    The vertex texture coordinate consits of a U and V coordinate.
         *          To get the U,V components of any texture coordinate following formulas are used:
         *          c = texcoord_component_count()
         *          u = texcoords().at(index * c + 0)
         *          v = texcoords().at(index * c + 1)
         */
        const std::vector<real_t>& texcoords(void) const noexcept
        { return this->_texcoords; }

        /** @return vertex's indices */
        const std::vector<uint32_t>& indices(void) const noexcept
        { return this->_indices; }

        /** @return material ID's of the mesh */
        const std::vector<uint32_t>& materials(void) const noexcept
        { return this->_materials; }

        /** 
         * @return  vertex's positions
         * @note    The vertex position consits of a X, Y and Z coordinate.
         *          To get the X,Y,Z components of any vertex following formulas are used:
         *          base_address = pvertices()
         *          c = vertex_component_count
         *          vx = base_address + (index * c + 0) * sizeof(real_t)
         *          vy = base_address + (index * c + 1) * sizeof(real_t)
         *          vz = base_address + (index * c + 2) * sizeof(real_t)
         */
        const void* pvertices(void) const noexcept
        { return this->_vertices.data(); }

        /** 
         * @return  vertex's normal vectors
         * @note    The vertex normal vector consits of a X, Y and Z coordinate.
         *          To get the X,Y,Z components of any normal vector following formulas are used:
         *          base_address = pnormals()
         *          c = normal_component_count()
         *          nx = base_address + (index * c + 0) * sizeof(real_t)
         *          ny = base_address + (index * c + 1) * sizeof(real_t)
         *          nz = base_address + (index * c + 2) * sizeof(real_t)
         */
        const void* pnormals(void) const noexcept
        { return this->_normals.data(); }

        /** 
         * @return  vertex's texture coordinates
         * @note    The vertex texture coordinate consits of a U and V coordinate.
         *          To get the U,V components of any texture coordinate following formulas are used:
         *          base_address = ptexcoords()
         *          c = texcoord_component_count()
         *          u = base_address + (index * c + 0) * sizeof(real_t)
         *          v = base_address + (index * c + 1) * sizeof(real_t)
         */
        const void* ptexcoords(void) const noexcept
        { return this->_texcoords.data(); }

        /** 
         * @return  vertex's indices
         * @note    To get the correct index following formula is used:
         *          base_address = pindices()
         *          index = base_address + i * sizeof(uint32_t)
         */
        const void* pindices(void) const noexcept
        { return this->_indices.data(); }

        /** 
         * @return  material ID's of the mesh
         * @note    To get the correct material ID following formula is used:
         *          base_address = pmaterials()
         *          ID = base_address + i * sizeof(uint32_t)
         */
        const void* pmaterials(void) const noexcept
        { return this->_materials.data(); }
        
        /**
         * @return  The number of componets of the vertex's position.
         * @brief   This component count is always 3 as the tiny object loader
         *          library always loads X, Y and Z components.
         *          A 4th W component can exist but it is stored in a different array.
         *          This feature has not been implemented yet.
         */
        constexpr uint32_t vertex_component_count(void) const noexcept
        { return VERTEX_COMPONENT_COUNT; }

        /**
         * @return  The number of componets of the vertex's normal vector.
         * @brief   This component count is always 3 as the tiny object loader
         *          library always loads X, Y and Z components.
         */
        constexpr uint32_t normal_component_count(void) const noexcept
        { return NORMAL_COMPONENT_COUNT; }

        /**
         * @return  The number of componets of the vertex's texture coordinate.
         * @brief   This component count is always 2 as the tiny object loader
         *          library always loads U and V components.
         *          A 3rd W component can exist but it is stored in a different array.
         *          This feature has not been implemented yet.
         */
        constexpr uint32_t texcoord_component_count(void) const noexcept
        { return TEXCOORD_COMPONENT_COUNT; }

        /**
         * @return Total number of components: vertex + normals + texture-coordinates.
         */
        constexpr uint32_t total_component_count(void) const noexcept
        { return VERTEX_COMPONENT_COUNT + NORMAL_COMPONENT_COUNT + TEXCOORD_COMPONENT_COUNT; }

        /** @return Number of vertices stored in the mesh. */
        uint32_t vertex_count(void) const noexcept
        { return this->_vertices.size() / 3; }

        /** @return Number of indices stored in the mesh. */
        uint32_t index_count(void) const noexcept
        { return this->_indices.size(); }

        /** @return Number of primtives of the mesh */
        uint32_t primitive_count(void) const noexcept
        { return this->_indices.size() / 3; } // every 3 indices construct 1 triangle -> triangle-count = primtive-count = index-count / 3

        /** @return The size in bytes of all vertex positions. */
        size_t vertex_size(void) const noexcept
        { return this->_vertices.size() * sizeof(real_t); }

        /** @return The size in bytes of all normal vectors. */
        size_t normal_size(void) const noexcept
        { return this->_normals.size() * sizeof(real_t); }

        /** @return The size in bytes of all texture coordinates. */
        size_t texcoord_size(void) const noexcept
        { return this->_texcoords.size() * sizeof(real_t); }

        /** @return The size in bytes of all indices. */
        size_t indices_size(void) const noexcept
        { return this->_indices.size() * sizeof(uint32_t); }

        /** @return The size in bytes of all material ID's. */
        size_t material_size(void) const noexcept
        { return this->_materials.size() * sizeof(uint32_t); }

        /** @return Total size of all mesh vectors: vertices + normals + texture-coordinates */
        size_t total_size(void) const noexcept
        { return (this->_vertices.size() + this->_normals.size() + this->_texcoords.size()) * sizeof(real_t); }
    };

    class Model
    {
    private:
        std::vector<Mesh> _meshes;
        std::vector<tinyobj::material_t> _materials;

        inline bool isset(ModelLoadOptionFlags flags, ModelLoadOptionFlagBits bit) const noexcept
        { return static_cast<bool>(flags & bit); }
    
    public:
        Model(void) = default;

        Model(const Model& src);
        Model& operator= (const Model& src);

        Model(Model&& src);
        Model& operator= (Model&& src);

        virtual ~Model(void) {}

        /**
         * @brief           Loads a model from a wavefront obj file.
         * @param[in] path  path to the file
         * @param[in] flags load option flags
         * @note            If VKA_MODEL_LOAD_OPTION_IGNORE_MATERIAL is set,
         *                  then all materials that are stored in the material
         *                  library will be ignored.
         * 
         *                  If VKA_MODEL_LOAD_OPTION_FORCE_PER_MESH_MATERIAL is set,
         *                  materials are loaded per mesh instead of per face.            
         */
        void load(const std::string& path, ModelLoadOptionFlags flags = VKA_MODEL_LOAD_OPTION_DEFAULT);

        /**
         * @brief cleares the whole model
         */
        void clear(void) noexcept;

        /**
         * @return  All meshes stored within the model.
         * @note    If the model was loaded with the VKA_MODEL_LOAD_OPTION_MESH_COMBINE_BIT flag-bit set,
         *          only one mesh is generated.
         */
        const std::vector<Mesh>& meshes(void) const noexcept
        { return this->_meshes; }

        /**
         * @return  All materials within the model.
         * @note    If the model was loaded with the VKA_MODEL_LOAD_OPTION_MESH_COMBINE_BIT set,
         *          all unreferenced materials are not aviable.
         */
        const std::vector<tinyobj::material_t>& materials(void) const noexcept
        { return this->_materials; }

    };  
} // namespace vka

#ifdef VKA_IMPLEMENTATION
    #include "vka_mesh_impl.inl"
    #include "vka_model_impl.inl"
#endif