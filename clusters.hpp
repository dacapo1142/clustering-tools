#ifndef CLUSTERS_HPP
#define CLUSTERS_HPP
#include "containers/VectorSet.hpp"
#include "containers/disjoint_sets.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <set>
#include <tuple>
#include <vector>

class Clusters {
  public:
    enum InputFormat { TWO_COLOUMN, THREE_COLOUMN };
    enum PartitionMethod { KSETS_PLUS, PARTITION_INPUT };
    class NodeInfo {
      public:
        unsigned vid;
        double weight;
        NodeInfo(unsigned _vid, double _weight) : vid(_vid), weight(_weight) {}
        NodeInfo(unsigned _vid) : NodeInfo(_vid, 0.0) {}
    };

    class NeighborVertex {
      public:
        NeighborVertex(unsigned _cid) : cid(_cid), cross(0.0) {}
        unsigned cid;
        double cross;
    };

    template <typename T> class VertexRecord {
      public:
        unsigned vid;
        T it;
        VertexRecord(unsigned _vid) : vid(_vid) {}
        inline bool is_recorded_by(unsigned _vid) { return vid == _vid; }
        inline void record(unsigned _vid, T _it) {
            vid = _vid;
            it = _it;
        }
    };
    unsigned original_vcount;
    unsigned vcount;
    unsigned k;
    std::vector<std::list<NodeInfo>> adj_list;
    std::vector<double> pv_list;
    std::vector<double> pc_list;
    std::vector<double> pcc_list;
    std::vector<double> pvv_list;
    std::vector<unsigned> which_supernode;
    VectorSet nonempty_set;
    DisjointSets sets;
    unsigned seed;
    double total_weight;
    std::list<unsigned> size_record;
    std::list<unsigned> iter_record;

    Clusters(unsigned _vcount, unsigned _k, std::istream &file,
             InputFormat inputformat = TWO_COLOUMN)
        : original_vcount(_vcount), vcount(_vcount), k(_k), adj_list(_vcount),
          pv_list(_vcount, 0.0), pc_list(_k, 0.0), pcc_list(_k, 0.0),
          pvv_list(_vcount, 0.0), which_supernode(_vcount, 0), nonempty_set(_k),
          sets(_vcount, _k), total_weight(0.0) {
        // seed(std::chrono::system_clock::now().time_since_epoch().count())
        nonempty_set.initial_full();

        read_weighted_edgelist_undirected(file, inputformat);
    }

    Clusters(unsigned _vcount, unsigned _k, std::istream &file, double lambda0,
             double lambda1)
        : original_vcount(_vcount), vcount(_vcount), k(_k), adj_list(_vcount),
          pv_list(_vcount, 0.0), pc_list(_k, 0.0), pcc_list(_k, 0.0),
          pvv_list(_vcount, 0.0), which_supernode(_vcount, 0), nonempty_set(_k),
          sets(_vcount, _k), total_weight(0.0) {
        // seed(std::chrono::system_clock::now().time_since_epoch().count())
        nonempty_set.initial_full();

        read_weighted_edgelist_undirected(file, lambda0, lambda1);
    }

    void add_edge(const unsigned &vid1, const unsigned &vid2,
                  const double &weight) {

        if (vid1 == vid2) {
            total_weight += weight;
            pv_list[vid1] += weight;
            unsigned cid1 = sets.which_cluster[vid1];
            pc_list[cid1] += weight;
            pcc_list[cid1] += weight;
            pvv_list[vid1] += weight;
        } else {
            total_weight += 2 * weight;
            adj_list[vid1].push_back(NodeInfo(vid2, weight));
            adj_list[vid2].push_back(NodeInfo(vid1, weight));
            pv_list[vid1] += weight;
            pv_list[vid2] += weight;
            unsigned cid1 = sets.which_cluster[vid1];
            unsigned cid2 = sets.which_cluster[vid2];
            pc_list[cid1] += weight;
            pc_list[cid2] += weight;
            if (cid1 == cid2) {
                pcc_list[cid1] += weight;
            }
        }
    }
    void read_weighted_edgelist_undirected(std::istream &file,
                                           InputFormat inputformat) {
        unsigned vid1, vid2;

        if (inputformat == TWO_COLOUMN) {
            while (file >> vid1 >> vid2) {
                add_edge(vid1, vid2, 1);
            }
        } else if (inputformat == THREE_COLOUMN) {
            double weight;
            while (file >> vid1 >> vid2 >> weight) {
                add_edge(vid1, vid2, weight);
            }
        }

        for (auto &pv : pv_list) {
            pv /= total_weight;
        }
        for (auto &pc : pc_list) {
            pc /= total_weight;
        }
        for (auto &pcc : pcc_list) {
            pcc /= total_weight;
        }

        for (auto &pvv : pvv_list) {
            pvv /= total_weight;
        }

        for (unsigned vid = 0; vid < vcount; vid++) {
            for (auto &nei : adj_list[vid]) {
                nei.weight /= total_weight;
            }
        }

        for (unsigned vid = 0; vid < vcount; vid++) {
            which_supernode[vid] = vid;
        }
    }

    void read_weighted_edgelist_undirected(std::istream &file, double lambda0,
                                           double lambda1) {
        unsigned vid1, vid2;
        while (file >> vid1 >> vid2) {
            add_edge(vid1, vid2, lambda1); // p_ij = lambda_1 / 2m if i != j
        }

        for (unsigned vid = 0; vid < vcount; vid++) {
            unsigned k_i = adj_list[vid].size();
            if (pvv_list[vid] > 0.0) { // already has a selfloop
                k_i += 1;
            }
            add_edge(vid, vid,
                     lambda0 * k_i); // p_ij = lambda_0 * k_i / 2m if i == j
        }

        for (auto &pv : pv_list) {
            pv /= total_weight;
        }
        for (auto &pc : pc_list) {
            pc /= total_weight;
        }
        for (auto &pcc : pcc_list) {
            pcc /= total_weight;
        }

        for (auto &pvv : pvv_list) {
            pvv /= total_weight;
        }

        for (unsigned vid = 0; vid < vcount; vid++) {
            for (auto &nei : adj_list[vid]) {
                nei.weight /= total_weight;
            }
        }

        for (unsigned vid = 0; vid < vcount; vid++) {
            which_supernode[vid] = vid;
        }
    }

    bool partition_procedure(const PartitionMethod &method) {
        unsigned round_count = 0;
        bool changed_once = false;
        bool changed = true;
        VectorSet candidate_set(vcount);
        std::vector<double> weight_list(vcount);
        while (changed) {
            round_count++;
            changed = false;

            for (unsigned vid = 0; vid < vcount; vid++) {
                candidate_set.clear();
                unsigned old_cid = sets.which_cluster[vid];

                // old cid should be listed on the first of the set
                candidate_set.insert(old_cid);
                weight_list[old_cid] = 0;
                for (auto &vertex2 : adj_list[vid]) {
                    unsigned vid2 = vertex2.vid;
                    unsigned cid = sets.which_cluster[vid2];

                    if (!candidate_set.contain(cid)) {
                        candidate_set.insert(cid);
                        weight_list[cid] = 0.0;
                    }
                    weight_list[cid] += vertex2.weight;
                }
                candidate_set.erase(old_cid);

                double pv = pv_list[vid];
                unsigned best_cid = old_cid;

                double best_correlation_measure = 0;
                if (sets.size[old_cid] != 1) {
                    best_correlation_measure =
                        weight_list[old_cid] - (pc_list[old_cid] - pv) * pv;
                }

                for (auto &cid : candidate_set) {
                    double correlation_measure = weight_list[cid];
                    correlation_measure = weight_list[cid] - pc_list[cid] * pv;

                    if (correlation_measure > best_correlation_measure) {
                        best_correlation_measure = correlation_measure;
                        best_cid = cid;
                    }
                }

                // doesn't change
                if (best_cid == old_cid) {
                    continue;
                }

                if (sets.size[old_cid] == 1) {
                    nonempty_set.erase(old_cid);
                }

                changed = true;
                changed_once = true;
                sets.move(vid, best_cid);

                // P{uniform choice an edge, and first end is in cluster c}
                pc_list[old_cid] -= pv_list[vid];
                pc_list[best_cid] += pv_list[vid];

                // P{uniform choice and edge, and two ends are in cluster c}
                pcc_list[old_cid] -= 2 * weight_list[old_cid] + pvv_list[vid];
                pcc_list[best_cid] += 2 * weight_list[best_cid] + pvv_list[vid];
            }
        }
        iter_record.push_back(round_count);
        return changed_once;
    }
    template <typename T> void print_vector(T vec) {
        for (auto &v : vec) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
    bool node_aggregation() { // O(m+n)
        unsigned new_vcount = nonempty_set.size();
        // VectorSet neighbor_set(new_vcount);

        typedef std::list<NodeInfo>::iterator It;
        std::vector<std::tuple<It, It, unsigned>> entries(new_vcount, std::make_tuple(It(), It(), new_vcount+1));
        unsigned max_cid = vcount;
        std::vector<std::list<NodeInfo>> new_adj_list(new_vcount);
        std::vector<double> new_pv_list(new_vcount, 0.0);
        std::vector<double> new_pcc_list(new_vcount, 0.0);

        for (auto &cid1 : nonempty_set) {

            unsigned new_vid1 = nonempty_set.position(cid1);
            unsigned new_cid1 = new_vid1;
            new_pcc_list[new_cid1] = pcc_list[cid1];
            for (auto vid1 = sets.begin(cid1); vid1 != sets.end();
                 vid1 = sets.next(vid1)) {
                // neighbor_set.clear();
                new_pv_list[new_vid1] += pv_list[vid1];
                for (auto &vertex2 : adj_list[vid1]) {
                    unsigned vid2 = vertex2.vid;
                    unsigned cid2 = sets.which_cluster[vid2];
                    unsigned new_vid2 = nonempty_set.position(cid2);
                    // !(vid1 >= vid2) -> avoid duplicate calculation
                    // !(new_vid1 == new_vid2) -> same cluster
                    if (new_vid1 >= new_vid2) {
                        continue;
                    }

                    double weight = vertex2.weight;

                    if (std::get<2>(entries[new_vid2]) != new_vid1) {
                        new_adj_list[new_vid1].push_back(NodeInfo(new_vid2));
                        new_adj_list[new_vid2].push_back(NodeInfo(new_vid1));
                        entries[new_vid2] = std::make_tuple(
                            std::prev(new_adj_list[new_vid1].end()),
                            std::prev(new_adj_list[new_vid2].end()), new_vid1);
                    }
                    auto it = entries[new_vid2];
                    std::get<0>(it)->weight += weight;
                    std::get<1>(it)->weight += weight;
                }
            }
        }
        std::vector<double> new_pc_list(new_pv_list);
        std::vector<unsigned> new_which_cluster(new_vcount);
        for (unsigned vid = 0; vid < new_vcount; vid++) {
            new_which_cluster[vid] = vid;
        }
        pv_list = std::move(new_pv_list);
        pc_list = std::move(new_pc_list);
        pcc_list = std::move(new_pcc_list);
        pvv_list = pcc_list;
        adj_list = std::move(new_adj_list);

        vcount = new_vcount;
        DisjointSets new_sets(new_vcount, new_vcount, new_which_cluster.begin(),
                              new_which_cluster.end());

        for (unsigned vid = 0; vid < original_vcount; vid++) {
            unsigned supernode_id = which_supernode[vid];
            unsigned cid = sets.which_cluster[supernode_id];
            which_supernode[vid] = nonempty_set.position(cid);
        }
        sets = std::move(new_sets);
        VectorSet new_nonempty_set(new_vcount);
        new_nonempty_set.initial_full();
        nonempty_set = std::move(new_nonempty_set);
        size_record.push_back(nonempty_set.size());
        return true;
    }

    void routine() {
        while (true) {
            k = vcount;
            if (!partition_procedure(PartitionMethod::PARTITION_INPUT) ||
                !node_aggregation()) {
                break;
            }
        }
    }

    void print() {
        for (unsigned vid = 0; vid < original_vcount; vid++) {
            std::cout << sets.which_cluster[which_supernode[vid]] << " ";
        }
        std::cout << std::endl;
    }

    void print_communities(std::ostream &of = std::cout) {

        DisjointSets s(original_vcount, nonempty_set.size(),
                       which_supernode.begin(), which_supernode.end());
        s.print(of);
    }

    void print_size(std::ostream &f) {
        for (auto &v : size_record) {
            f << v << " ";
        }
        f << std::endl;
    }
    void print_iter(std::ostream &f) {
        for (auto &v : iter_record) {
            f << v << " ";
        }
        f << std::endl;
    }
};

#endif
