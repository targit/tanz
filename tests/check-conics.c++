#include <tanz/conics.h++>
#include <gtest/gtest.h>
#include <Eigen/Geometry>

TEST( conics, conic_matrix )
{
    double a = 1.0, b = 2.0, c = 3.0, d = 4.0, e = 5.0, f = 6.0;
    auto C =
        tz::conic_matrix( { a, b, c, d, e, f } );
    Eigen::Matrix3d correct;
    correct <<
        a, b, d,
        b, c, e,
        d, e, f;
    EXPECT_TRUE( C.isApprox( correct ));
}

TEST( conics, conic_params )
{
    double a = 1.0, b = 2.0, c = 3.0, d = 4.0, e = 5.0, f = 6.0;
    tz::conic_params_t correct = { a, b, c, d, e, f };
    auto roundtrip = tz::conic_params( tz::conic_matrix( correct ) );
    EXPECT_EQ( roundtrip, correct );
}

namespace {
std::vector< Eigen::Vector2f >
sample_canonical_ellipsis_float( double a, double b )
{
    std::vector< Eigen::Vector2f > obs( 8 );

    obs.at( 0 ) = Eigen::Vector2f(  a,  0 );
    obs.at( 1 ) = Eigen::Vector2f( -a,  0 );
    obs.at( 2 ) = Eigen::Vector2f(  0,  b );
    obs.at( 3 ) = Eigen::Vector2f(  0, -b );
    obs.at( 4 ) = Eigen::Vector2f( +a / std::sqrt( 2.0 ), +b / std::sqrt( 2.0 ));
    obs.at( 5 ) = Eigen::Vector2f( +a / std::sqrt( 2.0 ), -b / std::sqrt( 2.0 ));
    obs.at( 6 ) = Eigen::Vector2f( -a / std::sqrt( 2.0 ), +b / std::sqrt( 2.0 ));
    obs.at( 7 ) = Eigen::Vector2f( -a / std::sqrt( 2.0 ), -b / std::sqrt( 2.0 ));
    return obs;
}

std::vector< Eigen::Vector2d >
sample_canonical_ellipsis_double( double a, double b )
{
    std::vector< Eigen::Vector2d > obs( 8 );

    obs.at( 0 ) = Eigen::Vector2d(  a,  0 );
    obs.at( 1 ) = Eigen::Vector2d( -a,  0 );
    obs.at( 2 ) = Eigen::Vector2d(  0,  b );
    obs.at( 3 ) = Eigen::Vector2d(  0, -b );
    obs.at( 4 ) = Eigen::Vector2d( a / sqrt( 2 ), b / sqrt( 2 ));
    obs.at( 5 ) = Eigen::Vector2d( a / sqrt( 2 ), -b / sqrt( 2 ));
    obs.at( 6 ) = Eigen::Vector2d( -a / sqrt( 2 ), b / sqrt( 2 ));
    obs.at( 7 ) = Eigen::Vector2d( -a / sqrt( 2 ), -b / sqrt( 2 ));
    return obs;
}

}

TEST( conics, fit_unitcircle )
{
    { /* float version */
        auto params =
            tz::fit_conic_params(
                sample_canonical_ellipsis_float( 1.0, 1.0 ));
        EXPECT_FLOAT_EQ( params[0], 1.0);
        EXPECT_FLOAT_EQ( params[1], 0.0);
        EXPECT_FLOAT_EQ( params[2], 1.0);
        EXPECT_FLOAT_EQ( params[3], 0.0);
        EXPECT_FLOAT_EQ( params[4], 0.0);
        EXPECT_FLOAT_EQ( params[5], -1.0);
        EXPECT_TRUE( tz::conic_is_ellipsis( params ));
    }
    { /* double version */
        auto params =
            tz::fit_conic_params(
                sample_canonical_ellipsis_double( 1.0, 1.0));
        EXPECT_DOUBLE_EQ( params[0], 1.0);
        EXPECT_DOUBLE_EQ( params[1], 0.0);
        EXPECT_DOUBLE_EQ( params[2], 1.0);
        EXPECT_DOUBLE_EQ( params[3], 0.0);
        EXPECT_DOUBLE_EQ( params[4], 0.0);
        EXPECT_DOUBLE_EQ( params[5], -1.0);
        EXPECT_TRUE( tz::conic_is_ellipsis( params ));
    }
}

TEST( conics, fit_ellipsis_1_2 )
{
    { /* float version */
        auto obs =
            sample_canonical_ellipsis_float( 1.0, 2.0 );
        auto params =
            tz::conic_canonical_params( tz::fit_conic_params( obs ));
        EXPECT_FLOAT_EQ( params.at( 0 ), 1.0 );
        EXPECT_FLOAT_EQ( params.at( 1 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 2 ), 0.25 );
        EXPECT_FLOAT_EQ( params.at( 3 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 4 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 5 ), -1.0 );
    }
}

TEST( conics, fit_ellipsis_1_2_rotated )
{
    { /* float version */
        auto obs =
            sample_canonical_ellipsis_float( 1.0, 2.0 );

        for( auto & ob : obs ) {
            ob = Eigen::Rotation2Df( 0.23 ) * ob;
        }

        auto params =
            tz::fit_conic_params( obs );
        EXPECT_TRUE( tz::conic_center( params ).isApprox( Eigen::Vector2d::Zero() ));

        auto canoic_params =
            tz::conic_canonical_params( params );
        EXPECT_FLOAT_EQ( canoic_params.at( 0 ), 1.0 );
        EXPECT_FLOAT_EQ( canoic_params.at( 1 ), 0.0 );
        EXPECT_FLOAT_EQ( canoic_params.at( 2 ), 0.25 );
        EXPECT_FLOAT_EQ( canoic_params.at( 3 ), 0.0 );
        EXPECT_FLOAT_EQ( canoic_params.at( 4 ), 0.0 );
        EXPECT_FLOAT_EQ( canoic_params.at( 5 ), -1.0 );
    }
}

TEST( conics, fit_ellipsis_1_2_shifted )
{
    { /* float version */
        auto obs =
            sample_canonical_ellipsis_float( 1.0, 2.0 );

        for( auto & ob : obs ) {
            ob = Eigen::Translation2f( 3.0, -12.0) * ob;
        }

        auto params =
            tz::conic_canonical_params( tz::fit_conic_params( obs ));
        EXPECT_FLOAT_EQ( params.at( 0 ), 1.0 );
        EXPECT_FLOAT_EQ( params.at( 1 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 2 ), 0.25 );
        EXPECT_FLOAT_EQ( params.at( 3 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 4 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 5 ), -1.0 );

        EXPECT_TRUE(
            tz::conic_center( tz::fit_conic_params( obs ) )
            .isApprox( Eigen::Vector2d( 3.0, -12.0 )));
    }
        { /* double version */
        auto obs =
            sample_canonical_ellipsis_double( 1.0, 2.0 );

        for( auto & ob : obs ) {
            ob = Eigen::Translation2d( 3.0, -12.0) * ob;
        }

        auto params =
            tz::conic_canonical_params( tz::fit_conic_params( obs ));
        EXPECT_FLOAT_EQ( params.at( 0 ), 1.0 );
        EXPECT_FLOAT_EQ( params.at( 1 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 2 ), 0.25 );
        EXPECT_FLOAT_EQ( params.at( 3 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 4 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 5 ), -1.0 );

        EXPECT_TRUE(
            tz::conic_center( tz::fit_conic_params( obs ) )
            .isApprox( Eigen::Vector2d( 3.0, -12.0 )));
    }
}

TEST( conics, fit_ellipsis_1_2_rotated_shifted )
{
    { /* float version */
        auto obs =
            sample_canonical_ellipsis_float( 1.0, 2.0 );

        for( auto & ob : obs ) {
            ob = Eigen::Translation2f( 3.0, -12.0) * Eigen::Rotation2Df( 0.234) * ob;
        }

        auto params =
            tz::conic_canonical_params( tz::fit_conic_params( obs ));
        EXPECT_FLOAT_EQ( params.at( 0 ), 1.0 );
        EXPECT_FLOAT_EQ( params.at( 1 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 2 ), 0.25 );
        EXPECT_FLOAT_EQ( params.at( 3 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 4 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 5 ), -1.0 );

        EXPECT_TRUE(
            tz::conic_center( tz::fit_conic_params( obs ) )
            .isApprox( Eigen::Vector2d( 3.0, -12.0 )));
        auto a = std::sqrt( 1 / params.at(0) );
        auto b = std::sqrt( 1 / params.at(2) );
        EXPECT_FLOAT_EQ( 1, a );
        EXPECT_FLOAT_EQ( 2, b );
    }
    { /* double version */
        double alpha = 0.234;
        auto obs =
            sample_canonical_ellipsis_double( 1.0, 2.0 );

        for( auto & ob : obs ) {
            ob =  Eigen::Translation2d( 3.0, -12.0) * Eigen::Rotation2Dd( alpha) * ob;
        }

        auto params =
            tz::conic_canonical_params( tz::fit_conic_params( obs ));
        EXPECT_FLOAT_EQ( params.at( 0 ), 1.0 );
        EXPECT_FLOAT_EQ( params.at( 1 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 2 ), 0.25 );
        EXPECT_FLOAT_EQ( params.at( 3 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 4 ), 0.0 );
        EXPECT_FLOAT_EQ( params.at( 5 ), -1.0 );

        EXPECT_TRUE(
            tz::conic_center( tz::fit_conic_params( obs ) )
            .isApprox( Eigen::Vector2d( 3.0, -12.0 )));
    }
}
