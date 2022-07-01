/**
* @file     vka_model_impl.inl
* @brief    Detailed implementation of model and mesh functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

inline size_t vka::detail::model::Hasher::hash_value(real_t v) const noexcept
{
    return std::hash<tinyobj::real_t>()(v);
}

inline void vka::detail::model::Hasher::hash_combine(size_t& seed, real_t v) const noexcept
{
    seed ^= this->hash_value(v) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
}

inline size_t vka::detail::model::Hasher::hash_range(const real_t* p, size_t n) const noexcept
{
    size_t seed = 0;
    const real_t* const end = p + n;
    for(; p < end; p++)
        this->hash_combine(seed, *p);
    return seed;
}

size_t vka::detail::model::Hasher::operator()(const vec_t& vec) const noexcept
{
    return this->hash_range(vec.data(), vec.size());
}