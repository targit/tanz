/* Tara Lorenz (c) 2020, GPL3 or Apache License 2.0 */
#include <tanz/union-find.h++>
#include <iostream>
#include <functional>

namespace tz {

union_finder_t::union_finder_t( size_t num_elements )
    : index_to_rep( num_elements )
{
    for( size_t k = 0; k < num_elements; k = k + 1 ) {
        index_to_rep[ k ] = { k, 1 };
    }
}

namespace {
size_t find_root( std::vector< union_finder_t::prep_t > & map, size_t a )
{
    auto & rep = map[ a ];
    auto current_root = rep.root;
    if( current_root == a )
    {
        return current_root;
    } else {
        /* Using the stack, fingers crossed… */
        auto new_root = find_root( map, current_root );
        rep.root = new_root;
        return new_root;
    }
};
}

void
union_finder_t::add_relation( size_t a, size_t b)
{
    auto unite_roots =
        [ this ]( size_t root_a, size_t root_b )
        {
            if( root_a != root_b ) {
                auto & x_a = index_to_rep[ root_a ];
                auto & x_b = index_to_rep[ root_b ];
                if( x_a.size < x_b.size ) {
                    /* b will be common root */
                    x_a.root = root_b;
                    x_b.size = x_a.size + x_b.size;
                } else {
                    /* a will be common root */
                    x_b.root = root_a;
                    x_a.size = x_a.size + x_b.size;
                }
            }
        };
    
    unite_roots( find_root( index_to_rep, a ),
                 find_root( index_to_rep, b ));
}

std::vector< std::vector< size_t > >
union_finder_t::classes()
{
    size_t current_index = 0;
    std::unordered_map< size_t, size_t > root_to_output_index;

    size_t tx = 0;
    for( auto const & rep : index_to_rep ) {
        auto root = find_root( index_to_rep, rep.root);
        if( root_to_output_index.insert( { root, current_index } ).second ) {
            current_index = current_index + 1;
        }
    }
    std::vector< std::vector< size_t > > cs( root_to_output_index.size());
    for( size_t k = 0; k < index_to_rep.size(); k = k + 1 ) {
        auto root = find_root( index_to_rep, index_to_rep[ k ].root);
        auto output_index = root_to_output_index[ root  ];
        cs[ output_index ].push_back( k );
    }

    return cs;
}
}
