#include <gtest/gtest.h>
#include <tanz/union-find.h++>
#include <random>
#include <ciso646>

TEST( union_finder_t, empty )
{
    tz::union_finder_t uf( 0 );

    EXPECT_EQ( uf.classes().size(), 0 );
}

TEST( union_finder_t, one )
{
    {
        tz::union_finder_t uf( 1 );
        auto classes = uf.classes();
        EXPECT_EQ( classes.size(), 1 );
        EXPECT_EQ( classes[0].size(), 1 );
        EXPECT_EQ( classes[0][0], 0 );
    }
    {
        tz::union_finder_t uf( 1 );
        uf.add_relation( 0, 0 );
        auto classes = uf.classes();
        EXPECT_EQ( classes.size(), 1 );
        EXPECT_EQ( classes[0].size(), 1 );
        EXPECT_EQ( classes[0][0], 0 );
    }
    {
        tz::union_finder_t uf( 1 );
        uf.add_relation( 0, 0 );
        uf.add_relation( 0, 0 );
        auto classes = uf.classes();
        EXPECT_EQ( classes.size(), 1 );
        EXPECT_EQ( classes[0].size(), 1 );
        EXPECT_EQ( classes[0][0], 0 );
    }
}

TEST( union_finder_t, two )
{
    {
        tz::union_finder_t uf( 2 );
        auto classes = uf.classes();
        EXPECT_EQ( classes.size(), 2 );
        EXPECT_EQ( classes[0].size(), 1 );
        EXPECT_EQ( classes[1].size(), 1 );
        EXPECT_EQ( classes[0][0], 0 );
        EXPECT_EQ( classes[1][0], 1 );
    }

    {
        tz::union_finder_t uf( 2 );
        uf.add_relation( 0, 0 );
        uf.add_relation( 1, 1 );
        auto classes = uf.classes();
        EXPECT_EQ( classes.size(), 2 );
        EXPECT_EQ( classes[0].size(), 1 );
        EXPECT_EQ( classes[1].size(), 1 );
        EXPECT_EQ( classes[0][0], 0 );
        EXPECT_EQ( classes[1][0], 1 );

    }
    {
        tz::union_finder_t uf( 2 );
        uf.add_relation( 0, 0 );
        uf.add_relation( 1, 0 );
        auto classes = uf.classes();
        EXPECT_EQ( classes.size(), 1 );
        EXPECT_EQ( classes[0].size(), 2 );
        EXPECT_EQ( classes[0][0], 0 );
        EXPECT_EQ( classes[0][1], 1 );
    }
    {
        tz::union_finder_t uf( 2 );
        uf.add_relation( 0, 1 );
        auto classes = uf.classes();
        EXPECT_EQ( classes.size(), 1 );
        EXPECT_EQ( classes[0].size(), 2 );
        EXPECT_EQ( classes[0][0], 0 );
        EXPECT_EQ( classes[0][1], 1 );
    }
    {
        tz::union_finder_t uf( 2 );
        uf.add_relation( 0, 1 );
        auto classes = uf.classes();
        EXPECT_EQ( classes.size(), 1 );
        EXPECT_EQ( classes[0].size(), 2 );
        EXPECT_EQ( classes[0][0], 0 );
        EXPECT_EQ( classes[0][1], 1 );
    }
}

TEST( union_finder_t, stresstest_simple_one_class )
{
    tz::union_finder_t uf( 10000 );
    for( size_t k = 0; k + 1 < 10000; k = k + 1 ) {
        uf.add_relation( k, k + 1 );
    }
    EXPECT_EQ( uf.classes().size(), 1 );
}

TEST( union_finder_t, stresstest_simple_two_classes )
{
    tz::union_finder_t uf( 10000 );
    for( size_t k = 0; k + 2 < 10000; k = k + 1 ) {
        uf.add_relation( k, k + 2 );
    }
    EXPECT_EQ( uf.classes().size(), 2 );
}

TEST( union_finder_t, stresstest_simple_five_classes )
{
    tz::union_finder_t uf( 10000 );
    for( size_t k = 0; k + 5 < 10000; k = k + 1 ) {
        uf.add_relation( k, k + 5 );
    }
    EXPECT_EQ( uf.classes().size(), 5 );
}

TEST( union_finder_t, stresstest_simple_one_hundred_classes )
{
    tz::union_finder_t uf( 10000 );
    for( size_t k = 0; k + 100 < 10000; k = k + 1 ) {
        uf.add_relation( k, k + 100 );
    }
    EXPECT_EQ( uf.classes().size(), 100 );
}

TEST( union_finder_t, random_two )
{
    constexpr size_t l = 10000;
    constexpr size_t n = l/2;

    std::random_device init;

    std::vector< size_t > randomised_indices( l );
    for( size_t k = 0; k < l; k = k + 1 ) {
        randomised_indices[ k ] = k;
    }
    std::shuffle( randomised_indices.begin(),
                  randomised_indices.end(),
                  std::mt19937( init()) );

    std::vector< size_t > relations( randomised_indices );
    std::shuffle( relations.begin(),
                  relations.end(),
                  std::mt19937( init()) );

    tz::union_finder_t uf( l );
    for( size_t k = 0; k < l; k = k + 1 ) {
        auto relation = randomised_indices[ k ];
        if( ((relation + 1) < l) and (relation != n )) {
            uf.add_relation( relations[ relation ],
                             relations[ relation + 1] );
        }
    }
    EXPECT_EQ( uf.classes().size(), 2 );
}

TEST( union_finder_t, random_seventyeight )
{
    constexpr size_t l = 77000;

    std::random_device init;

    std::vector< size_t > randomised_indices( l );
    for( size_t k = 0; k < l; k = k + 1 ) {
        randomised_indices[ k ] = k;
    }
    std::shuffle( randomised_indices.begin(),
                  randomised_indices.end(),
                  std::mt19937( init()) );

    std::vector< size_t > relations( randomised_indices );
    std::shuffle( relations.begin(),
                  relations.end(),
                  std::mt19937( init()) );

    tz::union_finder_t uf( l );
    for( size_t k = 0; k < l; k = k + 1 ) {
        auto relation = randomised_indices[ k ];
        if( ((relation + 1) < l) and ((relation % 1000) != 0)) {
            uf.add_relation( relations[ relation ],
                             relations[ relation + 1] );
        }
    }
    EXPECT_EQ( uf.classes().size(), 78 );
}
