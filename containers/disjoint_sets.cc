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

DisjointSets::DisjointSets(unsigned n, unsigned k)
    : _n(n), _k(k), size(_k, 1), c_id(_n), nonempty_set(_k) {
    std::iota(c_id.begin(), c_id.end(), 0);
    for (auto &v : c_id) {
        v = v % k;
    }
    // nonempty set = {V}
    for (unsigned v = 0; v < _k; v++) {
        nonempty_set.insert(v);
    }
}

template <typename T>
DisjointSets::DisjointSets(unsigned n, T cid_begin, T cid_end)
    : _n(n), _k(n), size(_k, 0), c_id(cid_begin, cid_end), nonempty_set(n, cid_begin, cid_end) {
        // update size
        for(auto it=cid_begin; it!=cid_end; it++){
            size[*it]++;
        }
}

inline void DisjointSets::assign(unsigned vid, unsigned cid) {
    unsigned old_cid = c_id[vid];
    // check if the cid is the original one
    if (old_cid == cid) {
        return;
    }
    // update cluster index
    c_id[vid] = cid;
    // update cluster size
    size[old_cid]--;
    size[cid]++;
    if (size[old_cid] == 0) {
        nonempty_set.erase(old_cid);
    }
}

inline void DisjointSets::_insert(unsigned v, unsigned new_cid,
                           std::vector<unsigned> &new_size) {
    unsigned idx = stored_range[new_cid] + new_size[new_cid];
    partition[idx] = v;
    new_size[new_cid]++;
}

inline void DisjointSets::relabel_cid() {
    for (auto &cid : c_id) {
        cid = nonempty_set.relabel(cid);
    }
}

inline auto DisjointSets::rearrange(const std::vector<double> &c_list) {
    std::vector<double> new_pcc(num_sets());
    for(unsigned cid=0; cid<num_sets(); cid++){
        unsigned original_cid = nonempty_set.relabel_inv(cid);
        new_pcc[cid] = c_list[original_cid];
    }
    return new_pcc;
}


inline void DisjointSets::init_disjoint_sets() {
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
    // initialize the size array of disjoint set
    std::vector<unsigned> new_size(num_nonempty);
    std::fill(new_size.begin(), new_size.end(), 0);

    partition.resize(_n);
    for (unsigned v = 0; v < _n; v++) {
        unsigned cid = c_id[v];
        unsigned new_cid = nonempty_set.relabel(cid);
        _insert(v, new_cid, new_size);
    }
    relabel_cid();
}
