#include <gtest/gtest.h>
#include <sstream>
#include <limits>
#include <tanz/binary-serialisation-eigen.h++>

TEST( binary_serialisation_eigen, matrixXf )
{
    using xtype = Eigen::MatrixXf;

    xtype a( 3, 2 );
    a <<
        1, 2, 3,
        4, 5, 6;

    std::stringstream str;

    str << tz::serialiser_t::start()
        << a;

    EXPECT_EQ( str.str().size(), sizeof( int64_t ) * 2 + 6 * sizeof( float ));
    
    xtype b;
    str >> tz::deserialiser_t::start()
        >> b;

    EXPECT_EQ( b.rows(), a.rows());
    EXPECT_EQ( b.cols(), a.cols());
    EXPECT_EQ( a, b );
}

TEST( binary_serialisation_eigen, matrix_d_2_f )
{
    using xtype = Eigen::Matrix< float, Eigen::Dynamic, 2 >;

    xtype a( 3, 2 );
    a <<
        1, 2, 3,
        4, 5, 6;

    std::stringstream str;

    str << tz::serialiser_t::start()
        << a;

    EXPECT_EQ( str.str().size(), sizeof( int64_t ) * 1 + 6 * sizeof( float ));
    
    xtype b;
    str >> tz::deserialiser_t::start()
        >> b;

    EXPECT_EQ( b.rows(), a.rows());
    EXPECT_EQ( b.cols(), a.cols());
    EXPECT_EQ( a, b );
}

TEST( binary_serialisation_eigen, matrix_3_d_f )
{
    using xtype = Eigen::Matrix< float, 3, Eigen::Dynamic >;

    xtype a( 3, 2 );
    a <<
        1, 2, 3,
        4, 5, 6;

    std::stringstream str;

    str << tz::serialiser_t::start()
        << a;

    EXPECT_EQ( str.str().size(), sizeof( int64_t ) * 1 + 6 * sizeof( float ));
    
    xtype b;
    str >> tz::deserialiser_t::start()
        >> b;

    EXPECT_EQ( b.rows(), a.rows());
    EXPECT_EQ( b.cols(), a.cols());
    EXPECT_EQ( a, b );
}

TEST( binary_serialisation_eigen, matrix_3_2_f )
{
    using xtype = Eigen::Matrix< float, 3, 2 >;

    xtype a( 3, 2 );
    a <<
        1, 2, 3,
        4, 5, 6;

    std::stringstream str;

    str << tz::serialiser_t::start()
        << a;

    EXPECT_EQ( str.str().size(), sizeof( int64_t ) * 0 + 6 * sizeof( float ));
    
    xtype b;
    str >> tz::deserialiser_t::start()
        >> b;

    EXPECT_EQ( b.rows(), a.rows());
    EXPECT_EQ( b.cols(), a.cols());
    EXPECT_EQ( a, b );
}

TEST( binary_serialisation_eigen, vector2d )
{
    Eigen::Vector2d v;
    v << 0.4, -.5;

    std::stringstream sstr;

    EXPECT_EQ( v( 0 ), 0.4 );
    EXPECT_EQ( v( 1 ), -.5 );
    
    sstr << tz::serialiser_t::start()
         << v
         << tz::serialiser_t::end()
         << std::flush;

    EXPECT_EQ( sstr.str().size(), sizeof( double ) *  2);
    
    Eigen::Vector2d v_in;
    sstr >> tz::deserialiser_t::start()
         >> v_in
         >> tz::deserialiser_t::end();

    EXPECT_EQ( v_in( 0 ), 0.4 );
    EXPECT_EQ( v_in( 1 ), -0.5 );
    
}

TEST( binary_serialisation_eigen, vector3d )
{
    Eigen::Vector3d v;
    v << 0.4, -.5, std::numeric_limits< double >::infinity();

    std::stringstream sstr;

    EXPECT_EQ( v( 0 ), 0.4 );
    EXPECT_EQ( v( 1 ), -.5 );
    EXPECT_EQ( v( 2 ), std::numeric_limits< double >::infinity());
    
    sstr << tz::serialiser_t::start()
         << v
         << tz::serialiser_t::end()
         << std::flush;
    
    Eigen::Vector3d v_in;
    sstr >> tz::deserialiser_t::start()
         >> v_in
         >> tz::deserialiser_t::end();

    EXPECT_EQ( v_in( 0 ), 0.4 );
    EXPECT_EQ( v_in( 1 ), -0.5 );
    EXPECT_EQ( v_in( 2 ), std::numeric_limits< double >::infinity() );
    
}

// TEST( binary_serialisation_eigen, vector3d_array_wrapper )
// {
//     Eigen::Vector3d v;
//     v << 0.4, -.5, std::numeric_limits< double >::infinity();

//     std::stringstream sstr;

//     EXPECT_EQ( v( 0 ), 0.4 );
//     EXPECT_EQ( v( 1 ), -.5 );
//     EXPECT_EQ( v( 2 ), std::numeric_limits< double >::infinity());
    
//     sstr << tz::serialiser_t::start()
//          << v.transpose().array()
//          << tz::serialiser_t::end()
//          << std::flush;
    
//     Eigen::Vector3d v_in;
//     sstr >> tz::deserialiser_t::start()
//          >> v_in.transpose().array()
//          >> tz::deserialiser_t::end();

//     EXPECT_EQ( v_in( 0 ), 0.4 );
//     EXPECT_EQ( v_in( 1 ), -0.5 );
//     EXPECT_EQ( v_in( 2 ), std::numeric_limits< double >::infinity() );
    
// }

TEST( binary_serialisation_eigen, vectorXd )
{
    Eigen::VectorXd v(3);
    v << 0.4, -.5, std::numeric_limits< double >::infinity();

    std::stringstream sstr;

    EXPECT_EQ( v( 0 ), 0.4 );
    EXPECT_EQ( v( 1 ), -.5 );
    EXPECT_EQ( v( 2 ), std::numeric_limits< double >::infinity());
    
    sstr << tz::serialiser_t::start()
         << v
         << tz::serialiser_t::end()
         << std::flush;
    
    Eigen::VectorXd v_in;
    sstr >> tz::deserialiser_t::start()
         >> v_in
         >> tz::deserialiser_t::end();

    EXPECT_EQ( v_in( 0 ), 0.4 );
    EXPECT_EQ( v_in( 1 ), -0.5 );
    EXPECT_EQ( v_in( 2 ), std::numeric_limits< double >::infinity() );
}

TEST( binary_serialisation_eigen, matrixXd )
{
    Eigen::VectorXd v(3);
    v << 0.4, -.5, std::numeric_limits< double >::infinity();

    std::stringstream sstr;

    EXPECT_EQ( v( 0 ), 0.4 );
    EXPECT_EQ( v( 1 ), -.5 );
    EXPECT_EQ( v( 2 ), std::numeric_limits< double >::infinity());
    
    sstr << tz::serialiser_t::start()
         << v
         << tz::serialiser_t::end()
         << std::flush;
    
    Eigen::VectorXd v_in;
    sstr >> tz::deserialiser_t::start()
         >> v_in
         >> tz::deserialiser_t::end();

    EXPECT_EQ( v_in( 0 ), 0.4 );
    EXPECT_EQ( v_in( 1 ), -0.5 );
    EXPECT_EQ( v_in( 2 ), std::numeric_limits< double >::infinity() );
}

TEST( binary_serialisation_eigen, affine3d )
{
    Eigen::Affine3d m;
    m.matrix() <<
        0.4, -.5, std::numeric_limits< double >::infinity(), 8.0,
        1.0, 2.0, 3.0, 4.0,
        0.0, 1.0, 2.0, 9.0,
        3.0, 4.5, 6.0, 1.0;

    std::stringstream sstr;

    sstr << tz::serialiser_t::start()
         << m
         << tz::serialiser_t::end()
         << std::flush;
    
    Eigen::Affine3d m2;
    sstr >> tz::deserialiser_t::start()
         >> m2
         >> tz::deserialiser_t::end();

    EXPECT_EQ( m.matrix(), m2.matrix());
}

TEST( binary_serialisation_eigen, affine3f )
{
    Eigen::Affine3f m;
    m.matrix() <<
        0.4, -.5, std::numeric_limits< double >::infinity(), 8.0,
        1.0, 2.0, 3.0, 4.0,
        0.0, 1.0, 2.0, 9.0,
        3.0, 4.5, 6.0, 1.0;

    std::stringstream sstr;

    sstr << tz::serialiser_t::start()
         << m
         << tz::serialiser_t::end()
         << std::flush;
    
    Eigen::Affine3f m2;
    sstr >> tz::deserialiser_t::start()
         >> m2
         >> tz::deserialiser_t::end();

    EXPECT_EQ( m.matrix(), m2.matrix());
}

TEST( binary_serialisation_eigen, affine2d )
{
    Eigen::Affine2d m;
    m.matrix() <<
        0.4, -.5, std::numeric_limits< double >::infinity(),
        0.0, 1.0, 2.0,
        3.0, 4.5, 1.0;

    std::stringstream sstr;

    sstr << tz::serialiser_t::start()
         << m
         << tz::serialiser_t::end()
         << std::flush;
    
    Eigen::Affine2d m2;
    sstr >> tz::deserialiser_t::start()
         >> m2
         >> tz::deserialiser_t::end();

    EXPECT_EQ( m.matrix(), m2.matrix());
}

TEST( binary_serialisation_eigen, affine2f )
{
    Eigen::Affine2f m;
    m.matrix() <<
        0.4, -.5, std::numeric_limits< double >::infinity(),
        0.0, 1.0, 2.0,
        3.0, 4.5, 1.0;

    std::stringstream sstr;

    sstr << tz::serialiser_t::start()
         << m
         << tz::serialiser_t::end()
         << std::flush;
    
    Eigen::Affine2f m2;
    sstr >> tz::deserialiser_t::start()
         >> m2
         >> tz::deserialiser_t::end();

    EXPECT_EQ( m.matrix(), m2.matrix());
}
