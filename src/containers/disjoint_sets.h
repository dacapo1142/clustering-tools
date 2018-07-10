#ifndef DISJOINT_SETS_H
#define DISJOINT_SETS_H

#include "VectorSet.hpp"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

class DisjointSets {
  public:
    // iterator wrapper
    template <typename T> class RangeWrapper {
        T _start;
        T _end;
      public:
        RangeWrapper(T start, T end)
            : _start(start), _end(end) {}
        auto begin() { return _start; }
        auto end() { return _end; }
    };
    // member
    static const unsigned NONE;
    unsigned _n;
    unsigned _k;
    std::vector<unsigned> size;
    std::vector<unsigned> c_id;
    VectorSet nonempty_set;
    std::vector<unsigned> partition;
    std::vector<unsigned> stored_range;

    // constructors
    DisjointSets(unsigned n);
    DisjointSets(unsigned n, unsigned k);
    template <typename T> DisjointSets(unsigned n, T cid_begin, T cid_end);

    // operations
    inline void _insert(unsigned v, unsigned new_cid,
                        std::vector<unsigned> &new_size);
    inline void init_disjoint_sets();
    inline void relabel_cid();
    inline void print(std::ostream &os);
    inline void assign(unsigned vid, unsigned cid);
    inline auto operator[](unsigned cid) {
        return RangeWrapper<decltype(partition)::iterator>(
            partition.begin() + stored_range[cid], partition.begin() + stored_range[cid+1]);
    };
    inline auto operator[](unsigned cid) const {
        return RangeWrapper<decltype(partition)::const_iterator>(
            partition.begin() + stored_range[cid], partition.begin() + stored_range[cid+1]);
    };
    inline auto num_sets() const { return nonempty_set.size(); }
    inline auto rearrange(const std::vector<double> &c_list);
};

const unsigned DisjointSets::NONE = std::numeric_limits<unsigned>::max();
std::ostream &operator<<(std::ostream &os, const DisjointSets &ds) {
    for (unsigned i = 0; i < ds.num_sets(); i++) {
        for (const auto &v : ds[i]) {
            os << v << " ";
        }
        os<<"\n";
    }
    return os;
}

#include "disjoint_sets.cc"
#endif
