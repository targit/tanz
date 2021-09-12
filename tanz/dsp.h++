#pragma once
#ifndef FILE_54F5C595B12968F_1A73B593251E5C5E_INCLUDED
#define FILE_54F5C595B12968F_1A73B593251E5C5E_INCLUDED

#include <Eigen/Dense>
#include <tanz/complex.h++>

namespace tz {

enum convolution_border_handling_t {
    CONV_BORDER_DISCARD = 1,
    CONV_BORDER_MIRROR = 2
};

template< typename T_signal, typename T_kernel >
Eigen::Matrix< tz::complex_type_of< typename T_signal::value_type >::complex_t, Eigen::Dynamic, Eigen::Dynamic >
convolve_rows( T_signal const & a,
               T_kernel const & b,
               convolution_border_handling_t border = CONV_BORDER_DISCARD)
{
    Eigen::MatrixXf h( a.rows(), a.cols() - b.size() + 1);
    if( border == CONV_BORDER_DISCARD ) {
        for( size_t row = 0; row < h.rows(); row = row + 1 ) {
            for( size_t c = 0; c < h.cols(); c = c + 1 ) {
                h( row, c ) = (a.block( row, c, 1, b.size()).array() * b.transpose().reverse().array()).matrix().sum();
            }
        }
    }
    return h;
}



template< typename T_kernel >
Eigen::MatrixXf
convolve( Eigen::MatrixXf const & a, T_kernel const & b )
{
    Eigen::MatrixXf h( a.rows(), a.cols() - b.size() + 1);
    for( size_t row = 0; row < h.rows(); row = row + 1 ) {
        for( size_t c = 0; c < h.cols(); c = c + 1 ) {
            h( row, c ) = (a.block( row, c, 1, b.size()).array() * b.transpose().reverse().array()).matrix().sum();
        }
    }
    return h;
}

/* Some simple 1 kernels */
/* Derivatives */
template< typename T = float >
Eigen::Matrix< T, 2, 1 >
kernel_deriv_1d_2()
{
    Eigen::Matrix< T, 2, 1 > kernel( +1 , -1 );
    return kernel;
}

template< typename T = float >
Eigen::Matrix< T, 3, 1 >
kernel_deriv_1d_3()
{
    Eigen::Matrix< T, 3, 1 > kernel( +1, 0, -1 );
    return kernel;
}

/* Laplace */

template< typename T = float >
Eigen::Matrix< T, 3, 1 >
kernel_laplace_1d_3()
{
    Eigen::Matrix< T, 3, 1 > kernel( +1, -2, +1 );
    return kernel;
}


template< typename T = float >
Eigen::Matrix< T, 1, 1 >
kernel_identity_1d_1( T scale = 1 )
{
    Eigen::Matrix< T, 1, 1 > kernel( scale );
    return kernel;
}

template< typename T = float >
Eigen::Matrix< T, 2, 1 >
kernel_identity_1d_2r( T scale = 1 )
{
    Eigen::Matrix< T, 1, 1 > kernel( 0, scale);
    return kernel;
}

template< typename T = float >
Eigen::Matrix< T, 2, 1 >
kernel_identity_1d_2l( T scale = 1 )
{
    Eigen::Matrix< T, 1, 1 > kernel( scale, 0 );
    return kernel;
}

template< typename T = float >
Eigen::Matrix< T, 3, 1 >
kernel_identity_1d_3( T scale )
{
    Eigen::Matrix< T, 3, 1 > kernel( 0, scale, 0 );
    return kernel;
}

Eigen::MatrixXcf
cols_spectrum( Eigen::MatrixXf const & time )
{
    Eigen::FFT< float > fft;
    Eigen::MatrixXcf frequency( time.rows(), time.cols());
    for( size_t col = 0; col < time.cols(); col = col + 1 ) {
        Eigen::VectorXf v_in( time.rows());
        v_in = time.col( col );
        Eigen::VectorXcf v_out( time.rows());
        fft.fwd( v_out, v_in );
        frequency.col( col ) = v_out;
    }
    return frequency;
}


}
    

#endif
