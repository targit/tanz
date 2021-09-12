#include <tanz/binary-serialisation.h++>
namespace tz {
namespace {

template< typename T >
inline
tz::serialiser_t &
ser_vector( ::tz::serialiser_t & out, std::vector< T > const & v )
{
    out << uint64_t( v.size());
    out.stream->write( reinterpret_cast< char const *>( v.data()),
                       sizeof( T ) * v.size());
    return out;
}

}
::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< float > const & v )
{
    return ser_vector(out, v );
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< double > const & v )
{
    return ser_vector(out, v );
}


::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< uint8_t > const & v )
{
    return ser_vector( out, v );
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< uint16_t > const & v )
{
    return ser_vector( out, v );
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< uint32_t > const & v )
{
    return ser_vector( out, v );
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< uint64_t > const & v )
{
    return ser_vector( out, v );
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< int8_t > const & v )
{
    return ser_vector( out, v );
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< int16_t > const & v )
{
    return ser_vector( out, v );
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< int32_t > const & v )
{
    return ser_vector( out, v );
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, std::vector< int64_t > const & v )
{
    return ser_vector( out, v );
}

namespace {
template< typename T >
::tz::deserialiser_t &
des( ::tz::deserialiser_t & in, std::vector< T >  & v )
{
    uint64_t len;
    in >> len;
    in.check();
    v.resize( len );
    in.stream->read( reinterpret_cast< char* >( v.data() ), sizeof( T ) * len );
    return in;
}
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< float > & v )
{
    return des( in, v );
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< double > & v )
{
    return des( in, v );
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< uint8_t > & v )
{
    return des( in, v );
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< uint16_t > & v )
{
    return des( in, v );
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< uint32_t > & v )
{
    return des( in, v );
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< uint64_t > & v )
{
    return des( in, v );
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< int8_t > & v )
{
    return des( in, v );
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< int16_t > & v )
{
    return des( in, v );
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< int32_t > & v )
{
    return des( in, v );
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, std::vector< int64_t > & v )
{
    return des( in, v );
}

}
