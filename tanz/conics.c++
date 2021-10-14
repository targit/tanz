#include <tanz/conics.h++>
#include <vector>
#include <stdexcept>
#include <ciso646>

#include <iostream>

namespace tz {
namespace {

template< typename T >
Eigen::MatrixXd
conics_constraints( std::vector< T > const & obs )
{
    Eigen::MatrixXd C( obs.size(), 6 );
    for( size_t k = 0; k < obs.size(); k = k + 1 ) {
        auto const & v = obs.at( k );
        double x = v[0];
        double y = v[1];
        C.block< 1, 6 >( k, 0 ) <<
            std::pow( x, 2.0),
            2.0 * x * y,
            std::pow( y, 2.0),
            2.0 * x,
            2.0 * y,
            1.0;
    }
    return C;
}

conic_params_t
solve_conic_params( Eigen::MatrixXd const & mat )
{
    Eigen::JacobiSVD< Eigen::MatrixXd > svd( mat, Eigen::ComputeThinU | Eigen::ComputeFullV );
    Eigen::VectorXd p = svd.matrixV().col( 5 );

    return
        { p(0), p(1), p(2), p(3), p(4), p(5) };
}

inline
double
A( conic_params_t const & p )
{
    return p[0];
}

inline
double
B( conic_params_t const & p )
{
    return p[1];
}

inline
double
C( conic_params_t const & p )
{
    return p[2];
}

inline
double
D( conic_params_t const & p )
{
    return p[3];
}

inline
double
E( conic_params_t const & p )
{
    return p[4];
}

inline
double
F( conic_params_t const & p )
{
    return p[5];
}

double
discriminant( conic_params_t const & p )
{
    return A( p ) * C( p ) - std::pow( B( p ), 2.0 );
}

double
params_det( conic_params_t const & p )
{
    return
        (A( p ) * C( p ) * F( p ) + 2.0 * B( p ) * E( p ) * D( p ))
        -
        (C( p ) * std::pow( D( p ), 2.0) + A( p ) * std::pow( E( p ), 2.0) + F( p ) * std::pow( B( p ), 2.0 ));
}

inline
conic_params_t
normalised_params( conic_params_t const & p )
/* changes params so that:
      det( conic_matrix( normalised_params( p ))) = -1
   Needed for the canonical form.  We assume that
      det( conic_matrix( p )) != 0
*/
{
    auto det0 = params_det( p );
    if( (not std::isnan( det0 )) and  det0 != 0.0 ) {
        auto s = -1.0 * std::copysign( 1, det0) * std::pow( std::abs(1/det0), 1.0 / 3.0);
        return
            { p[0] * s, p[1] * s, p[2] * s, p[3] * s, p[4] * s, p[5] * s };
    } else {
        throw std::domain_error( "not a conic" );
    }
}

}

conic_params_t
fit_conic_params( std::vector< Eigen::Vector2f > const & obs )
{
    return
        normalised_params(
            solve_conic_params(
                conics_constraints( obs )));
}

conic_params_t
fit_conic_params( std::vector< Eigen::Vector2d > const & obs )
{
    return
        normalised_params(
            solve_conic_params(
                conics_constraints( obs )));
}

Eigen::Matrix3d
conic_matrix( conic_params_t const & p)
{
    Eigen::Matrix3d C;
    C <<
        A(p), p[1], p[3],
        p[1], p[2], p[4],
        p[3], p[4], p[5];
    return C;
}

conic_params_t
conic_params( Eigen::Matrix3d const & C)
{
    return { C(0,0), C(1,0), C(1,1), C(0,2), C(1,2), C(2, 2) };
}


bool
conic_is_ellipsis( conic_params_t const & p_)
{
    auto p = normalised_params( p_ );
    auto x = discriminant( p );
    if( x > 0.0 ) {
        return (A( p ) + C( p ) > 0.0);
    }
    return false;
}

#define CPP_STRING_IMPL(X) #X
#define CPP_STRING(X) CPP_STRING_IMPL(X)

conic_params_t
conic_canonical_params_non_parabola( conic_params_t const & p_ )
{
        auto p = normalised_params( p_ );

        auto d = discriminant( p );
        auto ApC = A( p ) + C( p );
        auto S = sqrt( std::pow( ApC, 2.0 ) - 4.0 * d);

        auto l1 = 0.5 * (ApC + S);
        auto l2 = 0.5 * (ApC - S);

        auto mu = 1.0 / d;
        auto a = std::sqrt( std::abs( mu / l1 ));
        auto b = std::sqrt( std::abs( mu / l2 ));

        if( ((mu * l1) > 0.0) and ((mu * l2) > 0.0 )) {
            /* Ellipsis */
            return { 1/std::pow( a, 2), 0.0, 1/std::pow( b, 2.0), 0, 0, -1.0 };
        }
        throw std::domain_error( "Code needed here: " __FILE__ "@" CPP_STRING(__LINE__));
}

conic_params_t
conic_canonical_params_parabola( conic_params_t const & p_ )
{
    throw std::domain_error( "Code needed here: " __FILE__ "@" CPP_STRING(__LINE__));
}

conic_params_t
conic_canonical_params( conic_params_t const & p )
{
    if( discriminant( p ) != 0 ) {
        return conic_canonical_params_non_parabola( p );
    } else {
        return conic_canonical_params_parabola( p );
    }
}

conic_params_t
conic_normalised_params( conic_params_t const & p )
{
    return normalised_params( p );
}

Eigen::Vector2d
conic_center( conic_params_t const & p )
{
    double d = 1.0 / discriminant( p );
    return
        Eigen::Vector2d(
            (B( p ) * E( p ) - C( p ) * D( p )) * d,
            (D( p ) * B( p ) - A( p ) * E( p )) * d );
}

}
