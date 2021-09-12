// Tara Lorenz (c) 2021, License: Apache License 2.0 or MIT
#pragma once
#include <stdexcept>
#ifndef FILE_BC870D7A92CB54F_1C115EC6556C1EED_INCLUDED
#define FILE_BC870D7A92CB54F_1C115EC6556C1EED_INCLUDED
#include <istream>
#include <ostream>
#include <vector>
#include <array>
#include <utility>
#include <tuple>
#include <variant>
#include <complex>
#include <unordered_map>
#include <optional>
#include <map>
#include <chrono>

/* A very simplistic binary serialiser and deserialiser.
   The most important limitations are:
   - pointer cannot be serialised
   - there are no checks
   - there are no limits (DOS)

   This is intended to be used with ios::binary streams.
   
   So: Don't use this in any serious scenario.
   
   I use this only for data i somehow recorded myself. */
   

namespace tz {

struct serialiser_t {
    std::ostream * stream = nullptr;

    inline
    void
    check()
    {
        if( (not stream->good()) or stream->bad()) {
            throw std::runtime_error( "couldnot write" );
        }
    }

    struct start {};
    struct end {};
};

struct deserialiser_t {
    std::istream * stream = nullptr;

    inline
    void
    check()
    {
        if( (not stream->good()) or stream->bad()) {
            throw std::runtime_error( "couldnot read" );
        }
    }


    struct start {};
    struct end {};
};

inline
serialiser_t operator << ( std::ostream & out, serialiser_t::start const & x )
{
    return serialiser_t{ & out };
}

inline
std::ostream & operator << ( serialiser_t out, serialiser_t::end const & x )
{
    return *(out.stream);
}

template< typename T >
serialiser_t operator << ( serialiser_t s, T const & x)
{
    auto & sr = s;
    sr << x;
    return s;
}

inline
deserialiser_t operator >> ( std::istream & in, deserialiser_t::start const & x )
{
    return deserialiser_t{ & in };
}

inline
std::istream & operator >> ( deserialiser_t  in, deserialiser_t::end const & x )
{
    return *(in.stream);
}

template< typename T >
deserialiser_t operator >> ( deserialiser_t s, T & x)
{
    auto & sr = s;
    sr >> x;
    return s;
}

template< typename T >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, T const & v );
/* not linkable */

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, uint64_t const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, uint32_t const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, uint16_t const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, uint8_t const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, int8_t const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, int16_t const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, int32_t const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, int64_t const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, float const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, double const & v )
{
    out.stream->write( reinterpret_cast< char const * >( &v ), sizeof( v ));
    return out;
}

// array
template< typename T, size_t l >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::array< T, l > const & v )
{
    for( auto const & x : v) {
        out << x;
    }
    return out;
}

// complex
template< typename T >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, ::std::complex< T > const & v  )
{
    out << v.real() << v.imag();
    return out;
}

// duration
template< typename R, typename P >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, ::std::chrono::duration< R, P > const & d )
{
    return out << d.count();
}

// map
template< typename key_t, typename val_t >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::map< key_t, val_t > const & v )
{
    out << uint64_t( v.size());
    for( auto const & x : v ) {
        out << x.first << x.second;
    }
    return out;
}

// optional
template< typename val_t >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::optional< val_t > const & v )
{
    if( v ) {
        out << uint8_t( 1 )
            << *v;
    } else {
        out << uint8_t( 0 );
    }
    return out;
}

// pair
template< typename A, typename B >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::pair< A, B > const & v )
{
    out << v.first;
    out << v.second;
    return out;
}

// tuple
namespace impl {
template< size_t ... k, typename T >
void
serialise_tuple( serialiser_t & out, std::integer_sequence< size_t, k ... > const & x, T const & v )
{
    ((out << std::get< k >( v )), ... );
}
}

template< typename ... T >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::tuple< T ... > const & v )
{
    ::tz::impl::serialise_tuple( out, std::make_integer_sequence< size_t, sizeof ... (T)>(), v );
    return out;
}

// unordered_map
template< typename key_t, typename val_t >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::unordered_map< key_t, val_t > const & v )
{
    out << uint64_t( v.size());
    for( auto const & x : v ) {
        out << x.first << x.second;
    }
    return out;
}

// variant
inline
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::monostate const & v )
{
    return out;
}

namespace impl {
template< size_t idx, typename ... T >
bool
serialise_variant_entry( ::tz::serialiser_t & out, std::variant< T ... > const & var )
{
    if( var.index() == idx ) {
        out << std::get< idx >( var );
        return true;
    } else {
        return false;
    }
}
template< size_t ... k, typename ... T >
void
serialise_variant( ::tz::serialiser_t & out, std::variant< T ... > const & var, std::index_sequence< k ... > indices )
{
    (serialise_variant_entry<k>( out, var ) or ... );
}
}
template< typename ... T >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::variant< T ... > const & v )
{
    out << uint64_t( v.index());
    impl::serialise_variant( out, v, std::make_index_sequence< sizeof ... (T)>());
    return out;
}

// vector
template< typename T >
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< T > const & v )
{
    out << uint64_t( v.size());
    for( auto const & x : v) {
        out << x;
    }
    return out;
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< float > const & v );

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< double > const & v );

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< uint8_t > const & v );

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< uint16_t > const & v );

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< uint32_t > const & v );

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< uint64_t > const & v );

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< int8_t > const & v );

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< int16_t > const & v );

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< int32_t > const & v );

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< int64_t > const & v );

// Deserialisation

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, uint8_t &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, uint16_t &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, uint32_t &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, uint64_t &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, int8_t &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, int16_t &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, int32_t &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, int64_t &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, float &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

inline
tz::deserialiser_t &
operator >> ( deserialiser_t & in, double &v )
{
    in.stream->read( reinterpret_cast< char * >( &v ), sizeof( v ));
    return in;
}

// array
template< typename T, size_t k>
::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & out, std::array< T, k >  & v )
{
    for( size_t l = 0; l < k; l++ ) {
        out >> v[l];
    }
    return out;
}

// complex
template< typename T >
tz::deserialiser_t &
operator >> ( deserialiser_t & in, ::std::complex< T > &v )
{
    T rp, ip;
    in >> rp >> ip;
    v = std::complex< T >( rp, ip );
    return in;
}

// duration
template< typename R, typename P >
tz::deserialiser_t &
operator >> ( tz::deserialiser_t & in, std::chrono::duration< R, P > & v )
{
    R count;
    in >> count;
    v = std::chrono::duration< R, P >( count );
    return in;
}

// map
template< typename key_t, typename val_t >
::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::map< key_t, val_t >  & v )
{
    uint64_t len;
    in >> len;
    in.check();
    v.clear();
    for( uint64_t k = 0; k < len; k++ ) {
        key_t key;
        val_t val;
        in >> key >> val;
        v.insert({key, val});
    }
    return in;
}

// optional
template< typename T >
::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, ::std::optional< T > & v )
{
    uint8_t flag;
    in >> flag;
    in.check();
    if( flag ) {
        T tmp;
        in >> tmp;
        v.emplace( std::move( tmp ));
    } else {
        v = {};
    }
    return in;
}

// pair
template< typename A, typename B >
::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & out, std::pair< A, B >  & v )
{
    return (out >> v.first >> v.second);
}

// tuple
namespace impl {
template< size_t ... k, typename T >
void
deserialise_tuple( deserialiser_t & out, std::integer_sequence< size_t, k ... > const & x, T  & v )
{
    ((out >> std::get< k >( v )), ... );
}
}

template< typename ... T >
::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & out, std::tuple< T ... >  & v )
{
    ::tz::impl::deserialise_tuple( out, std::make_integer_sequence< size_t, sizeof ... (T)>(), v );
    return out;
}

// variant
namespace impl {

template< typename E , size_t k, typename ... T >
struct variant_deserialiser
{
    void
    read( ::tz::deserialiser_t & in, ::std::variant< T ... > & var )
    {
        E tmp;
        in >> tmp;
        var.template emplace< k >( std::move( tmp ));
    }
};

template< size_t k, typename ... T >
struct variant_deserialiser< ::std::monostate, k, T ... >
{
    void
    read( ::tz::deserialiser_t & in, ::std::variant< T ... > & var )
    {
    }
};

template< size_t idx_stat, typename ... T >
bool
deserialise_variant_element( ::tz::deserialiser_t & in, ::std::variant< T ... > & var, uint64_t idx )
{
    if( idx_stat == idx) {
        using element_type = typename std::tuple_element< size_t( idx_stat ), std::tuple< T ... >>::type;
        variant_deserialiser< element_type, idx_stat, T ... >().read( in, var );
        return true;
    } else {
        return false;
    }
}

template< size_t ... k, typename ... T >
void
deserialise_variant( tz::deserialiser_t & in, ::std::variant< T ... > & var, uint64_t idx, std::index_sequence< k ... > indices )
{
    (deserialise_variant_element< k >( in, var, idx ) or ...);
}
}

template< typename ... T >
::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, ::std::variant< T ... > & var )
{
    uint64_t idx;
    in >> idx;
    in.check();
    ::tz::impl::deserialise_variant( in, var, idx, std::make_index_sequence< sizeof ... (T) >());
    return in;
}

// vector
template< typename T >
::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< T >  & v )
{
    uint64_t len;
    in >> len;
    in.check();
    v.resize( 0 );
    v.reserve( len );
    for( size_t k = 0; k < len; k = k + 1 ) {
        T x;
        in >> x;
        v.push_back( x );
    }
    return in;
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< float > & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< double > & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< uint8_t > & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< uint16_t > & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< uint32_t > & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< uint64_t > & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< int8_t > & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< int16_t > & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< int32_t > & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< int64_t > & v );

// unordered_map
template< typename key_t, typename val_t >
::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::unordered_map< key_t, val_t >  & v )
{
    uint64_t len;
    in >> len;
    in.check();
    v.clear();
    v.reserve( len );
    for( uint64_t k = 0; k < len; k++ ) {
        key_t key;
        val_t val;
        in >> key >> val;
        v.insert({key, val});
    }
    return in;
}
}
#endif
