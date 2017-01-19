#include "clusters.hpp"
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;
using namespace std::chrono;
int main(int argc, char *argv[]) {
    const char *filename = argv[1];
    size_t n = atoi(argv[2]);
    double lambda0 = atof(argv[3]);
    double lambda1 = atof(argv[4]);

    ifstream file(filename);
    Clusters clusters(n, n, file, lambda0, lambda1);
    file.close();
    const char *time_filename = argv[5];
    ofstream metadata(time_filename);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    clusters.routine();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    metadata << time_span.count() << endl;
    metadata.close();
    clusters.print_communities();
    ofstream file_size(argv[6]);
    ofstream file_iter(argv[7]);
    clusters.print_size(file_size);
    clusters.print_iter(file_iter);
    file_size.close();
    file_iter.close();
    return 0;
}
