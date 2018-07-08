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
        unsigned next_id;
        unsigned prev_id;
    };
    class iterator {
      public:
        unsigned i;
    };
    static const unsigned NONE;
    unsigned _n;
    unsigned _k;
    long _seed;
    std::vector<Node> data;
    std::vector<unsigned> first;
    std::vector<unsigned> last;
    std::vector<unsigned> size;
    std::vector<unsigned> which_cluster;
    template <typename T>
    DisjointSets(unsigned n, unsigned k, T which_cluster_begin,
                 T which_cluster_end);
    DisjointSets(unsigned n, unsigned k, unsigned seed);
    DisjointSets(unsigned n, unsigned k);
    void initial();
    inline bool empty(unsigned cid);
    inline void insert(unsigned vid, unsigned cid);
    inline void print(std::ostream &os);
    inline unsigned begin(unsigned cid) { return first[cid]; }
    inline unsigned end() { return NONE; }
    inline bool not_end(unsigned vid) { return vid != NONE; }
    inline unsigned next(unsigned &vid) { return data[vid].next_id; }
    void merge(unsigned cid1, unsigned cid2);
    void random_assign();
    void move(unsigned vid, unsigned cid);
    unsigned operator[](unsigned i) { return 3; };
};

const unsigned DisjointSets::NONE = std::numeric_limits<unsigned>::max();
#include "disjoint_sets.cc"
#endif
