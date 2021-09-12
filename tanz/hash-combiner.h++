#pragma once
#ifndef FILE_AE54650BBE28EB0_13701ED4873AAAEA_INCLUDED
#define FILE_AE54650BBE28EB0_13701ED4873AAAEA_INCLUDED
#include <ciso646>
#include <cstdint>

namespace tz {
inline
uint64_t hash_combine_u64( uint64_t a, uint64_t b )
{
    size_t const m =  0x9E3779B97F4A7C15;
    return a = a xor ((b + m ) + (a << 6) + (a >> 2));
}

inline
uint32_t hash_combine_u32( uint32_t a, uint32_t b )
{
    size_t const m =  0x9E3779B9;
    return a = a xor ((b + m ) + (a << 6) + (a >> 2));
}

template<typename T>
uint64_t hash_combine( T a )
{
    return a;
}

template< typename ... T_rest >
uint64_t
hash_combine( uint64_t a, uint64_t b, T_rest... rest )
{
    return hash_combine_u64( a, hash_combine( b, rest... ));
}

template< typename ... T_rest >
uint32_t
hash_combine( uint32_t a, uint32_t b, T_rest ... rest )
{
    return hash_combine_u32( a, hash_combine( b, rest... ));
}
}

#endif
