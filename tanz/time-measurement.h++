#pragma once
#ifndef FILE_19321FB8AD3324EE_E2B815541996311_INCLUDED
#define FILE_19321FB8AD3324EE_E2B815541996311_INCLUDED
#include <chrono>
#include <tanz/binary-serialisation.h++>

namespace tz {

struct task_duration_t {
    using duration_t = ::std::chrono::duration< int64_t, ::std::nano >;
    /* So it is serialisable */
    
    duration_t start;
    float real_time = 0.0;
    float cpu_time = 0.0;

    task_duration_t
    operator + ( task_duration_t const & b ) const;

    bool
    operator == ( task_duration_t const & ) const = default;
};

::tz::serialiser_t &
operator << ( ::tz::serialiser_t & out, ::tz::task_duration_t const & v );

::tz::deserialiser_t &
operator >> ( ::tz::deserialiser_t & in, ::tz::task_duration_t  & v );

struct time_marker_t {
    using physical_time_t = std::chrono::time_point<std::chrono::steady_clock>;
    using cycle_time_t = uint64_t;

    physical_time_t physical;
    cycle_time_t    cycles;

    time_marker_t();
    task_duration_t operator - (time_marker_t const & earlier ) const;
};
}

#endif
