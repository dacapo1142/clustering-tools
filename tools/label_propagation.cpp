#include "clusters.hpp"
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>
using namespace std;
using namespace chrono;

int main(int argc, char *argv[]) {
    default_random_engine generator;

    const char *filename = argv[1];
    ifstream file(filename);
    unsigned n = atoi(argv[2]);
    unsigned k = atoi(argv[3]);
    unsigned total_k = n;
    Clusters clusters(n, n, file);
    bool changed = true;
    bool not_reach = true;
    vector<unsigned> iter_order(n);
    for (unsigned vid = 0; vid < n; vid++) {
        iter_order[vid] = vid;
    }
    shuffle(iter_order.begin(), iter_order.end(), generator);

    while (changed && not_reach) {
        changed = false;
        for (auto &vid : iter_order) {
            unsigned max_count = 0;
            unsigned tie_count = 0;
            map<unsigned, unsigned> m;
            for (auto &vertex2 : clusters.adj_list[vid]) {
                unsigned vid2 = vertex2.vid;
                unsigned cid2 = clusters.sets.which_cluster[vid2];
                m[cid2]++;
                if (m[cid2] == max_count) {
                    tie_count++;
                } else if (m[cid2] > max_count) {
                    tie_count = 1;
                    max_count = m[cid2];
                }
            }
            vector<unsigned> v;
            v.reserve(tie_count);
            for (auto &pair : m) {
                if (pair.second == max_count) {
                    v.push_back(pair.first);
                }
            }
            uniform_int_distribution<int> distribution(0, tie_count - 1);
            unsigned idx = distribution(generator);
            unsigned new_cid = v[idx];
            unsigned old_cid = clusters.sets.which_cluster[vid];
            clusters.sets.move(vid, new_cid);

            if (new_cid != old_cid) {
                changed = true;
            }

            if (clusters.sets.size[old_cid] == 0) {
                total_k--;
            }
            if (total_k <= k) {
                not_reach = false;
                break;
            }
        }
    }

    clusters.sets.print();

    return 0;
}
