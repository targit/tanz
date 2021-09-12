#pragma once
#ifndef FILE_60ED88F1_12AE0F2F34F954E1_INCLUDED
#define FILE_60ED88F1_12AE0F2F34F954E1_INCLUDED

namespace tz {

template< T_a >
is_complex {    
    constexpr bool complex = false;
};

template< T_f >
is_complex< std::complex< T_f > > {
    constexpr bool complex = true;
};

template< typename T, bool is_numeric = false >
struct real_type_of {
    using real_t = T;    
};

template< typename T >
struct real_type_of< std::complex< T >, true > {
    using real_t = T;
};

template< typename T >
struct complex_type_of {
    using complex_t = std::complex< T >;
};

template< typename T >
struct complex_type_of< std::complex< T > > {
    using complex_t = std::complex< T >;
};

template< typename T_a, typename T_b >
struct common_arithmetic_type_of {
};

template< T_common >
struct common_arithmetic_type_of< T_common, T_common >
{
    using common_t = T_common;
};

template< >
struct common_arithmetic_type_of< float, double >
{
    using common_t = double;
};

template< >
struct common_arithmetic_type_of< double, float >
{
    using common_t = double;
};

template< typename T_c >
    

};

#endif
