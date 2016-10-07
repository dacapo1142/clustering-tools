#include "disjoint_sets.h"
#include <iostream>

template <typename T>
DisjointSets::DisjointSets(size_t n, size_t k, T which_cluster_begin,
                           T which_cluster_end)
    : _n(n), _k(k), data(n), first(k, NONE), last(k, NONE), size(k, 0),
      which_cluster(which_cluster_begin, which_cluster_end) {
    assert(_n != NONE);
    initial();
}

void DisjointSets::print() {
    for (size_t cid = 0; cid < _k; cid++) {
        if (!empty(cid)) {
            size_t vid = first[cid];
            while (vid != NONE) {
                std::cout << vid << " ";
                vid = next(vid);
            }
            std::cout << std::endl;
        }
    }
}
