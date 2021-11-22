#include "tanz/homography.h++"
#include <stdexcept>

namespace tz {

Eigen::Matrix3d
compute_homography_2d( std::vector< Eigen::Vector2d > const & left_observations,
                       std::vector< Eigen::Vector2d > const & right_observations )
{
    if( left_observations.size() != right_observations.size() ) {
        throw
            std::domain_error( "Left and right observations are of different size." );
    }

    if( left_observations.size() < 4 ) {
        throw
            std::domain_error( "At least four points are needed for homography estimation" );
    }

    auto dlt_matrix_add_observation
        = [] (Eigen::MatrixXd & mat, size_t obs_index, Eigen::Vector2d const & a, Eigen::Vector2d const & b)
        {
            mat.block< 1, 3 >( obs_index * 3, 0 ) = Eigen::Vector3d::Zero().transpose();
            mat.block< 1, 3 >( obs_index * 3, 3 ) = - b.homogeneous().transpose();
            mat.block< 1, 3 >( obs_index * 3, 6 ) = a(1) * b.homogeneous().transpose();

            mat.block< 1, 3 >( obs_index * 3 + 1, 0 ) = b.homogeneous().transpose();
            mat.block< 1, 3 >( obs_index * 3 + 1, 3 ) = Eigen::Vector3d::Zero().transpose();
            mat.block< 1, 3 >( obs_index * 3 + 1, 6 ) = - a(0) * b.homogeneous().transpose();

            mat.block< 1, 3 >( obs_index * 3 + 2, 0 ) = - a(1) * b.homogeneous().transpose();
            mat.block< 1, 3 >( obs_index * 3 + 2, 3 ) = a(0) * b.homogeneous().transpose();
            mat.block< 1, 3 >( obs_index * 3 + 2, 6 ) = Eigen::Vector3d::Zero().transpose();
        };

    Eigen::MatrixXd dlt( left_observations.size() * 3, 9 );

    for( size_t k = 0; k < left_observations.size(); k = k + 1 ) {
        dlt_matrix_add_observation( dlt, k, left_observations.at( k ), right_observations.at( k ) );
    }
    Eigen::JacobiSVD< Eigen::MatrixXd > svd( dlt, Eigen::ComputeThinU | Eigen::ComputeFullV );

    Eigen::VectorXd h = svd.matrixV().col( 8 );

    Eigen::Matrix3d m
        = Eigen::Map< Eigen::Matrix< double, 3, 3,  Eigen::RowMajor>>( h.data());
    m = m * (1/m(2,2));
    return m;
}
}
