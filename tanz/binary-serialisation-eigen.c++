#include <tanz/binary-serialisation-eigen.h++>

namespace tz {

serialiser_t &
operator << ( serialiser_t & out, Eigen::Affine3d const & m )
{
    out.stream->write( reinterpret_cast< char const* >( m.data()),
                       sizeof( double ) * 16 );
    return out;
}

serialiser_t &
operator << ( serialiser_t & out, Eigen::Affine3f const & m )
{
    out.stream->write( reinterpret_cast< char const* >( m.data()),
                       sizeof( float ) * 16 );
    return out;
}

serialiser_t &
operator << ( serialiser_t & out, Eigen::Affine2d const & m )
{
    out.stream->write( reinterpret_cast< char const* >( m.data()),
                       sizeof( double ) * 9 );
    return out;
}

serialiser_t &
operator << ( serialiser_t & out, Eigen::Affine2f const & m )
{
    out.stream->write( reinterpret_cast< char const* >( m.data()),
                       sizeof( float ) * 9 );
    return out;
}

deserialiser_t &
operator >> ( deserialiser_t & out, Eigen::Affine3d & m )
{
    out.stream->read( reinterpret_cast< char* >( m.data() ),
                      sizeof( double ) * 16 );
    return out;
}

deserialiser_t &
operator >> ( deserialiser_t & out, Eigen::Affine3f & m )
{
    out.stream->read( reinterpret_cast< char* >( m.data() ),
                      sizeof( float ) * 16 );
    return out;
}

deserialiser_t &
operator >> ( deserialiser_t & out, Eigen::Affine2d & m )
{
    out.stream->read( reinterpret_cast< char* >( m.data() ),
                      sizeof( double ) * 9 );
    return out;
}

deserialiser_t &
operator >> ( deserialiser_t & out, Eigen::Affine2f & m )
{
    out.stream->read( reinterpret_cast< char* >( m.data() ),
                      sizeof( float ) * 9 );
    return out;
}
}
