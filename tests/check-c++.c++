#include <array>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <map>
#include <gtest/gtest.h>

TEST( array, sizeof )
{
    {
        std::array< uint64_t, 5 > a;
        EXPECT_EQ( sizeof( uint64_t ) * 5, sizeof( a ));
    }
    {
        std::array< char, 5 > a;
        EXPECT_EQ( sizeof( char ) * 5, sizeof( a ));
    }
}

TEST( istream, operator_order )
{
    {
        std::stringstream in( "1 2" );
        int a, b;

        in >> a >> b;
        EXPECT_EQ( a, 1 );
        EXPECT_EQ( b, 2 );
    }
    {
        std::stringstream str;
        str << 1 << " " << 2;
        int a, b;
        str >> a >> b;
        EXPECT_EQ( a, 1 );
        EXPECT_EQ( b, 2 );
    }
}

TEST( unordered_map, find_empty )
{
    {
        std::map< int, int > map;
        EXPECT_FALSE( map.find( 0 ) != map.end());
    }
    {
        std::map< int, int > map;
        map.insert( { 1, 1 } );
        EXPECT_TRUE( map.find( 1 ) != map.end());
        EXPECT_FALSE( map.find( 0 ) != map.end());
    }
    {
        std::map< int, int > map;
        map[1] = 0;
        EXPECT_TRUE( map.find( 1 ) != map.end());
        EXPECT_FALSE( map.find( 0 ) != map.end());
    }
    {
        std::map< int, std::vector< size_t > > map;
        EXPECT_FALSE( map.find( 0 ) != map.end());
    }
    {
        std::map< int, std::vector< size_t > > map;
        map.insert( { 1, { 1 } } );
        EXPECT_TRUE( map.find( 1 ) != map.end());
        EXPECT_FALSE( map.find( 0 ) != map.end());
    }
    {
        std::map< int, std::vector< size_t > > map;
        map[1].push_back( 0 );
        EXPECT_TRUE( map.find( 1 ) != map.end());
        [&map](){
            EXPECT_FALSE( map.find( 0 ) != map.end());
        }();
    }
}
