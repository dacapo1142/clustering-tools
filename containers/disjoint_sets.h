#ifndef DISJOINT_SETS_H
#define DISJOINT_SETS_H

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <vector>
#include "VectorSet.hpp"

class DisjointSets {
  public:
  //iterator wrapper
    template <typename T> class IterWrapper {
        T &_v;
        size_t _start;
        size_t _end;
      public:
        IterWrapper(T &v, size_t start, size_t end)
            : _v(v), _start(start), _end(end) {}
        auto begin() { return _v.begin() + _start; }
        auto end(){return _v.begin() + _end;}
        auto size(){return _end-_start;}
    };
    //member
    static const unsigned NONE;
    unsigned _n;
    unsigned _k;
    std::vector<unsigned> size;
    std::vector<unsigned> c_id;
    VectorSet nonempty_set;
    std::vector<unsigned> disjoint_set;
    std::vector<unsigned> stored_range;
    std::vector<unsigned> new_size;

    //opeartions
    DisjointSets(unsigned n);
    void init_disjoint_set();
    inline void _insert(unsigned vid, unsigned cid);
    inline void print(std::ostream &os);
    void assign(unsigned vid, unsigned cid);
    auto operator[](unsigned cid) {
      return IterWrapper<decltype(disjoint_set)>(disjoint_set, size[cid], size[cid+1]);
    };
    auto num_sets(){return nonempty_set.size();}
};

const unsigned DisjointSets::NONE = std::numeric_limits<unsigned>::max();
#include "disjoint_sets.cc"
#endif
