#include "disjoint_sets.h"
#include <iostream>

template <typename T>
DisjointSets::DisjointSets(unsigned n, unsigned k, T which_cluster_begin,
                           T which_cluster_end)
    : _n(n), _k(k), data(n), first(k, NONE), last(k, NONE), size(k, 0),
      which_cluster(which_cluster_begin, which_cluster_end) {
    assert(_n != NONE);
    initial();
}

DisjointSets::DisjointSets(unsigned n, unsigned k, unsigned seed)
    : _n(n), _k(k), _seed(seed), data(_n), first(_k, NONE), last(_k, NONE),
      size(_n, 0), which_cluster(n) {
    assert(_n != NONE);
    random_assign();
    initial();
}

DisjointSets::DisjointSets(unsigned n, unsigned k)
    : _n(n), _k(k),
      _seed(std::chrono::system_clock::now().time_since_epoch().count()),
      data(_n), first(_k, NONE), last(_k, NONE), size(_n, 0), which_cluster(n) {
    assert(_n != NONE);
    random_assign();
    initial();
}

void DisjointSets::random_assign() {
    std::vector<unsigned> vertice(_n);
    for (unsigned i = 0; i < _n; i++) {
        vertice[i] = i;
    }
    // shuffle(vertice.begin(), vertice.end(),
    //         std::default_random_engine(_seed));
    for (unsigned i = 0; i < _n; i++) {
        which_cluster[i] = vertice[i] % _k;
    }
}

void DisjointSets::print() {
    for (unsigned cid = 0; cid < _k; cid++) {
        if (!empty(cid)) {
            unsigned vid = first[cid];
            while (vid != NONE) {
                std::cout << vid << " ";
                vid = next(vid);
            }
            std::cout << std::endl;
        }
    }
}

void DisjointSets::insert(unsigned vid, unsigned cid) {
    size[cid]++;
    if (empty(cid)) {
        first[cid] = vid;
        last[cid] = vid;
        data[vid].prev_id = NONE;
        data[vid].next_id = NONE;
    } else {
        unsigned last_id = last[cid];
        auto &last_node = data[last_id];
        auto &new_node = data[vid];
        last_node.next_id = vid;
        new_node.prev_id = last_id;
        new_node.next_id = NONE;
        last[cid] = vid;
    }
}

void DisjointSets::initial() {
    for (unsigned vid = 0; vid < _n; vid++) {
        unsigned cid = which_cluster[vid];
        insert(vid, cid);
    }
}

void DisjointSets::merge(unsigned cid1, unsigned cid2) {
    if (cid1 > cid2) {
        std::swap(cid1, cid2);
    }

    if (empty(cid2)) {
        return;
    }

    size[cid1] += size[cid2];
    size[cid2] = 0;

    unsigned vid = first[cid2];
    while (vid != NONE) {
        which_cluster[vid] = cid1;
        vid = next(vid);
    }

    unsigned first_cid2_index = first[cid2];
    unsigned last_cid1_index = last[cid1];
    first[cid2] = NONE;
    data[last_cid1_index].next_id = first_cid2_index;
    data[first_cid2_index].prev_id = last_cid1_index;

    unsigned last_cid2_index = last[cid2];
    last[cid2] = NONE;
    last[cid1] = last_cid2_index;
}

void DisjointSets::move(unsigned vid, unsigned cid) {
    unsigned old_cid = which_cluster[vid];
    if (old_cid == cid) {
        return;
    }
    which_cluster[vid] = cid;
    size[old_cid]--;
    unsigned prev_id = data[vid].prev_id;
    unsigned next_id = data[vid].next_id;
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
    insert(vid, cid);
}

bool DisjointSets::empty(unsigned cid) { return last[cid] == NONE; }
