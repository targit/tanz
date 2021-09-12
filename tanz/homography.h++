/* Tara Lorenz (c) 2014, GPL3 or Apache License 2.0 */
#pragma once
#ifndef FILE_1821538691A25F1B_E46BA05C40E4314_INCLUDED
#define FILE_1821538691A25F1B_E46BA05C40E4314_INCLUDED

#include <vector>
#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace tz {

Eigen::Affine2d
compute_homography_2d( std::vector< Eigen::Vector2d > const & left_observations,
                       std::vector< Eigen::Vector2d > const & right_observations );


}
#endif
