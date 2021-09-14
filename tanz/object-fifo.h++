#pragma once
#include <cstddef>
#ifndef FILE_7945C9A7B39CCE1_1EB13B619F4B2875_INCLUDED
#define FILE_7945C9A7B39CCE1_1EB13B619F4B2875_INCLUDED

#include <condition_variable>
#include <deque>
#include <cassert>
#include <memory>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <optional>
#include <atomic>

namespace tz {

struct fifo_is_closed : public ::std::exception {
};

struct fifo_max_size_must_be_positive
    : public ::std::domain_error
{
    fifo_max_size_must_be_positive()
        : ::std::domain_error( "Fifo max size must be positive" )
    {
    };
};

template<typename T>
struct object_fifo
{
    /* TODO:
     * - Add move versions for writing and reading.
     * - Reduce number of locking operations at runtime,
     *   there is some monotonicty to be taken use of.
     */
private:
    using mutex_t = ::std::mutex;
    using lock_t  = ::std::unique_lock< mutex_t>;
    using condition_variable_t = ::std::condition_variable;

    bool  marked_as_closed;
    size_t        max_size;
    mutex_t       mtx;
    std::deque<T> queue;
    condition_variable_t input_state_change;

    bool full_unlocked() const
    {
        return queue.size() >= max_size;
    }

    void reserve_space(){
        while ( full_unlocked() ) {
            queue.pop_back();
        }
    }

public:
    using value_type = T;

    object_fifo( size_t max_size_ )
        : marked_as_closed( false ), max_size( max_size_ )
    {
        if ( max_size < 1 ) {
            throw fifo_max_size_must_be_positive();
        }
    }

    object_fifo( )
        : marked_as_closed( false ),
          max_size( -1 )
    {
    }

    void close( )
    {
        lock_t lock( mtx );
        marked_as_closed = true;
        input_state_change.notify_all();
    }

    /** With multiple writers there's no guarantee about the order. */
    void write_overwrite( value_type const& x )
    {
        lock_t lock( mtx );

        assert( queue.size() <= max_size );

        if ( marked_as_closed ) {
            throw fifo_is_closed();
        }
        reserve_space(); // cannot fail
        queue.push_back( x );

        input_state_change.notify_all();
    }

    void write_blockingly( value_type const& x )
    {
        lock_t lock( mtx );

    try_with_mutex_still_locked:
        assert( queue.size() <= max_size );

        if ( marked_as_closed ) {
            throw fifo_is_closed();
        } else if ( queue.size() == max_size ) {
            input_state_change.wait( lock );
            goto try_with_mutex_still_locked;
        } else {
            queue.push_back( x );
            input_state_change.notify_all();
        }
    }

    value_type read_blockingly( )
    {
        lock_t lock( mtx );

    try_with_mutex_still_locked:

        if ( queue.empty() ) {
            if ( marked_as_closed ) {
                throw fifo_is_closed();
            } else {
                input_state_change.wait( lock );
                goto try_with_mutex_still_locked;
            }
        } else {
            value_type x(queue.front());
            queue.pop_front();
            input_state_change.notify_all();
            return x;
        }
    }

    template< typename Clock, typename Duration >
    std::optional< value_type >
    read_timeout( std::chrono::time_point< Clock, Duration > const &time_max )
    {
        lock_t lock( mtx );

    try_with_mutex_still_locked:

        if ( queue.empty() ) {
            if ( marked_as_closed ) {
                throw fifo_is_closed();
            } else {
                auto code = input_state_change.wait_until( lock, time_max );
                if ( code == ::std::cv_status::timeout ) {
                    if( marked_as_closed ) {
                        throw fifo_is_closed();
                    } else {
                        return {};
                    }
                } else {
                    goto try_with_mutex_still_locked;
                }
            }
        } else {
            value_type x(queue.front());
            queue.pop_front();
            input_state_change.notify_all();
            return x;
        }
    }

    template< typename Rep, typename Rat >
    std::optional< value_type >
    read_timeout( ::std::chrono::duration< Rep, Rat > const & timeout )
    {
        return read_timeout( ::std::chrono::steady_clock::now() + timeout);
    }

    std::optional< value_type >
    read_timeout( double len )
    {
        return read_timeout(::std::chrono::duration< double, std::ratio< 1 > >( len ));
    }

    size_t size()
    {
        lock_t lock( mtx );
        return queue.size();
    }

    bool closed()
    {
        lock_t lock( mtx );
        return marked_as_closed;
    }

    bool empty()
    {
        lock_t lock( mtx );
        return queue.empty();
    }

    bool full()
    {
        lock_t lock( mtx );
        return full_unlocked();
    }
};

template< typename T >
struct fifo_guard
{
    using fifo_type = object_fifo< T >;
    using fifo_ptr  = ::std::shared_ptr< fifo_type >;

    fifo_ptr fifo;

    fifo_guard()
    {
    }

    fifo_guard( fifo_ptr fifo_ )
        : fifo( fifo_ )
    {
    }

    void
    close()
    {
        if( fifo ) {
            fifo->close();
        }
        // but still keep the pointer
    }

    size_t
    size()
    {
        if( not fifo ) {
            throw std::domain_error( "no fifo to operate on" );
        }

        return fifo->size();
    }

    void release()
    {
        fifo.reset();
    }

    void adopt( fifo_guard && b )
    {
        close();
        fifo.swap( b.fifo );
    }

    operator bool () const {
        return not not fifo;
    }

    fifo_guard & operator = (fifo_guard const & ) = delete;
    fifo_guard & operator = (fifo_guard && g2 )
    {
        adopt( std::move( g2 ));
    }

    void participate( fifo_guard const & b )
    {
        close();
        fifo = b.fifo;
    }

    ~fifo_guard()
    {
        close();
    }
};

template< typename T > struct fifo_guard_writing;

template< typename T >
struct fifo_guard_reading {
    using fifo_type = object_fifo< T >;
    using value_type = typename fifo_type::value_type;
    using ptr_type = ::std::shared_ptr< fifo_type >;

    ptr_type fifo; /* think before you touch this! */

    fifo_guard_reading( ptr_type const &ptr )
        : fifo( ptr )
    {
    }

    fifo_guard_reading() = default;

    ~fifo_guard_reading()
    {
        close();
    }

    fifo_guard_reading( fifo_guard_reading const & ) = delete;

    fifo_guard_reading& operator = (fifo_guard_reading const &) = delete;

    fifo_guard_reading( fifo_guard_reading && b ) = default;

    fifo_guard_reading & operator = ( fifo_guard_reading && b )
    {
        close();
        fifo = ::std::move( b.fifo );
    }

    operator bool () const {
        return not not fifo;
    }

    fifo_guard< T >
    make_guard()
    {
        return fifo_guard< T >( fifo );
    }

    fifo_guard_writing< T >
    make_guard_writing()
    {
        return fifo_guard_writing< T >( fifo );
    }

    void close( )
    {
        if( fifo ) {
            fifo->close();
        }
    }

    value_type read_blockingly( )
    {
        return fifo->read_blockingly();
    }

    bool is_closed()
    {
        return fifo->closed();
    }

    bool is_empty()
    {
        return fifo->empty();
    }

    size_t
    size()
    {
        return fifo->size();
    }
};

template< typename T >
struct fifo_guard_writing {
    using fifo_type = object_fifo< T >;
    using value_type = typename fifo_type::value_type;
    using ptr_type = ::std::shared_ptr< fifo_type >;

    ptr_type fifo; /* think before you touch this! */

    fifo_guard_writing( ptr_type const &ptr )
        : fifo( ptr )
    {
    }

    fifo_guard_writing()
    {}

    ~fifo_guard_writing()
    {
        close();
    }

    fifo_guard_writing( fifo_guard_writing const & ) = delete;
    fifo_guard_writing& operator = (fifo_guard_writing const &) = delete;

    fifo_guard_writing( fifo_guard_writing && b ) = default;
    fifo_guard_writing & operator = ( fifo_guard_writing && b )
    {
        close();
        fifo = ::std::move( b.fifo );
        return *this;
    }

    void close( )
    {
        if( fifo ) {
            fifo->close();
        }
    }

    fifo_guard< T >
    make_guard()
    {
        return fifo_guard< T >( fifo );
    }

    fifo_guard_reading< T >
    make_guard_reading()
    {
        return fifo_guard_reading< T >( fifo );
    }

    void write_overwrite( value_type const& x )
    {
        fifo->write_overwrite( x );
    }

    void write_blockingly( value_type const& x )
    {
        fifo->write_blockingly( x );
    }

    operator bool () const {
        return not not fifo;
    }

    bool closed() const
    {
        return fifo->closed();
    }

    bool full() const
    {
        return fifo->full();
    }
};
}
#endif
