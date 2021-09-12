#include <gtest/gtest.h>
#include <tanz/binary-serialisation.h++>
#include <sstream>


TEST( serialisation, primitives )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        uint16_t x( 1 );
        ser << x;
        EXPECT_EQ( str.str().size(), sizeof( x ));
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        uint32_t x( 1 );
        ser << x;
        EXPECT_EQ( str.str().size(), sizeof( x ));
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        uint64_t x( 1 );
        ser << x;
        EXPECT_EQ( str.str().size(), sizeof( x ));
    }
}

TEST( serialisation, primitive_roundtrips )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype =
            std::tuple< uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t, int64_t, float, double >;

        ttype x_out={1,2,3,4,-1,-2,-3,-4, 0.23, 0.532443 };
        ttype x_in;
        ser << x_out;        
        deser >> x_in;

        EXPECT_EQ( std::get< 0 >( x_out ), std::get< 0 >( x_in ) );
        EXPECT_EQ( std::get< 1 >( x_out ), std::get< 1 >( x_in ) );
        EXPECT_EQ( std::get< 2 >( x_out ), std::get< 2 >( x_in ) );
        EXPECT_EQ( std::get< 3 >( x_out ), std::get< 3 >( x_in ) );
        EXPECT_EQ( std::get< 4 >( x_out ), std::get< 4 >( x_in ) );
        EXPECT_EQ( std::get< 5 >( x_out ), std::get< 5 >( x_in ) );
        EXPECT_EQ( std::get< 6 >( x_out ), std::get< 6 >( x_in ) );
        EXPECT_EQ( std::get< 7 >( x_out ), std::get< 7 >( x_in ) );
        EXPECT_EQ( std::get< 8 >( x_out ), std::get< 8 >( x_in ) );
        EXPECT_EQ( std::get< 9 >( x_out ), std::get< 9 >( x_in ) );

    }
}

TEST( serialisation, array )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::vector< std::array< double, 0 > >;

        ttype x_out;
        ttype x_in;
        ser << x_out; 
        deser >> x_in;

        EXPECT_EQ( x_out, x_in );
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::array< double, 1 >;

        ttype x_out = { 23.0 };
        ttype x_in;
        ser << x_out; 
        deser >> x_in;

        EXPECT_EQ( x_out, x_in );
        EXPECT_EQ( x_out[0], 23.0 );
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::array< double, 8 >;

        ttype x_out = { 1, 2, 3, 4, 5, 6, 7, 8 };
        ttype x_in;
        ser << x_out; 
        deser >> x_in;

        EXPECT_EQ( x_out, x_in );
        EXPECT_EQ( x_out[0], 1.0 );
        EXPECT_EQ( x_out[1], 2.0 );
        EXPECT_EQ( x_out[2], 3.0 );
        EXPECT_EQ( x_out[3], 4.0 );
        EXPECT_EQ( x_out[4], 5.0 );
        EXPECT_EQ( x_out[5], 6.0 );
        EXPECT_EQ( x_out[6], 7.0 );
        EXPECT_EQ( x_out[7], 8.0 );
    }
}

TEST( serialisation, complex )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::complex< float >;

        ttype x_out( 0, 1 );
        ttype x_in;
        ser << x_out; 
        deser >> x_in;

        EXPECT_EQ( x_out.real(), x_in.real() );
        EXPECT_EQ( x_out.imag(), x_in.imag() );
        EXPECT_EQ( x_out, x_in );
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::complex< double >;

        ttype x_out( 0, 1 );
        ttype x_in;
        ser << x_out; 
        deser >> x_in;

        EXPECT_EQ( x_out.real(), x_in.real() );
        EXPECT_EQ( x_out.imag(), x_in.imag() );
        EXPECT_EQ( x_out, x_in );
    }
}

TEST( serialisation, map )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::map< double, int >;

        ttype x_out = {{0.0, 1},{2.0,3}};
        ttype x_in;
        ser << x_out; 
        deser >> x_in;

        EXPECT_EQ( x_out.size(), x_in.size() );
        EXPECT_EQ( x_out, x_in );
        EXPECT_EQ( x_out[2.0], 3 );
    }
}

TEST( serialisation, optional )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::optional< int >;
        ttype x_out = {};
        ttype x_in;
        ser << x_out; 
        deser >> x_in;
        EXPECT_EQ( str.str().size(), sizeof( uint8_t ));
        EXPECT_EQ( x_out, x_in );
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::optional< int >; // std::variant< std::monostate, int >
        ttype x_out = 1;
        ttype x_in;
        ser << x_out; 
        deser >> x_in;
        EXPECT_EQ( str.str().size(), sizeof( uint8_t ) + sizeof( int ));
        EXPECT_EQ( x_out, x_in );
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::optional< std::variant< std::monostate, int > >;
        ttype x_out = {};
        ttype x_in;
        ser << x_out; 
        deser >> x_in;
        EXPECT_EQ( str.str().size(), sizeof( uint8_t ));
        EXPECT_EQ( x_out, x_in );
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::optional< std::variant< std::monostate, int > >;
        ttype x_out = {{ }};
        ttype x_in;
        ser << x_out; 
        deser >> x_in;
        EXPECT_EQ( str.str().size(), sizeof( uint8_t ) + sizeof( uint64_t ));
        EXPECT_EQ( x_out, x_in );
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::optional< std::variant< std::monostate, int > >;
        ttype x_out = { { 1 }};
        ttype x_in;
        ser << x_out; 
        deser >> x_in;
        EXPECT_EQ( str.str().size(), sizeof( uint8_t ) + sizeof( uint64_t ) + sizeof( int ));
        EXPECT_EQ( x_out, x_in );
    }
}

TEST( serialisation, pair )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::pair< double, float >;

        ttype x_out = {-1, -2};
        ttype x_in;
        ser << x_out; 
        deser >> x_in;

        EXPECT_EQ( x_out, x_in );
    }
}

TEST( serialisation, variant )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        
        auto a = std::variant< std::monostate, float >();
        ser << a;
        EXPECT_EQ( str.str().size(), sizeof( uint64_t ));
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        
        auto a = std::variant< std::monostate, float >();
        a = 34.0f;
        ser << a;
        EXPECT_EQ( str.str().size(), sizeof( uint64_t ) + sizeof( float ));
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        
        auto a = std::variant< std::monostate, float >();
        std::variant< std::monostate, float > b;
        ser << a;
        
        deser >> b;
        
        EXPECT_EQ( str.str().size(), sizeof( uint64_t ));
        EXPECT_EQ( b.index(), 0 );
    }
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        
        auto a = std::variant< std::monostate, float >();
        std::variant< std::monostate, float > b;
        a = 34.0f;
        ser << a;
        
        deser >> b;
        
        EXPECT_EQ( str.str().size(), sizeof( uint64_t ) + sizeof( float ));
        EXPECT_EQ( b.index(), 1 );
        EXPECT_EQ( std::get< 1 >( b ), 34.0f );
    }
}

TEST( serialisation, vector )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::vector< std::vector< double > >;

        ttype x_out = {{0, 1},{2,3},{}};
        ttype x_in;
        ser << x_out; 
        deser >> x_in;

        EXPECT_EQ( x_out.size(), x_in.size() );
        EXPECT_EQ( x_in[0].size(), 2 );
        EXPECT_EQ( x_in[1].size(), 2 );
        EXPECT_EQ( x_in[2].size(), 0 );
        EXPECT_EQ( x_in[0][0], 0.0 );
        EXPECT_EQ( x_in[0][1], 1.0 );
        EXPECT_EQ( x_in[1][0], 2.0 );
        EXPECT_EQ( x_in[1][1], 3.0 );
    }
}

TEST( serialisation, unordered_map )
{
    {
        std::stringstream str;
        tz::serialiser_t ser{ &str };
        tz::deserialiser_t deser{ &str };
        using ttype = std::unordered_map< double, int >;

        ttype x_out = {{0.0, 1},{2.0,3}};
        ttype x_in;
        ser << x_out; 
        deser >> x_in;

        EXPECT_EQ( x_out.size(), x_in.size() );
        EXPECT_EQ( x_out, x_in );
        EXPECT_EQ( x_out[2.0], 3 );
    }
}

TEST( serialisation, start_and_stop )
{
    {
        std::stringstream str;

        using ttype = std::unordered_map< double, int >;
        
        ttype x1_out = {{0.0, 1},{2.0,3}};
        double x2_out = 1.234;

        ttype x1_in;
        double x2_in;
        
        str << tz::serialiser_t::start() << x1_out << x2_out << tz::serialiser_t::end() << std::flush;
        str >> tz::deserialiser_t::start() >> x1_in >> x2_in >> tz::deserialiser_t::end();
        
        EXPECT_EQ( x1_out.size(), x1_in.size() );
        EXPECT_EQ( x1_out, x1_in );
        EXPECT_EQ( x1_out[2.0], 3 );
        EXPECT_EQ( x2_out, x2_in );
        EXPECT_EQ( x2_in, 1.234 );
    }
}
