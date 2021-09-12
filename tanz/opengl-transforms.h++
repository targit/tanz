#pragma once
#ifndef FILE_1834646377161DFF_700203E057555AC_INCLUDED
#define FILE_1834646377161DFF_700203E057555AC_INCLUDED

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace tz {

/* Simple transformations.

   All these transformations are so simple, you could as well program
   as needed.  However, they are collected here to have names (the
   same for all my programs), i.e. to enhance (re-)readability */

Eigen::Affine3d
clipping_matrix( Eigen::Vector3d const & v_min, Eigen::Vector3d const & v_max );

Eigen::Affine3d
camera_intrinsics_to_space_intrinsics( Eigen::Affine2d const & in );

Eigen::Matrix3d
normal_transform( Eigen::Affine3d const & trans );

Eigen::Affine2d
centered_rectangle_embedding( Eigen::Vector2d const & from, Eigen::Vector2d const & to );
/* Assuming two rectangles whose centers are at the origin,
   the transformation will make FROM scale to TO, but leaving the
   aspect as is, thus maybe introducing a border in one dimension. */

Eigen::Affine2d
rectangle_embedding( Eigen::Vector2d const & tl_from, Eigen::Vector2d const & br_from,
                     Eigen::Vector2d const & tl_to,  Eigen::Vector2d const & br_to );

Eigen::Affine3d
rectangle_embedding_3d( Eigen::Vector2d const & tl_from, Eigen::Vector2d const & br_from,
                        Eigen::Vector2d const & tl_to,   Eigen::Vector2d const & br_to );

Eigen::Affine2d
rectangle_embedding( Eigen::Vector2d const & tl_from, Eigen::Vector2d const & br_from,
                     Eigen::Vector2d const & tl_to,  Eigen::Vector2d const & br_to );
   

Eigen::Affine3d
rotate_0_180();
/* Constructs a rotation matrix around the x-axis by 180.0 degrees.
   Purely convenience, but it's used very often when i deal with
   OpenGL.  So it is more a kind of a marker.

   My "standard" coordinate system has:
   +x -> right
   +y -> down
   +z -> far

   , while OpenGLs normalised device coordinates are rotated around
   the x-axis by 180 degrees:
   +x -> right
   -y -> down
   -z -> far
*/
}

#endif
