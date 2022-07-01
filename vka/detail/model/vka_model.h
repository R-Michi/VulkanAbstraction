/**
* @file     vka_model.h
* @brief    Detailed declaration of model and mesh functions.
* @author   Github: R-Michi
* Copyright (c) 2021 by R-Michi
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

namespace vka
{
    namespace detail
    {
        namespace model
        {
            using vec_t = std::vector<tinyobj::real_t>;

            /**
             * @brief   This class is a custom hasher used for an unordered_map for mesh creation.
             * @note    Hasher::hash_combine and Hasher::hash_range are implementd
             *          equal to boost::hash_combine and boost::hash_range.
             */
            class Hasher
            {
            private:
                inline size_t hash_value(real_t v) const noexcept;
                inline void hash_combine(size_t& seed, real_t v) const noexcept;
                inline size_t hash_range(const real_t* p, size_t n) const noexcept;

            public:
                size_t operator()(const vec_t& vec) const noexcept;
            };
        } // namespace model   
    } // namespace detail
} // namespace vka

#ifdef VKA_IMPLEMENTATION
    #include "vka_array_impl.inl"
#endif

