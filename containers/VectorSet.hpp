#ifndef VECTORSET_HPP
#define VECTORSET_HPP
#include <limits>
#include <vector>

class VectorSet {
  private:
    std::vector<size_t> _list;
    std::vector<size_t> _position;
    size_t _size;
    const static size_t NONE;

  public:
    VectorSet(size_t max_length)
        : _list(max_length), _position(max_length, NONE), _size(0){};
    inline void insert(size_t vid) {
        if (_position[vid] == NONE) {
            _list[_size] = vid;
            _position[vid] = _size;
            _size++;
        }
        return;
    }
    inline void erase(size_t vid) {
        if (_position[vid] != NONE) {
            size_t vid_pos = _position[vid];
            if (vid_pos != _size - 1) {
                size_t last_val = _list[_size - 1];
                std::swap(_list[_size - 1], _list[vid_pos]);
                _position[last_val] = vid_pos;
            }
            _position[vid] = NONE;
            _size--;
        }
    }
    inline auto begin() { return _list.begin(); }
    inline auto end() { return _list.begin() + _size; }
    auto find(const size_t &vid) {
        if (_position[vid] != NONE) {
            return this->begin() + _position[vid];
        } else {
            return this->end();
        }
    }
    inline auto &operator[](size_t i) { return _list[i]; }
    inline size_t size() { return _size; }
    inline void clear() {
        while (_size != 0) {
            erase(_list[_size - 1]);
        }
    }
};

const size_t VectorSet::NONE = std::numeric_limits<size_t>::max();

#endif
