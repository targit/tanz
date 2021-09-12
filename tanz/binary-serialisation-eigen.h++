#pragma once
#ifndef FILE_2AFD9BF30B136AC_1E366AB0056C2C5D_INCLUDED
#define FILE_2AFD9BF30B136AC_1E366AB0056C2C5D_INCLUDED
#include <tanz/binary-serialisation.h++>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace tz {

template< typename T >
serialiser_t &
operator << ( serialiser_t & out, Eigen::Matrix< T, Eigen::Dynamic, Eigen::Dynamic > const & m )
{
    out << int64_t( m.rows()) << int64_t(m.cols());    
    (*out.stream).write( reinterpret_cast< char const * >( m.data()), sizeof( T) * m.rows() * m.cols());
    return out;
}

template< typename T, int rspec >
serialiser_t &
operator << ( serialiser_t & out, Eigen::Matrix< T, rspec, Eigen::Dynamic > const & m )
{
    out << int64_t(m.cols());
    std::vector< T > buffer( m.rows() * m.cols());
    (*out.stream).write( reinterpret_cast< char const * >( m.data()), sizeof( T ) * m.rows() * m.cols());
    return out;
}

template< typename T, int cspec >
serialiser_t &
operator << ( serialiser_t & out, Eigen::Matrix< T, Eigen::Dynamic, cspec > const & m )
{
    out << int64_t(m.rows());
    (*out.stream).write( reinterpret_cast< char const * >( m.data()), sizeof( T ) * m.rows() * m.cols());
    return out;
}

template< typename T, int rspec, int cspec >
serialiser_t &
operator << ( serialiser_t & out, Eigen::Matrix< T, rspec, cspec > const & m )
{
    (*out.stream).write( reinterpret_cast< char const * >( m.data()), sizeof( T ) * m.rows() * m.cols());
    return out;
}

serialiser_t &
operator << ( serialiser_t & out, Eigen::Affine3d const & m );

serialiser_t &
operator << ( serialiser_t & out, Eigen::Affine3f const & m );

serialiser_t &
operator << ( serialiser_t & out, Eigen::Affine2d const & m );

serialiser_t &
operator << ( serialiser_t & out, Eigen::Affine2f const & m );

template< typename T >
deserialiser_t &
operator >> ( deserialiser_t & in, Eigen::Matrix< T, Eigen::Dynamic, Eigen::Dynamic > & m )
{
    int64_t rows;
    int64_t cols;
    in >> rows >> cols;
    in.check();

    m.resize( rows, cols );

    in.stream->read( reinterpret_cast< char* >( m.data()),
                     sizeof( T ) * rows * cols );
    return in;
}

template< typename T, int rspec >
deserialiser_t &
operator >> ( deserialiser_t & in, Eigen::Matrix< T, rspec, Eigen::Dynamic > & m )
{
    int64_t cols;
    in >> cols;
    in.check();

    m.resize( rspec, cols );
    in.stream->read( reinterpret_cast< char* >( m.data()),
                     sizeof( T ) * cols * rspec );
    return in;
}

template< typename T, int cspec >
deserialiser_t &
operator >> ( deserialiser_t & in, Eigen::Matrix< T, Eigen::Dynamic, cspec > & m )
{
    int64_t rows;
    in >> rows;
    in.check();

    m.resize( rows, cspec );
    in.stream->read( reinterpret_cast< char* >( m.data()),
                      sizeof( T ) * m.rows() * m.cols());
    return in;
}

template< typename T, int rspec, int cspec >
deserialiser_t &
operator >> ( deserialiser_t & in, Eigen::Matrix< T, rspec, cspec > & m )
{
    in.stream->read( reinterpret_cast< char* >( m.data()),
                      sizeof( T ) * m.rows() * m.cols());
    return in;
}

deserialiser_t &
operator >> ( deserialiser_t & out, Eigen::Affine3d & m );

deserialiser_t &
operator >> ( deserialiser_t & out, Eigen::Affine3f & m );

deserialiser_t &
operator >> ( deserialiser_t & out, Eigen::Affine2d & m );

deserialiser_t &
operator >> ( deserialiser_t & out, Eigen::Affine2f & m );

}

#endif
