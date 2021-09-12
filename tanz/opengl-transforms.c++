#include <Eigen/Core>
#include <Eigen/Dense>
#include <tanz/opengl-transforms.h++>

namespace tz {

Eigen::Affine3d
clipping_matrix( Eigen::Vector3d const & v_min, Eigen::Vector3d const & v_max )
{
    Eigen::Vector3d center = (v_max + v_min) / 2.0;
    Eigen::Vector3d widths = (v_max - v_min) / 2.0;
    Eigen::Affine3d trans =
        Eigen::Scaling( widths.array().inverse().matrix()) *
        Eigen::Translation3d( - center  );
    return trans;
}

Eigen::Affine3d
camera_intrinsics_to_space_intrinsics( Eigen::Affine2d const & in )
{
    Eigen::Affine3d out = Eigen::Affine3d::Identity();
    out.linear().block< 1, 2 >(0, 0) = in.linear().block< 1, 2 >( 0, 0 );
    out.linear().block< 1, 2 >(1, 0) = in.linear().block< 1, 2 >( 1, 0 );
    out.linear().block< 2, 1 >(0, 2) = in.translation();
    return out;
}

Eigen::Matrix3d
normal_transform( Eigen::Affine3d const & trans )
{
    return trans.linear().inverse().transpose();
}

Eigen::Affine2d
centered_rectangle_embedding( Eigen::Vector2d const & from, Eigen::Vector2d const & to )
{
    double scale = (to.array() / from.array()).matrix().minCoeff();
    Eigen::Affine2d trans = Eigen::Scaling( scale, scale ) * Eigen::Affine2d::Identity();
    return trans;
}

Eigen::Affine2d
rectangle_embedding( Eigen::Vector2d const & tl_from, Eigen::Vector2d const & br_from,
                     Eigen::Vector2d const & tl_to,  Eigen::Vector2d const & br_to )
{
    Eigen::Vector2d center_from = (tl_from + br_from) / 2.0;
    Eigen::Vector2d center_to = (tl_to + br_to) / 2.0;

    Eigen::Vector2d widths_from = (br_from - tl_from )/ 2.0;
    Eigen::Vector2d widths_to = (br_to - tl_to )/ 2.0;

    double scale = ( widths_to.array() / widths_from.array()).matrix().minCoeff();

    Eigen::Affine2d trans =
        Eigen::Translation2d( center_to )
        * Eigen::Scaling( scale, scale)
        * Eigen::Translation2d( - center_from  );
    return trans;
}

Eigen::Affine3d
rectangle_embedding_3d( Eigen::Vector2d const & tl_from, Eigen::Vector2d const & br_from,
                        Eigen::Vector2d const & tl_to,  Eigen::Vector2d const & br_to )
{
    auto a2d = rectangle_embedding( tl_from,
                                    br_from,
                                    tl_to,
                                    br_to );
    Eigen::Affine3d trans = Eigen::Affine3d::Identity();
    trans.linear().block< 2, 2 >( 0, 0 ) = a2d.linear();
    trans.translation().block< 2, 1 >( 0, 0 ) = a2d.translation();
    return trans;
}

Eigen::Affine3d
rotate_0_180()
{
    return Eigen::Scaling( +1.0, -1.0, -1.0 ) * Eigen::Affine3d::Identity();
}
}
