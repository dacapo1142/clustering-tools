#ifndef VECTORSET_HPP
#define VECTORSET_HPP
#include <cstddef>
#include <limits>
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
    inline void initial_full() {
        _size = _capacity;
        for (size_t vid = 0; vid < _capacity; vid++) {
            _data[vid] = vid;
            _index[vid] = vid;
        }
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
    inline auto end() { return _data.begin() + _size; }
    auto find(const size_t &vid) {
        if (_index[vid] != NONE) {
            return this->begin() + _index[vid];
        } else {
            return this->end();
        }
    }
    inline bool contain(const size_t &vid) { return _index[vid] != NONE; }
    inline auto &operator[](size_t i) { return _data[i]; }
    inline size_t size() { return _size; }
    inline void clear() {
        for (auto &v : *this) {
            _index[v] = NONE;
        }
        _size = 0;
    }
    inline size_t position(const size_t &vid) { return _index[vid]; }
};

const size_t VectorSet::NONE = std::numeric_limits<size_t>::max();

#endif
