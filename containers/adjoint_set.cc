#include "adjoint_set.h"
#include <iostream>

template <typename T1, typename T2>
AdjointSet::AdjointSet(size_t _n, size_t _k, T1 a, T2 b) : n(_n), k(_k) {}
