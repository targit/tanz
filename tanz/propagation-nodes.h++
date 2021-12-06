/* Tara Lorenz (c) 2021, License: AGPLv3 or Apache License 2.0 */
#pragma once
#ifndef FILE_609816DD_12F6839E956CDCE1_INCLUDED
#define FILE_609816DD_12F6839E956CDCE1_INCLUDED
#include <optional>
#include <memory>
#include <tuple>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <functional>

#include <tanz/propagation-nodes.h++>

namespace tz {

template< typename... argument_types >
struct connection_table_t
{
    struct ptr_hasher_t {
        size_t
        operator() (std::weak_ptr< void > const & p )
            const
        {
            auto sp = p.lock();
            auto h = ::std::hash< size_t >()( (size_t)(sp.get()) );
            return h;
        }
    };

    struct ptr_equal_t {
        bool
        operator() ( std::weak_ptr< void > const & a, std::weak_ptr< void > const & b )
            const
        {
            auto sa = a.lock();
            auto sb = b.lock();
            return sa == sb;
        }
    };

    using propagation_function = std::function< void( argument_types const & ...) >;
    using key_type = std::weak_ptr< void >;
    using map_type =
        std::unordered_map< key_type, propagation_function, ptr_hasher_t, ptr_equal_t >;

    map_type object_to_callback;

    void
    cleanup()
    {
        std::erase_if( object_to_callback,
                       []( auto x ) {
                           return x.first.expired();
                       });
    }

    template< typename T >
    void
    add_callback( std::shared_ptr< T >  callee, propagation_function const & callback )
    {
        key_type key( std::reinterpret_pointer_cast< void >( callee ));

        if( object_to_callback.find( key ) != object_to_callback.end()) {
            throw std::logic_error( "Object is connected already." );
        }

        object_to_callback[ key ] = callback;
        cleanup(); // TODO: How to do this systematically?
    }

    template< typename T >
    void
    remove_callback( std::shared_ptr< T > callee )
    {
        object_to_callback.erase( callee );
    }

    void
    operator() ( argument_types const & ... arg )
    {
        for( auto k : object_to_callback ) {
            auto p = k.first.lock();
            if( p ) {
                k.second( arg... );
            }
        }
        cleanup(); // TODO: How to do this systematically?
    }
};

template< typename... T_args >
struct data_node_t
{
    using data_type = std::tuple< ::std::optional< T_args >... >;
    using optional_data_type = std::optional< data_type >;
    using connection_table_t = ::tz::connection_table_t< optional_data_type >;

    using propagation_type = std::function< void( optional_data_type const & x) >;
    /* Why do we wrap this into an optional? Well, there is a
       difference, if some (including all) slots are unknown, or there
       is no result at all, indicating failure.  I do not know if we
       are really gonna need this distinction in our simple
       application. */

    data_type all_data;
    connection_table_t connections;

    data_node_t() = default;
    data_node_t( data_node_t const & ) = delete;
    data_node_t & operator = ( data_node_t const & ) = delete;
    data_node_t & operator = ( data_node_t && ) = delete;

    ~data_node_t()
    {
        all_data = {};
        notify();
    }

    data_node_t const & input() const
    {
        return *this;
    }

    data_node_t & input()
    {
        return *this;
    }

    data_node_t const & output() const
    {
        return *this;
    }

    data_node_t & output()
    {
        return *this;
    }

    constexpr
    data_type const &
    data() const
    {
        return all_data;
    }

    constexpr
    data_type &
    data()
    {
        return all_data;
    }

    template< size_t idx >
    void
    set_only_direct( typename std::remove_reference<decltype( std::get< idx >( all_data ))>::type::value_type const & v )
    {
        std::get< idx >( data() ) = v;
    }

    template< size_t idx >
    void
    set_only_optional( typename std::remove_reference<decltype( std::get< idx >( all_data ))>::type const & v )
    {
        std::get< idx >( data() ) = v;
    }

    template< size_t idx >
    void
    set_slot_direct( typename std::remove_reference<decltype( std::get< idx >( all_data ))>::type::value_type const & v )
    {
        clear_successors();
        set_only_direct< idx >( v );
        notify();
    }

    template< size_t idx >
    void
    set_slot_optional( typename std::remove_reference<decltype( std::get< idx >( all_data ))>::type const & v )
    {
        clear_successors();
        set_only_optional< idx >( v );
        notify();
    }

    template< size_t idx >
    typename std::remove_reference<decltype( std::get< idx >( all_data ))>::type const &
    ref_optional() const
    {
        return std::get< idx >( data() );
    }

    template< size_t idx >
    typename std::remove_reference<decltype( std::get< idx >( all_data ))>::type::value_type const &
    ref_direct() const
    {
        return *(std::get< idx >( data() ));
    }

    template< size_t... k >
    bool
    internal_check_all( std::integer_sequence< size_t, k...> ) const
    {
        return ( ref_optional< k >() and ... and true);
    }

    bool all_set_p() const
    {
        return internal_check_all( std::make_integer_sequence< size_t, sizeof...(T_args)>());
    }

    template< size_t ... k >
    void
    notify_impl( std::integer_sequence< size_t, k ... >)
    {
        connections( data() );
    }

    void
    clear_successors()
    {
        connections( {} );
    }

    void
    notify( bool success = true )
    {
        if( success ) {
            notify_impl( std::make_integer_sequence< size_t, sizeof ... (T_args) >());
        }
    }

    template< size_t ... k >
    void
    set_all_impl( std::integer_sequence< size_t, k ... >, T_args const & ... args )
    {
        ( ( std::get< k >( data() ) = args, 0), ... );
    }

    typename connection_table_t::propagation_function
    adapt_propagator( typename connection_table_t::propagation_function & fn )
    {
        return fn;
    }

    typename connection_table_t::propagation_function
    adapt_propagator( std::function< void( data_type ) > const & fn,
                      std::function<void()> const & failure_cont = [](){})
    {
        return
            [fn, failure_cont]( optional_data_type const & arg ) -> void
            {
                if( arg ) {
                    fn(*arg);
                } else {
                    failure_cont();
                }
            };
    }

    typename connection_table_t::propagation_function
    adapt_propagator( std::function< void( std::optional< T_args > const & ... )> const & fn,
                      std::function<void()> const & failure_cont = [](){})
    {
        return
            [fn, failure_cont]( optional_data_type const & arg ) -> void
            {
                if( arg ) {
                    std::apply( fn, *arg );
                } else {
                    failure_cont();
                }
            };
    }

    // typename connection_table_t::propagation_function
    // adapt_propagator( std::function< void(  T_args  const & ... )> const & fn)
    // /* Beware: This adaptor will only propagate in case of full
    //    success */
    // {
    //     return
    //         [fn]( optional_data_type const & arg ) -> void
    //         {
    //             if( std::all_of(
    //             std::apply( fn, *arg );
    //         };
    // }

    void
    operator() ( T_args const & ... args )
    {
        clear_successors();
        set_all_impl( std::make_integer_sequence< size_t, sizeof ... (T_args)>(), args ...);
        notify();
    }

    void
    operator() ( std::optional< T_args > const & ... args )
    {
        clear_successors();
        set_all_impl( std::make_integer_sequence< size_t, sizeof ... (T_args)>(), args ...);
        notify();
    }

    void
    operator() ( data_type const & args )
    {
        clear_successors();
        data() = args;
        notify();
    }

    void
    operator() ( optional_data_type const & args )
    {
        clear_successors();
        if( args ) {
            data() = *args;
        } else {
            data() = {};
        }
        notify();
    }
};

template< typename ... T_args >
std::shared_ptr< data_node_t< T_args ... > >
make_data_node()
{
    return std::make_shared< data_node_t< T_args ... > >();
}

struct computation_node_t
{
    template< typename... domain_type_t >
    struct domain_t
    {
        template< typename ... codomain_type_t >
        struct codomain_t
        {
            using domain_node_t = data_node_t< domain_type_t ... >;
            using codomain_node_t = data_node_t<  codomain_type_t ... >;

            using domain_node_ptr_t = std::shared_ptr< domain_node_t >;
            using codomain_node_ptr_t = std::shared_ptr< codomain_node_t >;

            std::shared_ptr< domain_node_t > domain_node;
            std::shared_ptr< codomain_node_t > codomain_node;

            using input_type = typename domain_node_t::optional_data_type;
            using output_type = typename codomain_node_t::optional_data_type;

            using processor_type = std::function< output_type( input_type const & ) >;

            codomain_t()
                : domain_node( std::make_shared< domain_node_t >()),
                  codomain_node( std::make_shared< codomain_node_t >())
            {}

            codomain_t( codomain_t const & ) = delete;
            codomain_t & operator = ( codomain_t const & ) = delete;

            domain_node_ptr_t  & input()
            {
                return domain_node;
            }

            domain_node_ptr_t const & input()
                const
            {
                return domain_node;
            }

            codomain_node_ptr_t & output()
            {
                return codomain_node;
            }

            codomain_node_ptr_t const & output() const
            {
                return codomain_node;
            }

            processor_type
            normalise_processor( processor_type const & fn )
            {
                return fn;
            }

            // processor_type
            // normalise_processor( std::function< output_type( typename domain_node_t::data_type const &)> const & fn,
            //                      std::function< output_type() > const & any_failure_cont
            //                      = [](){
            //                          return output_type();
            //                      })
            // {
            //     return
            //         [ fn, any_failure_cont ]( typename domain_node_t::optional_data_type const & arg)
            //         {
            //             if( arg ) {
            //                 return fn(*arg);
            //             } else {
            //                 return any_failure_cont();
            //             }
            //         };
            // }

            processor_type
            normalise_processor( std::function< output_type ( std::optional< domain_type_t > const & ...)> const & fn,
                                 std::function< output_type () > const & any_failure_cont
                                 = [](){
                                     return output_type();
                                 })
            {
                auto the_processor =
                    [ fn, any_failure_cont ]( typename domain_node_t::optional_data_type const & arg)
                    {
                        if( arg ) {
                            return std::apply( fn, *arg );
                        } else {
                            // return std::apply( fn, {} ... );
                            return any_failure_cont();
                        }
                    };
                return the_processor;
            }

            typename domain_node_t::propagation_type
            make_transfer_function( std::function< typename codomain_node_t::optional_data_type( typename domain_node_t::optional_data_type)> const & fn )
            {
                auto the_transfer_function = [this,fn]( typename domain_node_t::optional_data_type arg )
                {
                    (*output())(fn( arg ));
                };
                return the_transfer_function;
            }

            void
            setup_internal_connection( processor_type const & fn )
            {
                input()->connections.remove_callback( output() );
                input()->connections.add_callback( output(),
                                                   make_transfer_function( fn ) );
            }

            template< typename F >
            void
            set_processor( F const & fn )
            {
                setup_internal_connection( normalise_processor( fn ));
            }

            template< typename F, size_t ... k >
            output_type
            apply_unpacked( F const & fn, input_type const & input, std::index_sequence< k ... > x )
            {
                try {
                    if( input and ( std::get< k >( *input ) and ... )) {
                        return fn( *(std::get< k >( *input )) ... );
                    } else {
                        return {};
                    }
                } catch (...) {
                    return {};
                }
            }


            template< typename F>
            void
            set_simple_processor( F const & fn )
            {
                setup_internal_connection(
                    [this, fn] ( input_type const & input )
                    -> output_type
                    {
                        return
                            apply_unpacked( fn,
                                            input,
                                            std::make_index_sequence< std::tuple_size< typename input_type::value_type >::value >{});
                    });
            }
        };
    };
};

template< typename ... A, typename ... B, size_t ... k, size_t ... l >
void
copy_tuple_subset( std::tuple< std::optional< A > ... > const & from,
                   std::tuple< std::optional< B > ... > & to,
                   std::index_sequence< k ... > from_set,
                   std::index_sequence< l ... > to_set)
{
    (( std::get< l >( to )  = std::get< k >( from ), 0), ... );
}


template< typename input_type_t, typename receiver_node_ptr_t, size_t ... k, size_t ... l >
auto
make_subconnector( receiver_node_ptr_t receiver,
                   std::index_sequence< k ... > a_set,
                   std::index_sequence< l ... > b_set )
{
    return
        [ receiver, a_set, b_set ]( input_type_t const & input ) -> void
        {
            if( input ) {
                receiver->clear_successors();
                copy_tuple_subset( *input,
                                   receiver->data(),
                                   a_set,
                                   b_set );
                receiver->notify();
            }
        };
}


template< typename A, typename B, size_t ... k, size_t ... l >
void
connect_slot( std::shared_ptr< A > a,
              std::shared_ptr< B > b,
              std::index_sequence< k ... > a_set,
              std::index_sequence< l ... > b_set)
{
    a->connections.add_callback( b,
                                 make_subconnector< typename A::optional_data_type >( b, a_set, b_set ));

}

template< typename source_t, typename sink_t >
void
connect_full( std::shared_ptr< source_t >  source, std::shared_ptr< sink_t >  sink )
{
    source->output().connections.add_callback( sink,
                                               [sink]( typename sink_t::optional_data_type const & data )
                                               {
                                                   (*sink)( data );
                                               });
}
}
#endif
