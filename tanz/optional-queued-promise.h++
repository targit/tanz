#pragma once
#include <chrono>
#include <initializer_list>
#ifndef FILE_199994D0FCFC6EF2_156EC7C79B5A347B_INCLUDED
#define FILE_199994D0FCFC6EF2_156EC7C79B5A347B_INCLUDED

#include <future>
#include <optional>
#include <variant>
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

    struct cancellation_task_t {};

    std::optional< T > value;
    std::optional< std::future< std::optional< T > > > pending;
    std::optional< std::variant< cancellation_task_t, std::function< std::optional< T >() >>> queued;

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

        if( queued  ) {
            if( not pending ) {
                if( queued->index() == 1) {
                    LOG_OQP( "SCHEDULED" );
                    auto fn = std::move( std::get<1>(*queued) );
                    pending = std::move(
                        ::std::async(
                            ::std::launch::async,
                            [fn]()
                            -> std::optional< T >
                            {
                                try {
                                    return fn();
                                } catch ( ... ) {
                                    return {};
                                }
                            }));
                    queued = {};
                } else {
                    queued = {};
                }
            }
        }

        if( pending ) {
            if( (*pending).wait_for( std::chrono::milliseconds(0)) == std::future_status::ready) {
                if( not queued ) {
                    LOG_OQP( "PENDING -> SET" );
                    value = std::move( (*pending).get() );
                    pending = {};
                    return 1;
                } else if( queued ) {
                    LOG_OQP( "READY, BUT CANCELLED" );
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
        else if( queued and queued->index() == 0 ) {
            return UNSET;
        }
        else if( pending or (queued and queued->index() == 1)) {
            return PENDING;
        }
        else {
            return UNSET;
        }
    }

    operator bool () const
    /* Does not update.  I think, it should update.  Update is
       monotone.  However, in this case the operation would not be
       'const', another important property.  I could nevertheless
       'declare' update to be const, but have mixed feelings about
       it.
    */
    {
        return (state() == SET);
    }

    T const &
    operator * () const
    {
        return *value;
    }

    operator bool ()
    /* Having both the const and non-const version is asking for nasty
       surprises. */
    {
        update();
        return (state() == SET );
    }

    optional_threaded_t &
    operator = ( std::initializer_list<int> l )
    {
        if( l.size() == 0 ) {
            if( pending ) {
                if( value ) {
                    throw std::logic_error( "optional_threaded_t::operator = {}: Pending while holding a value." );
                }
                LOG_OQP( "MARK CANCELLATION" );
                queued = cancellation_task_t{};
            }
            else if( value ) {
                LOG_OQP( "RESET OLD VALUE" );
                value = {};
            }
        } else {
            throw std::domain_error( "optional_threaded_t::operator = {}: Initializer list too long." );
        }
        return *this;
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
            if( queued.index() == 1 ) {
                LOG_OQP( "QUEUED OVERWRITING" );
            }
        } else {
            LOG_OQP( "QUEUED" );
        }
        queued = f;
        update();
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
            if( queued->index() == 1 ) {
                LOG_OQP( "QUEUED OVERWRITING" );
            }
        } else {
            LOG_OQP( "QUEUED" );
        }
        queued = std::move( f );
        update();
        return *this;
    }

    T const&
    operator * () {
        update();
        return *value;
    }

    template <typename F>
    optional_threaded_t &
    operator = ( std::optional< F > const & x )
    {
        if( not  x ) {
            (*this) = {};
        } else {
            (*this) = *x;
        }
        return *this;
    }
};
}
#endif
