
// compile:
// g++ ksets.cpp -std=c++14 -O3 -o ksets

// execute:
// random initialization
// ./ksets distance_metric number_of_the_vertic number_of_the_communities
// initialize with hint
// ./ksets distance_metric number_of_the_vertic number_of_the_communities
// hint_file
// Li-Heng Liou, dacapo1142@gmail.com
#include "containers/VectorSet.hpp"
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

class VectorSet {
  private:
    vector<int> _list;
    vector<int> _position;
    int _size;

  public:
    VectorSet(int max_length)
        : _list(max_length), _position(max_length, -1), _size(0){};
    void insert(int vid) {
        if (_position[vid] == -1) {
            _list[_size] = vid;
            _position[vid] = _size;
            _size++;
        }
        return;
    }
    inline void erase(int vid) {
        if (_position[vid] != -1) {
            int vid_pos = _position[vid];
            if (vid_pos != _size - 1) {
                int last_val = _list[_size - 1];
                swap(_list[_size - 1], _list[vid_pos]);
                _position[last_val] = vid_pos;
            }

            _position[vid] = -1;
            _size--;
        }
    }
    inline auto &operator[](int i) { return _list[i]; }

    inline auto begin() { return _list.begin(); }
    inline auto end() { return _list.begin() + _size; }
    inline int size() { return _size; }
};

inline double dbar_increase(double kstar_len, double &dbar, double &dtemp) {
    return pow(kstar_len, 2) / pow(kstar_len + 1, 2) * dbar +
           2 * kstar_len / pow(kstar_len + 1, 2) * dtemp;
}

inline double dbar_decrease(double old_len, double &dbar, double &dtemp) {
    return pow(old_len, 2) / pow(old_len - 1, 2) * (dbar - 2 / old_len * dtemp);
}

void random_initialize(vector<int> &which_cluster, vector<VectorSet> &clusters,
                       int vcount, int k) {
    srand(unsigned(time(0)));
    vector<int> vertice(vcount);
    for (int vid = 0; vid < vcount; vid++) {
        vertice[vid] = vid;
    }
    clusters.reserve(k);
    for (int cid = 0; cid < k; cid++) {
        clusters.push_back(VectorSet(vcount));
    }
    random_shuffle(vertice.begin(), vertice.end());
    for (int idx = 0; idx < vcount; idx++) {
        int vid = vertice[idx];
        int cid = idx % k;
        which_cluster[vid] = cid;
        clusters[cid].insert(vid);
    }
    return;
}

void hint_initialize(const char *hint_filename, vector<int> &which_cluster,
                     vector<VectorSet> &clusters, int vcount, int k) {
    clusters.reserve(k);
    for (int cid = 0; cid < k; cid++) {
        clusters.push_back(VectorSet(vcount));
    }
    ifstream hint_file(hint_filename);
    string line;
    int cid = 0;
    while (getline(hint_file, line)) {
        istringstream iss(line);
        vector<string> tokens{istream_iterator<string>{iss},
                              istream_iterator<string>{}};
        for (auto it = tokens.begin(); it != tokens.end(); it++) {
            int vid = stoi(*it);
            which_cluster[vid] = cid;
            clusters[cid].insert(vid);
        }
        cid++;
    }
    hint_file.close();
}

void print_vector(vector<double> &v) {
    for (auto it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

void ksets(vector<int> &which_cluster, vector<VectorSet> &clusters,
           vector<vector<double>> &distance_metric, int &vcount, int &k) {
    // random_initialize(which_cluster, clusters, vcount, k);
    vector<double> dbar(k);
    for (int cid = 0; cid < k; cid++) {
        dbar[cid] = 0;
        for (auto vid1 = clusters[cid].begin(); vid1 != clusters[cid].end();
             vid1++) {
            for (auto vid2 = clusters[cid].begin(); vid2 != clusters[cid].end();
                 vid2++) {
                dbar[cid] += distance_metric[*vid1][*vid2];
            }
        }
        dbar[cid] /= pow(clusters[cid].size(), 2);
    }
    vector<list<int>> move_in(k);
    vector<VectorSet> move_in_set(k, VectorSet(vcount));
    vector<list<int>> move_out(k);
    vector<VectorSet> move_out_set(k, VectorSet(vcount));
    vector<vector<double>> clusters_distance(vcount, vector<double>(k));
    bool change = true;
    int count = 0;
    while (change) {
        count++;
        change = false;
        // cout << count << endl;
        for (int vid = 0; vid < vcount; vid++) {
            vector<double> dtemp(k);
            vector<double> new_dtemp(k);
            vector<double> ddelta(k);
            vector<double> new_ddelta(k);
            vector<double> ddelta_a(k);
            // first round
            if (count == 1) {
                for (int cid = 0; cid < k; cid++) {
                    dtemp[cid] = 0;
                    for (auto vid2 = clusters[cid].begin();
                         vid2 != clusters[cid].end(); vid2++) {
                        dtemp[cid] += distance_metric[vid][*vid2];
                    }
                    clusters_distance[vid][cid] = dtemp[cid];
                    dtemp[cid] /= clusters[cid].size();
                    ddelta[cid] = 2 * dtemp[cid] - dbar[cid];
                }
            } else {
                for (int cid = 0; cid < k; cid++) {
                    int vid_in = move_in[cid].front();
                    int vid_out = move_out[cid].front();
                    move_in[cid].pop_front();
                    move_out[cid].pop_front();
                    if (vid_in != -1)
                        move_in_set[cid].erase(vid_in);
                    if (vid_out != -1)
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
                        clusters_distance[vid][cid] / clusters[cid].size();
                    ddelta[cid] = 2 * dtemp[cid] - dbar[cid];
                }
            }

            // check if vid move to other cluster
            int old_k = which_cluster[vid];
            if (clusters[old_k].size() <= 1) {
                continue;
            }

            for (int cid = 0; cid < k; cid++) {
                double size = (double)clusters[cid].size();
                if (cid == old_k) {
                    ddelta_a[cid] = ddelta[cid] * size / (size + 1);
                } else {
                    if (size > 1)
                        ddelta_a[cid] = ddelta[cid] * size / (size - 1);
                    else
                        ddelta_a[cid] = numeric_limits<double>::lowest();
                }
            }

            auto best_delta_a = min_element(ddelta_a.begin(), ddelta_a.end());
            int kstar = static_cast<int>(best_delta_a - ddelta_a.begin());

            if (*best_delta_a < ddelta_a[old_k]) {
                change = true;
                which_cluster[vid] = kstar;
                dbar[kstar] = dbar_increase(clusters[kstar].size(), dbar[kstar],
                                            dtemp[kstar]);
                clusters[kstar].insert(vid);
                dbar[old_k] = dbar_decrease(clusters[old_k].size(), dbar[old_k],
                                            dtemp[old_k]);
                clusters[old_k].erase(vid);
                for (int cid = 0; cid < k; cid++) {
                    if (cid == kstar) {
                        move_in[cid].push_back(vid);
                        move_out[cid].push_back(-1);
                        move_in_set[cid].insert(vid);
                    } else if (cid == old_k) {
                        move_in[cid].push_back(-1);
                        move_out[cid].push_back(vid);
                        move_out_set[cid].insert(vid);
                    } else {
                        move_in[cid].push_back(-1);
                        move_out[cid].push_back(-1);
                    }
                }
            } else {
                for (int cid = 0; cid < k; cid++) {
                    move_in[cid].push_back(-1);
                    move_out[cid].push_back(-1);
                }
            }
        }
    }

    return;
}

void read_binary_metric(const char *metric_filename, int vcount,
                        vector<vector<double>> &distance_metric) {
    ifstream metric_file(metric_filename, ios::binary); // input binary file
    double read;
    for (int row = 0; row < vcount; row++) {
        for (int col = 0; col < vcount; col++) {
            metric_file.read(reinterpret_cast<char *>(&read), sizeof read);
            distance_metric[row][col] = read;
        }
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
    int vcount = atoi(argv[2]); // number of vertice
    int k = atoi(argv[3]);      // number of communities
    vector<vector<double>> distance_metric(
        vcount, vector<double>(vcount)); // distance metric

    read_binary_metric(metric_filename, vcount,
                       distance_metric); // read from binary file

    vector<int> which_cluster(vcount); // vid to cid table
    vector<VectorSet> clusters;        // list of sets
    high_resolution_clock::time_point t1 =
        high_resolution_clock::now(); // start the timer
    if (argc == 4) {
        random_initialize(which_cluster, clusters, vcount, k);
    } else {
        const char *hint_filename = argv[4];
        hint_initialize(hint_filename, which_cluster, clusters, vcount, k);
    }
    ksets(which_cluster, clusters, distance_metric, vcount, k); // computing

    high_resolution_clock::time_point t2 =
        high_resolution_clock::now(); // stop the timer
    duration<double> time_span =
        duration_cast<duration<double>>(t2 - t1); // duration

    // output result
    for (int cid = 0; cid < k; cid++) {
        for (auto it = clusters[cid].begin(); it != clusters[cid].end(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    }
    string time_filename = "nb_time.txt"; // name of the following file
    fstream time_fs(time_filename.c_str(),
                    fstream::out |
                        fstream::app);    // file record the time consumption
    time_fs << time_span.count() << endl; // output time consumption
    time_fs.close();
    return 0;
}
