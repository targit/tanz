#include <tanz/opengl-transforms.h++>
#include <gtest/gtest.h>

TEST( clipping_matrix, identity )
{
    Eigen::Affine3d trans =
        tz::clipping_matrix( Eigen::Vector3d( -1, -1, -1),
                             Eigen::Vector3d( +1, +1, +1));
    Eigen::Affine3d identity =
        Eigen::Affine3d::Identity();
    EXPECT_TRUE( trans.isApprox( identity ));
}

TEST( clipping_matrix, translation )
{
    Eigen::Affine3d trans =
        tz::clipping_matrix( Eigen::Vector3d( -1 + 1, -1 + 2, -1 + 3),
                             Eigen::Vector3d( +1 + 1, +1 + 2, +1 + 3));
    Eigen::Affine3d correct =
        Eigen::Translation3d( -1, -2, -3 ) * Eigen::Affine3d::Identity();
    EXPECT_TRUE( trans.isApprox( correct ));
}

TEST( centered_rectangle_embedding, test0 )
{
    {
        Eigen::Affine2d trans =
            tz::centered_rectangle_embedding( Eigen::Vector2d( 1, 1), Eigen::Vector2d( 1, 2 ));
        EXPECT_DOUBLE_EQ( trans.linear()(0,0), trans.linear()(1,1));
        EXPECT_TRUE( trans.linear().diagonal().isApprox( Eigen::Vector2d( 1, 1)));
    }
    {
        Eigen::Affine2d trans =
            tz::centered_rectangle_embedding( Eigen::Vector2d( 1, 1), Eigen::Vector2d( 2, 1 ));
        EXPECT_DOUBLE_EQ( trans.linear()(0,0), trans.linear()(1,1));
        EXPECT_TRUE( trans.linear().diagonal().isApprox( Eigen::Vector2d( 1, 1)));
    }
    {
        Eigen::Affine2d trans =
            tz::centered_rectangle_embedding( Eigen::Vector2d( 1, 1), Eigen::Vector2d( 1, 0.5 ));
        EXPECT_DOUBLE_EQ( trans.linear()(0,0), trans.linear()(1,1));
        EXPECT_TRUE( trans.linear().diagonal().isApprox( Eigen::Vector2d( 0.5, 0.5)));
    }
    {
        Eigen::Affine2d trans =
            tz::centered_rectangle_embedding( Eigen::Vector2d( 2, 1), Eigen::Vector2d( 1, 1 ));
        EXPECT_DOUBLE_EQ( trans.linear()(0,0), trans.linear()(1,1));
        EXPECT_TRUE( trans.linear().diagonal().isApprox( Eigen::Vector2d( 0.5, 0.5)));
    }
}

TEST( rectangle_embedding, test0 )
{
    {
        Eigen::Vector2d tl_from(1, 1);
        Eigen::Vector2d br_from(0, 0);
        Eigen::Vector2d tl_to(1, 1);
        Eigen::Vector2d br_to(-1, -1);
        
        Eigen::Affine2d trans =
            tz::rectangle_embedding( tl_from, br_from, tl_to, br_to );
        EXPECT_TRUE( (trans * tl_from ).isApprox( tl_to ));
        EXPECT_TRUE( (trans * br_from).isApprox( br_to ));
    }
    {
        Eigen::Vector2d tl_from(0, 0);
        Eigen::Vector2d br_from(1, 1);
        Eigen::Vector2d tl_to(1, 1);
        Eigen::Vector2d br_to(-1, -1);
        
        Eigen::Affine2d trans =
            tz::rectangle_embedding( tl_from, br_from, tl_to, br_to );
        EXPECT_TRUE( (trans * tl_from ).isApprox( tl_to ));
        EXPECT_TRUE( (trans * br_from).isApprox( br_to ));
    }
    {
            Eigen::Vector2d tl_from(1, 8);
            Eigen::Vector2d br_from(0, 0);
            Eigen::Vector2d tl_to(1, 1);
            Eigen::Vector2d br_to(-1, -1);
        
            Eigen::Affine2d trans =
                tz::rectangle_embedding( tl_from, br_from, tl_to, br_to );

            EXPECT_TRUE( (trans * tl_from ).isApprox( Eigen::Vector2d( 1.0/8.0, 1.0 )));
            EXPECT_TRUE( (trans * br_from).isApprox( Eigen::Vector2d( -1.0/8.0, -1.0 )));
        

    }
}

TEST( rectangle_embedding_3d, test0 )
{
    {
        Eigen::Vector2d tl_from(1, 1);
        Eigen::Vector2d br_from(0, 0);
        Eigen::Vector2d tl_to(1, 1);
        Eigen::Vector2d br_to(-1, -1);
        
        Eigen::Affine3d trans =
            tz::rectangle_embedding_3d( tl_from, br_from, tl_to, br_to );
        EXPECT_TRUE( (trans * tl_from.homogeneous().matrix() ).isApprox( tl_to.homogeneous().matrix() ));
        EXPECT_TRUE( (trans * br_from.homogeneous().matrix() ).isApprox( br_to.homogeneous().matrix() ));
    }
    {
        Eigen::Vector2d tl_from(0, 0);
        Eigen::Vector2d br_from(1, 1);
        Eigen::Vector2d tl_to(1, 1);
        Eigen::Vector2d br_to(-1, -1);
        
        Eigen::Affine3d trans =
            tz::rectangle_embedding_3d( tl_from, br_from, tl_to, br_to );
        EXPECT_TRUE( (trans * tl_from.homogeneous().matrix() ).isApprox( tl_to.homogeneous().matrix() ));
        EXPECT_TRUE( (trans * br_from.homogeneous().matrix() ).isApprox( br_to.homogeneous().matrix() ));
    }
    {
            Eigen::Vector2d tl_from(1, 8);
            Eigen::Vector2d br_from(0, 0);
            Eigen::Vector2d tl_to(1, 1);
            Eigen::Vector2d br_to(-1, -1);
        
            Eigen::Affine3d trans =
                tz::rectangle_embedding_3d( tl_from, br_from, tl_to, br_to );

            EXPECT_TRUE( (trans * tl_from.homogeneous().matrix() ).isApprox( Eigen::Vector3d( 1.0/8.0, 1.0, 1.0 )));
            EXPECT_TRUE( (trans * br_from.homogeneous().matrix() ).isApprox( Eigen::Vector3d( -1.0/8.0, -1.0, 1.0 )));
        

    }
}

TEST( clipping_matrix, scaling )
{
    Eigen::Affine3d trans =
        tz::clipping_matrix( Eigen::Vector3d( -8, -16, -32),
                             Eigen::Vector3d( +8, +16, +32));
    Eigen::Affine3d correct =
        Eigen::Scaling( 1/8.0, 1/16.0, 1/32.0) * Eigen::Affine3d::Identity();
    EXPECT_TRUE( trans.isApprox( correct ));
}

TEST( rotate_0_180, components )
{
    Eigen::Affine3d trans = tz::rotate_0_180();
    Eigen::Matrix4d correct = Eigen::Vector4d( 1, -1, -1, 1 ).asDiagonal();
    EXPECT_TRUE( trans.matrix().isApprox( correct ));
    EXPECT_TRUE( (trans * Eigen::Vector3d( 1, 2, 5)).isApprox( Eigen::Vector3d( 1, -2, -5)));
}
