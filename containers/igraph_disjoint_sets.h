#ifndef DISJOINT_SETS_H
#define DISJOINT_SETS_H

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

class DisjointSets {
  public:
    class Node {
      public:
        size_t next_id;
        size_t prev_id;
    };
    class iterator {
      public:
        size_t i;
    };
    static const size_t NONE;
    size_t _n;
    size_t _k;
    size_t _seed;
    std::vector<Node> data;
    std::vector<size_t> first;
    std::vector<size_t> last;
    std::vector<size_t> size;
    std::vector<size_t> which_cluster;
    template <typename T>
    DisjointSets(size_t n, size_t k, T which_cluster_begin,
                 T which_cluster_end);
    DisjointSets(size_t n, size_t k, size_t seed);
    DisjointSets(size_t n, size_t k);
    void initial();
    inline bool empty(size_t cid);
    inline void insert(size_t vid, size_t cid);
    inline void print();
    inline size_t begin(size_t cid) { return first[cid]; }
    inline size_t end() { return NONE; }
    inline bool not_end(size_t vid) { return vid != NONE; }
    inline size_t next(size_t &vid) { return data[vid].next_id; }
    void merge(size_t cid1, size_t cid2);
    void random_assign();
    void move(size_t vid, size_t cid);
    size_t operator[](size_t i) { return 3; };
};

const size_t DisjointSets::NONE = std::numeric_limits<std::size_t>::max();
#include "disjoint_sets.cc"
#endif
