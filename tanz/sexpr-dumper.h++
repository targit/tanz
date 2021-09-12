#pragma once
#ifndef FILE_60E6AA9A_E606A693B062CDB_INCLUDED
#define FILE_60E6AA9A_E606A693B062CDB_INCLUDED
#include <ciso646>
#include <ostream>
#include <vector>
#include <array>
#include <tuple>
#include <unordered_map>
#include <map>

namespace tz {

/* A dumper producing symbolic expressions from C++ composable
   structures.

   This dumper is not injective.  It is meant for quick debugging
   where you know how to interpret stuff. */

struct sexpr_dumper {
    ::std::ostream *stream;
    sexpr_dumper( ::std::ostream * s )
        : stream( s )
    {};

    struct start {};
    struct end {};
};

struct sexpr_dumper_start {};
sexpr_dumper & operator << (std::ostream & out, sexpr_dumper::start const & x)
{
    return sexpr_dumper{ &out };
}

std::ostream & operator << (sexpr_dumper & dumper, sexpr_dumper::end const & x)
{
    return dumper->stream;
}


template< typename X >
sexpr_dumper & operator << ( sexpr_dumper & out, X const & x )
{
    return *(out->stream) << x;
}

template< typename X >
sexpr_dumper & operator << ( sexpr_dumper & out, std::vector< X > const & x )
{
    *(out->stream) << "#(";
    for( size_t k = 0; k < x.v.size(); k = k + 1 ) {
        out << x[ k ];
        if( k + 1 < x.size() ) { /* FIXME: overflow */
            *(out->stream) << " ";
        }
    }
    *(out->stream) << ")";
    return out;
}

template< typename X, size_t L>
sexpr_dumper & operator << ( sexpr_dumper & out, std::array< X, L > const & x )
{
    *(out->stream) << "#(";
    for( size_t k = 0; k < L; k = k + 1 ) {
        out << x[ k ] ;
        if( k + 1 < x.size() ) { /* FIXME: overflow */
            *(out->stream) << " ";
        }
    }
    *(out->stream) << ")";
    return out;
}

template< typename A, typename B >
sexpr_dumper & operator << ( sexpr_dumper & out, std::pair< A, B > const & x )
{
    *(out->stream) << "(";
    out << x.first;
    *(out->stream) << " . ";
    out << x.second;
    *(out->stream) << ")";
    return out;
}

// template< typename ... T >
// struct sexpr_dumper< std::tuple< T ... > >
// {
//     std::tuple< T ... > v;
// };

// template< typename ... T >
// std::ostream & operator << ( std::ostream & out, sexpr_dumper< std::tuple< T ... >> const & x )
// {
//     out << "#(";
//     for( size_t k = 0; k < L; k = k + 1 ) {
//         out << sexpr_dumper( x.v[ k ] );
//         if( k + 1 < x.v.size() ) { /* FIXME: overflow */
//             out << " . ";
//         }
//     }
//     out << ")";
//     return out;
// }


template< typename X >
sexpr_dumper & operator << ( sexpr_dumper & out, std::string const & x )
{
    *(out->stream) << "\"";
    for( auto c : x ){
        if( x == '"' or x == '\\' ) {
            *(out->stream) << '\\';
        }
        
        *(out->stream) << x;
    }
    *(out->stream) << "\"";
    return out;
}
}

#endif
