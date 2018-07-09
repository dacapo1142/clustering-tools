
// compile:
// g++ ksets.cpp -std=c++14 -O3 -o ksets

// execute:
// random initialization
// ./ksets distance_metric number_of_the_vertic number_of_the_communities
// initialize with hunsigned
// ./ksets distance_metric number_of_the_vertic number_of_the_communities
// hint_file
// Li-Heng Liou, dacapo1142@gmail.com
#include "containers/VectorSet.hpp"
#include "containers/disjoint_sets.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <ratio>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

inline double dbar_increase(double kstar_len, double &dbar, double &dtemp) {
    return pow(kstar_len, 2) / pow(kstar_len + 1, 2) * dbar +
           2 * kstar_len / pow(kstar_len + 1, 2) * dtemp;
}

inline double dbar_decrease(double old_len, double &dbar, double &dtemp) {
    return pow(old_len, 2) / pow(old_len - 1, 2) * (dbar - 2 / old_len * dtemp);
}
//
// void random_initialize(vector<unsigned> &which_cluster,
//                        vector<VectorSet> &clusters, unsigned vcount,
//                        unsigned k) {
//     srand(unsigned(time(0)));
//     vector<unsigned> vertice(vcount);
//     for (unsigned vid = 0; vid < vcount; vid++) {
//         vertice[vid] = vid;
//     }
//     clusters.reserve(k);
//     for (unsigned cid = 0; cid < k; cid++) {
//         clusters.push_back(VectorSet(vcount));
//     }
//     // random_shuffle(vertice.begin(), vertice.end());
//     for (unsigned idx = 0; idx < vcount; idx++) {
//         unsigned vid = vertice[idx];
//         unsigned cid = idx % k;
//         which_cluster[vid] = cid;
//         clusters[cid].insert(vid);
//     }
//     return;
// }

// void hint_initialize(const char *hint_filename, vector<unsigned>
// &which_cluster,
//                      vector<VectorSet> &clusters, unsigned vcount, unsigned
//                      k) {
//     clusters.reserve(k);
//     for (unsigned cid = 0; cid < k; cid++) {
//         clusters.push_back(VectorSet(vcount));
//     }
//     ifstream hint_file(hint_filename);
//     string line;
//     unsigned cid = 0;
//     while (getline(hint_file, line)) {
//         istringstream iss(line);
//         vector<string> tokens{istream_iterator<string>{iss},
//                               istream_iterator<string>{}};
//         for (auto it = tokens.begin(); it != tokens.end(); it++) {
//             unsigned vid = stoi(*it);
//             which_cluster[vid] = cid;
//             clusters[cid].insert(vid);
//         }
//         cid++;
//     }
//     hint_file.close();
// }

void prunsigned_vector(vector<double> &v) {
    for (auto it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

void ksets(DisjointSets &clusters, vector<vector<double>> &distance_metric,
           unsigned vcount, unsigned k) {
    // random_initialize(which_cluster, clusters, vcount, k);
    vector<double> dbar(k, 0.0);
    for (unsigned cid = 0; cid < k; cid++) {
        dbar[cid] = 0;
        for (auto vid1 = clusters.begin(cid); vid1 != clusters.end();
             vid1 = clusters.next(vid1)) {
            for (auto vid2 = clusters.begin(cid); vid2 != clusters.end();
                 vid2 = clusters.next(vid2)) {
                dbar[cid] += distance_metric[vid1][vid2];
            }
        }
        dbar[cid] /= pow(clusters.size[cid], 2);
    }
    vector<list<unsigned>> move_in(k);
    vector<set<int>> move_in_set(k);
    vector<list<unsigned>> move_out(k);
    vector<set<int>> move_out_set(k);
    vector<vector<double>> clusters_distance(vcount, vector<double>(k));
    bool change = true;
    unsigned count = 0;
    const unsigned NONE = vcount + 1;
    while (change) {
        count++;
        change = false;
        // cout << count << endl;
        for (unsigned vid = 0; vid < vcount; vid++) {
            vector<double> dtemp(k);
            vector<double> new_dtemp(k);
            vector<double> ddelta(k);
            vector<double> new_ddelta(k);
            vector<double> ddelta_a(k);
            // first round
            if (count == 1) {
                for (unsigned cid = 0; cid < k; cid++) {
                    dtemp[cid] = 0;
                    for (auto vid2 = clusters.begin(cid);
                         vid2 != clusters.end(); vid2 = clusters.next(vid2)) {
                        dtemp[cid] += distance_metric[vid][vid2];
                    }
                    clusters_distance[vid][cid] = dtemp[cid];
                    dtemp[cid] /= clusters.size[cid];
                    ddelta[cid] = 2 * dtemp[cid] - dbar[cid];
                }
            } else {
                for (unsigned cid = 0; cid < k; cid++) {
                    unsigned vid_in = move_in[cid].front();
                    unsigned vid_out = move_out[cid].front();
                    move_in[cid].pop_front();
                    move_out[cid].pop_front();
                    if (vid_in != NONE)
                        move_in_set[cid].erase(vid_in);
                    if (vid_out != NONE)
                        move_out_set[cid].erase(vid_out);

                    for (auto vid2 = move_in_set[cid].begin();
                         vid2 != move_in_set[cid].end(); vid2++) {
                        clusters_distance[vid][cid] +=
                            distance_metric[vid][*vid2];
                    }
                    for (auto vid2 = move_out_set[cid].begin();
                         vid2 != move_out_set[cid].end(); vid2++) {
                        clusters_distance[vid][cid] -=
                            distance_metric[vid][*vid2];
                    }
                    dtemp[cid] =
                        clusters_distance[vid][cid] / clusters.size[cid];
                    ddelta[cid] = 2 * dtemp[cid] - dbar[cid];
                }
            }

            // check if vid move to other cluster
            unsigned old_k = clusters.which_cluster[vid];
            // if (clusters.size[old_k] <= 1) {
            //     for (unsigned cid = 0; cid < k; cid++) {
            //         move_in[cid].push_back(NONE);
            //         move_out[cid].push_back(NONE);
            //     }
            //     continue;
            // }

            for (unsigned cid = 0; cid < k; cid++) {
                double size = (double)clusters.size[cid];
                if (cid == old_k) {
                    if (size > 1)
                        ddelta_a[cid] = ddelta[cid] * size / (size + 1);
                    else
                        ddelta_a[cid] = numeric_limits<double>::lowest();
                } else {
                    ddelta_a[cid] = ddelta[cid] * size / (size - 1);
                }
            }

            auto best_delta_a = min_element(ddelta_a.begin(), ddelta_a.end());
            unsigned kstar =
                static_cast<unsigned>(best_delta_a - ddelta_a.begin());

            if (*best_delta_a < ddelta_a[old_k]) {
                change = true;
                dbar[kstar] = dbar_increase(clusters.size[kstar], dbar[kstar],
                                            dtemp[kstar]);
                dbar[old_k] = dbar_decrease(clusters.size[old_k], dbar[old_k],
                                            dtemp[old_k]);
                clusters.move(vid, kstar);
                for (unsigned cid = 0; cid < k; cid++) {
                    if (cid == kstar) {
                        move_in[cid].push_back(vid);
                        move_out[cid].push_back(NONE);
                        move_in_set[cid].insert(vid);
                    } else if (cid == old_k) {
                        move_in[cid].push_back(NONE);
                        move_out[cid].push_back(vid);
                        move_out_set[cid].insert(vid);
                    } else {
                        move_in[cid].push_back(NONE);
                        move_out[cid].push_back(NONE);
                    }
                }
            } else {
                for (unsigned cid = 0; cid < k; cid++) {
                    move_in[cid].push_back(NONE);
                    move_out[cid].push_back(NONE);
                }
            }
        }
    }

    return;
}

void read_binary_metric(const char *metric_filename, unsigned vcount,
                        vector<vector<double>> &distance_metric) {
    ifstream metric_file(metric_filename, ios::binary); // input binary file
    double read;
    for (unsigned row = 0; row < vcount; row++) {
        for (unsigned col = 0; col < vcount; col++) {
            metric_file.read(reinterpret_cast<char *>(&read), sizeof read);
            distance_metric[row][col] = read;
            // cout << read << " ";
        }
        // cout << endl;
    }
    metric_file.close();
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 5) {
        cout << argc << endl;
        cout << "./ksets distance_metric number_of_the_vertic "
                "number_of_the_communities [hint_file]"
             << endl;
        return 0;
    }
    const char *metric_filename = argv[1];
    unsigned vcount = atoi(argv[2]); // number of vertice
    unsigned k = atoi(argv[3]);      // number of communities
    vector<vector<double>> distance_metric(
        vcount, vector<double>(vcount)); // distance metric

    read_binary_metric(metric_filename, vcount,
                       distance_metric); // read from binary file

    // vector<VectorSet> clusters;        // list of sets
    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); // start the timer
    DisjointSets *clusters = NULL;
    if (argc == 5) {
        const char *hint_filename = argv[4];
        ifstream hint_file(hint_filename);
        string line;
        unsigned cid = 0;
        vector<unsigned> which_cluster(vcount);
        while (getline(hint_file, line)) {
            istringstream iss(line);
            vector<string> tokens{istream_iterator<string>{iss},
                                  istream_iterator<string>{}};
            for (auto it = tokens.begin(); it != tokens.end(); it++) {
                unsigned vid = stoi(*it);
                which_cluster[vid] = cid;
            }
            cid++;
        }
        hint_file.close();
        clusters = new DisjointSets(vcount, k, which_cluster.begin(),
                                    which_cluster.end());
    } else {
        clusters = new DisjointSets(vcount, k);
    }

    ksets(*clusters, distance_metric, vcount, k); //
    // computing

    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); // stop the timer
    duration<double> time_span =
        duration_cast<duration<double>>(t2 - t1); // duration

    clusters->print();
    delete clusters;
    string time_filename = "nb_time.txt"; // name of the following file
    fstream time_fs(time_filename.c_str(),
                    fstream::out |
                        fstream::app);    // file record the time consumption
    time_fs << time_span.count() << endl; // output time consumption
    time_fs.close();
    return 0;
}
