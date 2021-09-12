#include <gtest/gtest.h>
#include <tanz/hash-combiner.h++>

TEST( hashcombiner, u32)
{
    EXPECT_EQ( tz::hash_combine_u32( uint32_t( 123 ), uint32_t( 511 )),
               uint32_t(2654444269) );
}

TEST( hashcombiner, u64 )
{
    EXPECT_EQ( tz::hash_combine_u64( uint64_t( 123 ), uint64_t( 511 )),
               0x9e3779b97f4a9c89
               
               );
}
