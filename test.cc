#include "containers/adjoint_set.h"
#include <iostream>
#include <typeinfo>
#include <vector>
using namespace std;
template <typename T1> void test(T1 t1, T1 t2) {
    for (T1 t = t1; t != t2; t++) {
        cout << *t << " ";
    }
    cout << "\n";
}

int main() {
    int a[] = {1, 0, 1, 1, 2};
    AdjointSet s(5, 3, a, a + 5);
    s.print();
    vector<int> v(a, a + 5);
    test(a, a + 5);
    return 0;
}
