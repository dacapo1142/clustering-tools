#include "disjoint_sets.h"
#include <iostream>
#include <numeric>

DisjointSets::DisjointSets(unsigned n)
    : _n(n), _k(n), size(_k, 1), c_id(_n), nonempty_set(n) {
    // each vertex is an individual community, hence c_id[i]=i
    std::iota(c_id.begin(), c_id.end(), 0);
    // nonempty set = V
    nonempty_set.initial_full();
}

void DisjointSets::assign(unsigned vid, unsigned cid) {
    unsigned old_cid = c_id[vid];
    if (old_cid == cid) {
        return;
    }
    c_id[vid] = cid;
    size[old_cid]--;
    size[cid]++;
    if (size[old_cid] == 0) {
        nonempty_set.erase(old_cid);
    }
}

void DisjointSets::_insert(unsigned v, unsigned new_cid) {
    unsigned idx = stored_range[new_cid] + new_size[new_cid];
    disjoint_set[idx] = v;
    new_size[new_cid]++;
}

void DisjointSets::init_disjoint_set() {
    // initialize stored_range
    unsigned num_nonempty = nonempty_set.size();
    stored_range.resize(num_nonempty + 1);
    stored_range[0] = 0;
    unsigned acc = 0;
    for (auto &cid : nonempty_set) {
        auto new_cid = nonempty_set.relabel(cid);
        acc += size[cid];
        stored_range[new_cid + 1] = acc;
    }
    // initialize disjoint set size
    new_size.resize(num_nonempty);
    std::fill(new_size.begin(), new_size.end(), 0);

    disjoint_set.resize(_n);
    for (unsigned v = 0; v < _n; v++) {
        unsigned cid = c_id[v];
        unsigned new_cid = nonempty_set.relabel(cid);
        _insert(v, new_cid);
    }
}
