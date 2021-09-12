/* Tara Lorenz (c) 2020, GPL3 or Apache License 2.0 */
#pragma once
#ifndef FILE_60E69869_FBF6C337739AA77_INCLUDED
#define FILE_60E69869_FBF6C337739AA77_INCLUDED
#include <vector>
#include <unordered_map>

namespace tz {
/* Classical union-find algorithm with Tarjans heuristics.
   See https://en.wikipedia.org/wiki/Disjoint-set_data_structure */

struct union_finder_t
{
    struct prep_t {
        size_t root;
        size_t size; /* not used ATM */
    };

    std::vector< prep_t > index_to_rep;

    union_finder_t( size_t num_elements );
    ~union_finder_t() = default;

    void
    add_relation( size_t a, size_t b);

    std::vector< std::vector< size_t > >
    classes();
};
}
#endif
