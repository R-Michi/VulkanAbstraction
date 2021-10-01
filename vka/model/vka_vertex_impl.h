/**
* @file     vka_vertex_impl.h
* @brief    Implementation of general purpose vertex struct.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include "vka_model.h"
#include <glm/gtx/hash.hpp>

template class vka::vertex_t<3, 2, 3>;

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::vertex_t<_vc, _tc, _nc>::vertex_t(const pos_t& pos, const texc_t& texcoord, const normal_t& normal)
{
    this->position  = pos;
    this->texcoord  = texcoord;
    this->normal    = normal;
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::vertex_t<_vc, _tc, _nc>& vka::vertex_t<_vc, _tc, _nc>::operator= (const vertex_t& other)
{
    this->position  = other.position;
    this->texcoord  = other.texcoord;
    this->normal    = other.normal;
    return *this;
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::vertex_t<_vc, _tc, _nc>::vertex_t(const vertex_t& other) 
{ 
    *this = other; 
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::vertex_t<_vc, _tc, _nc>::vertex_t(vertex_t&& other) 
{ 
    *this = std::move(other);
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
vka::vertex_t<_vc, _tc, _nc>& vka::vertex_t<_vc, _tc, _nc>::operator= (vertex_t&& other)
{
    this->position = other.position;
    this->texcoord = other.texcoord;
    this->normal = other.normal;

    other.position = pos_t(0.0f);
    other.texcoord = texc_t(0.0f);
    other.normal = normal_t(0.0f);
    return *this;
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
bool vka::vertex_t<_vc, _tc, _nc>::operator== (const vertex_t& other) const noexcept
{
    //return (this->position == other.position && this->texcoord == other.texcoord && this->normal == other.normal);
    return (memcmp(this, &other, sizeof(*this)) == 0);
}

template<uint32_t _vc, uint32_t _tc, uint32_t _nc>
size_t  vka::vertex_t<_vc, _tc, _nc>::hasher::operator()(const vertex_t& v) const noexcept
{
    // combine 3 hashes
    return (
        (std::hash<vka::vertex_t<_vc, _tc, _nc>::pos_t>()(v.position) ^
        (std::hash<vka::vertex_t<_vc, _tc, _nc>::texc_t>()(v.texcoord) << 1)) >> 1) ^
        (std::hash<vka::vertex_t<_vc, _tc, _nc>::normal_t>()(v.normal) << 1
    );
}
