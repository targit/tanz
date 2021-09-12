#include <gtest/gtest.h>
#include <tanz/time-measurement.h++>
#include <sstream>

TEST( binary_serialisation, task_duration )
{
    std::stringstream str;

    tz::task_duration_t d;
    d.start = ::tz::task_duration_t::duration_t( 123 );
    d.cpu_time = 23.0;
    d.real_time = 48.0;

    tz::serialiser_t ser{ & str };
    tz::deserialiser_t des{ & str };

    tz::task_duration_t d2;
    ser << d;
    des >> d2;

    EXPECT_EQ( d.start.count(), d2.start.count() );
    EXPECT_EQ( d.cpu_time, d2.cpu_time );
    EXPECT_EQ( d.real_time, d2.real_time );
}
