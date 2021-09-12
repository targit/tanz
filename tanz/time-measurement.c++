#include <ctime>
#include <cerrno>
#include <system_error>
#include <tanz/time-measurement.h++>

#include <iostream>

namespace tz {

time_marker_t::time_marker_t()
    : physical( std::chrono::steady_clock::now())
{
#if __unix__
    struct timespec ts;
    if( clock_gettime( CLOCK_THREAD_CPUTIME_ID, &ts ) < 0 ) {
        throw std::system_error(
            std::error_code(
                errno,
                std::system_category()));
    }
    cycles = ts.tv_sec * uint64_t(1000000000) + ts.tv_nsec;
#else
#  error CPU time clock not implemented for this platform
#endif
}

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, ::tz::task_duration_t const & v )
{
    return out
        << v.start
        << v.real_time
        << v.cpu_time;
}

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, ::tz::task_duration_t  & v )
{
    return
        in >> v.start
           >> v.real_time
           >> v.cpu_time;
}


task_duration_t
task_duration_t::operator + ( task_duration_t const & b ) const
{
    return task_duration_t{ start, real_time + b.real_time, cpu_time + b.cpu_time };
}


task_duration_t
time_marker_t::operator - (time_marker_t const & earlier ) const
{
    task_duration_t res;
    res.start = earlier.physical.time_since_epoch();
    res.real_time =
        1.0E-9f * std::chrono::duration_cast< std::chrono::nanoseconds >( physical - earlier.physical ).count();
    res.cpu_time =
        (1.0E-9f * (cycles - earlier.cycles ));
    return res;
}
}
