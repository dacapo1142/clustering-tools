#ifndef ADJOINT_SET_H
#define ADJOINT_SET_H

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

class AdjointSet {
  public:
    class Node {
      public:
        size_t next_id;
        size_t prev_id;
    };
    size_t n;
    size_t k;
    template <typename T1, typename T2>
    AdjointSet(size_t _n, size_t _k, T1 a, T2 b);
};

#include "adjoint_set.cc"
#endif
