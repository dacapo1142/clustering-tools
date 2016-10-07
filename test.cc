#include "containers/disjoint_sets.h"
#include <iostream>
#include <typeinfo>
#include <vector>
using namespace std;

int main() {
    int a[] = {1, 0, 1, 1, 2};
    DisjointSets s(5, 3, a, a + 5);
    s.print();
    cout << s.empty(0);
    return 0;
}
