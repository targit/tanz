#pragma once
#include <chrono>
#ifndef FILE_199994D0FCFC6EF2_156EC7C79B5A347B_INCLUDED
#define FILE_199994D0FCFC6EF2_156EC7C79B5A347B_INCLUDED

#include <future>
#include <optional>
#include <stdexcept>

#if SHOW_OQP_LOG
#   include <iostream>
#   define LOG_OQP( MESG ) \
    std::cerr << MESG << "\n";
#else
#   define LOG_OQP( MESG )
#endif

namespace tz {

template< typename T >
struct optional_threaded_t {
    enum state_t {
        UNSET,
        PENDING,
        SET
    };

    std::optional< T > value;
    std::optional< std::future< T > > pending;
    std::optional< std::function< T() >> queued;

    int
    update()
    /* Never blocks */
    {
        if( value and (pending or queued)) {
            throw std::logic_error( "SET and PENDING at the same time." );
        }

        if( value ) {
            return 0;
        }

        if( queued ) {
            if( not pending ) {
                LOG_OQP( "SCHEDULED" );
                pending = std::move( ::std::async( ::std::launch::async, *queued ));
                queued = {};
            }
        }

        if( pending ) {
            if( (*pending).wait_for( std::chrono::milliseconds(0)) == std::future_status::ready) {
                LOG_OQP( "READY" );
                if( not queued ) {
                    value = std::move( (*pending).get() );
                    pending = {};
                    return 1;
                } else if( queued ) {
                    LOG_OQP( "CANCELLED" );
                    /* Ignore this result */
                    pending = {};
                    return update();
                }
            }
        }
        return 0;
    }

    state_t
    state() const
    {
        if( value ) {
            return SET;
        }
        else if( pending or queued ) {
            return PENDING;
        }
        else {
            return UNSET;
        }
    }

    template <typename F>
    optional_threaded_t &
    operator = ( F const & f )
    /* As always: After setting a 'global' update needs to be done */
    {
        if( value ) {
            throw std::logic_error( "A new value can only be scheduled in the UNSET state." );
        }
        if( queued ) {
            LOG_OQP( "QUEUED OVERWRITING" );
        } else {
            LOG_OQP( "QUEUED" );
        }
        queued = f;
        return *this;
    }

    template <typename F>
    optional_threaded_t &
    operator = ( F && f )
    /* As always: After setting a 'global' update needs to be done */
    {
        if( value ) {
            throw std::logic_error( "A new value can only be scheduled in the UNSET state." );
        }
        if( queued ) {
            LOG_OQP( "QUEUED OVERWRITING" );
        } else {
            LOG_OQP( "QUEUED" );
        }
        queued = std::move( f );
        return *this;
    }

    T const&
    operator * () {
        update();
        return *value;
    }
};
}
#endif
