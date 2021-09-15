#include <gtest/gtest.h>
#include <tanz/time-measurement.h++>
#include <sstream>

TEST( binary_serialisation, task_duration )
{
    std::stringstream str;

    tz::task_duration_t d;
    d.start = ::tz::task_duration_t::duration_t( 123 );
    d.real_time = 48.0;
    d.cpu_time = 23.0;

    tz::serialiser_t ser{ & str };
    tz::deserialiser_t des{ & str };

    tz::task_duration_t d2;
    ser << d;
    des >> d2;

    EXPECT_EQ( d.start.count(), d2.start.count() );
    EXPECT_EQ( d.cpu_time, d2.cpu_time );
    EXPECT_EQ( d.real_time, d2.real_time );
}

TEST( binary_serialisation, opt_task_duration )
{
    std::stringstream str;

    std::optional< tz::task_duration_t > d;
    d = {{ ::tz::task_duration_t::duration_t( 123 ),
          23.0,
          48.0 }};

    tz::serialiser_t ser{ & str };
    tz::deserialiser_t des{ & str };

    std::optional< tz::task_duration_t > d2;
    ser << d;
    des >> d2;

    EXPECT_EQ( d2->cpu_time, 48.0 );
    EXPECT_EQ( d->start.count(), d2->start.count() );
    EXPECT_EQ( d->cpu_time, d2->cpu_time );
    EXPECT_EQ( d->real_time, d2->real_time );
}
