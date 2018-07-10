#include "clusters.hpp"
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;
using namespace std::chrono;
int main(int argc, char *argv[]) {
    //disabling sync can increase I/O speed
    std::ios::sync_with_stdio(false);

    //input edgelist
    const char *filename = argv[1];
    ifstream file(filename);

    //input number of vertices
    size_t n = atoi(argv[2]);

    //initialize
    Clusters clusters(n, n, file, Clusters::InputFormat::TWO_COLOUMN);
    file.close();

    //benchmark and performing the main algorithm
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    clusters.routine();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    //output the duration time
    const char *time_filename = argv[3];
    ofstream metadata(time_filename);
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    metadata << time_span.count() << endl;
    metadata.close();

    //output outcome
    clusters.print_communities();

    //output size of p-aggregate graph in each depth
    ofstream file_size(argv[4]);
    clusters.print_size(file_size);
    file_size.close();
    
    //output the recursive depth
    ofstream file_iter(argv[5]);
    clusters.print_iter(file_iter);
    file_iter.close();

    return 0;
}
