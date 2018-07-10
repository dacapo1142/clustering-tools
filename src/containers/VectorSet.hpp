#ifndef VECTORSET_HPP
#define VECTORSET_HPP
#include <cstddef>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

class VectorSet {
  private:
    std::vector<size_t> _data;
    std::vector<size_t> _index;
    size_t _size;
    const static size_t NONE;
    size_t _capacity;

  public:
    VectorSet(size_t max_length)
        : _data(max_length), _index(max_length, NONE), _size(0),
          _capacity(max_length){};
    template <typename T>
    VectorSet(size_t max_length, T cid_begin, T cid_end)
        : _data(max_length), _index(max_length, NONE), _size(0),
          _capacity(max_length) {
        for (auto v = cid_begin; v != cid_end; v++) {
            insert(*v);
        }
    };
    inline void initial_full() {
        _size = _capacity;
        std::iota(_data.begin(), _data.end(), 0);
        std::iota(_index.begin(), _index.end(), 0);
    }
    inline void insert(size_t vid) {
        if (_index[vid] == NONE) {
            _data[_size] = vid;
            _index[vid] = _size;
            _size++;
        }
        return;
    }
    inline void erase(size_t vid) {
        if (_index[vid] != NONE) {
            size_t vid_pos = _index[vid];
            if (vid_pos != _size - 1) {
                size_t last_val = _data[_size - 1];
                std::swap(_data[_size - 1], _data[vid_pos]);
                _index[last_val] = vid_pos;
            }
            _index[vid] = NONE;
            _size--;
        }
    }
    inline auto begin() { return _data.begin(); }
    inline auto begin() const { return _data.begin(); }
    inline auto end() { return _data.begin() + _size; }
    inline auto end() const { return _data.begin() + _size; }

    auto find(size_t vid) {
        if (_index[vid] != NONE) {
            return this->begin() + _index[vid];
        } else {
            return this->end();
        }
    }
    inline bool contain(size_t vid) { return _index[vid] != NONE; }
    inline auto &operator[](size_t i) { return _data[i]; }
    // inline auto &operator[](size_t i) const { return _data[i]; }
    
    inline size_t size() const { return _size; }
    inline void clear() {
        for (auto &v : *this) {
            _index[v] = NONE;
        }
        _size = 0;
    }
    inline size_t relabel(size_t vid) const { return _index[vid]; }
    inline size_t relabel_inv(size_t vid) const { return _data[vid]; }
};

const size_t VectorSet::NONE = std::numeric_limits<size_t>::max();
std::ostream &operator<<(std::ostream &os, const VectorSet &vs) {
    for (const auto &v : vs) {
        os << v << " ";
    }
    return os;
}
#endif
