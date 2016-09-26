#include "containers/adjoint_set.h"
#include <iostream>
using namespace std;
int main() {
    int a[] = {1, 0, 1, 1, 2};
    AdjointSet s(5, 3, a, a + 5);
    cout << s.n << endl;
    return 0;
}
