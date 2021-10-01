/**
* @file     vka_model.h
* @brief    Experimental implementation of model loading.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* 
* NOTE: This is just an experimental implementation of model loading.
* It is not the final version, the implementation can not determine
* at runtime how many components there are for: vertices, texture coordinates
* and normal vectors. Additionally the library is restricted to only be
* capable of loading .obj files. The final version will support the
* most (common) model formats.
* 
* Now, the tiny object loader library will be used in the background.
* Which is, like i mentioned before, only capable of loading
* .obj files. For future implementation I will use assimp in the
* background, which is a more powerful library.
*/

#pragma once

#include "../vka_global.h"
#include "../vka_types.h"
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <glm/glm.hpp>

namespace vka
{
    template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
    struct vertex_t
    {
        static_assert(_vc <= 4U && _vc >= 1, "Vertex component count of vka::Mesh::vertex_t is greater than 4 or less than 1!");
        static_assert(_tc <= 4U && _tc >= 1, "Texture coordinate component count of vka::Mesh::vertex_t is greater than 4 or less than 1!");
        static_assert(_nc <= 4U && _nc >= 1, "Normal vector component count of vka::Mesh::vertex_t is greater than 4 or less than 1!");

        using pos_t     = glm::vec<_vc, tinyobj::real_t, glm::qualifier::defaultp>;
        using texc_t    = glm::vec<_tc, tinyobj::real_t, glm::qualifier::defaultp>;
        using normal_t  = glm::vec<_nc, tinyobj::real_t, glm::qualifier::defaultp>;
        
        pos_t position;
        texc_t texcoord;
        normal_t normal;

        /**
        * @param[in] pos: vertex position
        * @param[in] texcoord: texture coodinate
        * @param[in] normal: surface normal vector
        */
        vertex_t(const pos_t& pos = pos_t(0.0f), const texc_t& texcoord = texc_t(0.0f), const normal_t& normal = normal_t(0.0f));

        vertex_t(const vertex_t& other);
        vertex_t& operator= (const vertex_t& other);

        vertex_t(vertex_t&& other);
        vertex_t& operator= (vertex_t&& other);

        ~vertex_t(void) { /*dtor*/ }
        
        /**
        * @brief Equal operator.
        */
        bool operator== (const vertex_t& other) const noexcept;
        
        // used for std::unordered_map
        struct hasher
        {
            /**
            * @brief Generates hash code of vertex.
            * @param[in] v: vertex to hash
            * @return hash code of vertex
            */
            size_t operator()(const vertex_t& v) const noexcept;
        };
    };

    template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
    class Mesh
    {
    private:
        std::vector<vertex_t<_vc, _tc, _nc> > _vertices;    // vertices of mesh
        std::vector<uint32_t> _indices;                     // indices of mesh
        std::vector<int> _materials;                        // corresponding material ID's
        std::string _name;                                  // name of mesh
    
    public:
        Mesh(void) = default;   // not allowed
        
        Mesh(const Mesh& other);
        Mesh& operator= (const Mesh& other);
        
        Mesh(Mesh&& other);
        Mesh& operator= (Mesh&& other);
        
        virtual ~Mesh(void) {}

        /**
        * @brief Creates and initializes the mesh.
        * @param attribute Attribute type from tiny object loader of current model.
        * @param shape Sha�e type from tiny object loader of current model.
        */
        void create(const tinyobj::attrib_t& attribute, const tinyobj::shape_t& shape);
        
        /** @return vector of vertices. */
        const std::vector< vertex_t<_vc, _tc, _nc> >& vertices(void) const noexcept;
        
        /** @return vector of indices */
        const std::vector<uint32_t>& indices(void) const noexcept;
        
        /** @return vector of the corresponding material ID's */
        const std::vector<int>& material_ids(void) const noexcept;
        
        /** @return name of the mesh  */
        const std::string& name(void) const noexcept;
        
        /** @return pointer to the vertex data  */
        const void* vertex_data(void) const noexcept;
        
        /** @return pointer to the index data */
        const void* index_data(void) const noexcept;
        
        /** @return size in bytes of all vertices */
        size_t vertex_size(void) const noexcept;
        
        /** @return size in bytes of all indices */
        size_t index_size(void) const noexcept;
        
        /** @brief Cleares the mesh. */
        void clear(void) noexcept;
    };

    template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
    class Model
    {
    private:
        std::vector< Mesh<_vc, _tc, _nc> > _meshes;     // all meshes of the model
        std::vector<tinyobj::material_t> _materials;    // all materials of the model
    
    public:
        Model(void) = default;
        virtual ~Model(void) {}
        
        Model(const Model& other);
        Model& operator= (const Model& other);
        
        Model(Model&& other);
        Model& operator= (Model&& other);
        
        /**
        * @brief Loads the model into the memory.
        * @param[in] path: path to the model file
        */
        void load(const std::string& path);
        
        /**
        * @brief Combines vertices and indices of all meshes into a single vertex and index vector.
        * NOTE: Materials for different meshes will become useless!
        * @param[out] vertices: vertex vector
        * @param[out] indices: index vector
        */
        void merge(std::vector< vertex_t<_vc, _tc, _nc> >& vertices, std::vector<uint32_t>& indices);
        
        /** @return vector of all meshes */
        const std::vector< Mesh<_vc, _tc, _nc> >& meshes(void) const noexcept;
        
        /** @return vector of all materials */
        const std::vector<tinyobj::material_t> materials(void) const noexcept;
        
        /** @brief Cleares model / cleares all internal vectors. */
        void clear(void) noexcept;
    };
    
    // typical definitions
    using vertex323_t = vertex_t<3, 2, 3>;
    using vertex423_t = vertex_t<4, 2, 3>;
    using vertex333_t = vertex_t<3, 3, 3>;
    using vertex433_t = vertex_t<4, 3, 3>;
    
    using Mesh323 = Mesh<3, 2, 3>;
    using Mesh423 = Mesh<4, 2, 3>;
    using Mesh333 = Mesh<3, 3, 3>;
    using Mesh433 = Mesh<4, 3, 3>;
    
    using Model323 = Model<3, 2, 3>;
    using Model423 = Model<4, 2, 3>;
    using Model333 = Model<3, 3, 3>;
    using Model433 = Model<4, 3, 3>;
}