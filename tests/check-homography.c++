#include <gtest/gtest.h>
#include <tanz/homography.h++>

TEST( homography, identity )
{
    Eigen::Vector2d x0( 0, 0);
    Eigen::Vector2d x1( 0, 1);
    Eigen::Vector2d x2( 1, 0);
    Eigen::Vector2d x3( 1, 1);
    Eigen::Affine2d h =
        tz::compute_homography_2d( {x0, x1, x2, x3},
                                   {x0, x1, x2, x3});
    EXPECT_TRUE( h.isApprox( Eigen::Affine2d::Identity()) );
}

TEST( homography, mirrormirror )
{
    Eigen::Vector2d x0( 0, 0);
    Eigen::Vector2d x1( 0, 1);
    Eigen::Vector2d x2( 1, 0);
    Eigen::Vector2d x3( 1, 1);
    Eigen::Affine2d h =
        tz::compute_homography_2d( {x0, x1, x2, x3},
                                   {-x0, -x1, -x2, -x3});
    EXPECT_TRUE( h.isApprox( Eigen::Affine2d( Eigen::Scaling( -1.0, -1.0)) ));
}

TEST( homography, translation )
{
    Eigen::Vector2d x0( 0, 0);
    Eigen::Vector2d x1( 0, 1);
    Eigen::Vector2d x2( 1, 0);
    Eigen::Vector2d x3( 1, 1);

    Eigen::Vector2d off(3, 5);
    
    Eigen::Affine2d h =
        tz::compute_homography_2d( {x0 + off, x1 + off, x2 + off, x3 + off},
                                   {x0, x1, x2, x3});
    EXPECT_TRUE( h.linear().isApprox( Eigen::Affine2d::Identity().linear() ));
    EXPECT_TRUE( h.translation().isApprox( off ));
}


TEST( homography, random )
{

    Eigen::Matrix3d correct = Eigen::Matrix3d::Random();
    correct = correct * (1.0/correct(2,2)); // Just to make
                                            // comparision easier
    
    Eigen::Vector2d r0( 0, 0);
    Eigen::Vector2d r1( 0, 1);
    Eigen::Vector2d r2( 1, 0);
    Eigen::Vector2d r3( 1, 1);
    Eigen::Vector2d r4( 1, 1);
    Eigen::Vector2d r5( 1, 1);

    Eigen::Vector2d l0 = (correct * r0.homogeneous()).hnormalized();
    Eigen::Vector2d l1 = (correct * r1.homogeneous()).hnormalized();
    Eigen::Vector2d l2 = (correct * r2.homogeneous()).hnormalized();
    Eigen::Vector2d l3 = (correct * r3.homogeneous()).hnormalized();
    Eigen::Vector2d l4 = (correct * r4.homogeneous()).hnormalized();
    Eigen::Vector2d l5 = (correct * r5.homogeneous()).hnormalized();
    
    Eigen::Affine2d h =
        tz::compute_homography_2d( {l0, l1, l2, l3, l4, l5},
                                   {r0, r1, r2, r3, r4, r5} );
    EXPECT_TRUE( h.matrix().isApprox( correct ) );
}
