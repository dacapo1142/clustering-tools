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
    // random initial with given _seed
    // DisjointSets(size_t n, size_t k, size_t seed)
    //     : _n(n), _k(k), _seed(seed), data(_n), first(_k, NONE), last(_k,
    //     NONE),
    //       size(_n, 0), which_cluster(n) {
    //     assert(_n != NONE);
    //     random_assign();
    //     initial();
    // }
    // DisjointSets(size_t n, size_t k)
    //     : _n(n), _k(k),
    //       _seed(std::chrono::system_clock::now().time_since_epoch().count()),
    //       data(_n), first(_k, NONE), last(_k, NONE), size(_n, 0),
    //       which_cluster(n) {
    //     _seed = 1460625373037597;
    //     assert(_n != NONE);
    //     random_assign();
    //     initial();
    // }
    void initial();
    inline bool empty(size_t cid);
    inline void insert(size_t vid, size_t cid) {
        size[cid]++;
        if (empty(cid)) {
            first[cid] = vid;
            last[cid] = vid;
            data[vid].prev_id = NONE;
            data[vid].next_id = NONE;
        } else {
            size_t last_id = last[cid];
            auto &last_node = data[last_id];
            auto &new_node = data[vid];
            last_node.next_id = vid;
            new_node.prev_id = last_id;
            new_node.next_id = NONE;
            last[cid] = vid;
        }
    }
    inline void print();
    inline size_t begin(size_t cid) { return first[cid]; }
    inline size_t end() { return NONE; }
    inline bool not_end(size_t vid) { return vid != NONE; }
    inline size_t next(size_t &vid) { return data[vid].next_id; }
    // append cid2 to cid1
    void merge(size_t cid1, size_t cid2) {
        if (cid1 > cid2) {
            std::swap(cid1, cid2);
        }

        if (empty(cid2)) {
            return;
        }

        size[cid1] += size[cid2];
        size[cid2] = 0;

        size_t vid = first[cid2];
        while (vid != NONE) {
            which_cluster[vid] = cid1;
            vid = next(vid);
        }

        size_t first_cid2_index = first[cid2];
        size_t last_cid1_index = last[cid1];
        first[cid2] = NONE;
        data[last_cid1_index].next_id = first_cid2_index;
        data[first_cid2_index].prev_id = last_cid1_index;

        size_t last_cid2_index = last[cid2];
        last[cid2] = NONE;
        last[cid1] = last_cid2_index;
    }

    void random_assign() {
        std::vector<size_t> vertice(_n);
        for (size_t i = 0; i < _n; i++) {
            vertice[i] = i;
        }
        // shuffle(vertice.begin(), vertice.end(),
        //         std::default_random_engine(_seed));
        for (size_t i = 0; i < _n; i++) {
            which_cluster[i] = vertice[i] % _k;
        }
    }

    void move(size_t vid, size_t cid) {
        size_t old_cid = which_cluster[vid];
        if (old_cid == cid) {
            return;
        }
        which_cluster[vid] = cid;
        size[old_cid]--;
        size_t prev_id = data[vid].prev_id;
        size_t next_id = data[vid].next_id;
        if (prev_id != NONE) {
            data[prev_id].next_id = next_id;
        } else {
            first[old_cid] = next_id;
        }
        if (next_id != NONE) {
            data[next_id].prev_id = prev_id;
        } else {
            last[old_cid] = prev_id;
        }

        // need to be fixed
        // size[cid]++;
        // data[vid].next_id = NONE;
        // data[vid].prev_id = last[cid];
        // data[last[cid]].next_id = vid;
        // last[cid] = vid;
        insert(vid, cid);
    }
};

const size_t DisjointSets::NONE = std::numeric_limits<std::size_t>::max();
#include "disjoint_sets.cc"
#endif
