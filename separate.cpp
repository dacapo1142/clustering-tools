#include "clusters.hpp"
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;
using namespace std::chrono;
int main(int argc, char *argv[]) {
    const char *filename = argv[1];
    size_t n = atoi(argv[2]);
    string ofname = string(argv[3]);
    ifstream file(filename);
    Clusters clusters(n, n, file, Clusters::InputFormat::TWO_COLOUMN);
    file.close();

    bool improve = true;
    int count = 0;

    while (improve) {
        improve = clusters.partition_procedure(
            Clusters::PartitionMethod::PARTITION_INPUT);
        if (improve) {
            clusters.NodeAggregate();
            string name = ofname + to_string(count);
            ofstream f(name.c_str());
            clusters.print_communities(f);
            f.close();
            count++;
        } else {
            break;
        }
    }

    return 0;
}
