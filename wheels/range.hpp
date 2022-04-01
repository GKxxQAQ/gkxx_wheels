#ifndef GKXX_WHEEL_RANGE_HPP
#define GKXX_WHEEL_RANGE_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <stdexcept>

// size = max(0, ceil(1.0 * (right - left) / step))
// passend = left + size * step

namespace gkxx {

class Range;

class RangeIter {
  friend bool operator==(const RangeIter &, const RangeIter &);
  friend bool operator!=(const RangeIter &, const RangeIter &);

 public:
  using iterator_category = std::forward_iterator_tag;
  using iterator_type = RangeIter;
  using value_type = int;
  using difference_type = int;
  using reference = const int &;
  using pointer = const int *;

  RangeIter(int pos, int st) : cur(pos), step(st) {}
  RangeIter() : cur(0), step(1) {}
  RangeIter(const RangeIter &) = default;
  RangeIter &operator=(const RangeIter &) = default;
  ~RangeIter() = default;
  int operator*() const {
    return cur;
  }
  pointer operator->() const {
    throw std::logic_error("operator-> not allowed on RangeIter.");
  }
  RangeIter &operator++() {
    cur += step;
    return *this;
  }
  RangeIter operator++(int) {
    auto tmp = *this;
    this->operator++();
    return tmp;
  }
  void swap(RangeIter &t) {
    using std::swap;
    swap(cur, t.cur);
    swap(step, t.step);
  }

 private:
  int cur;
  int step;
};

inline void swap(RangeIter &lhs, RangeIter &rhs) {
  lhs.swap(rhs);
}

inline bool operator==(const RangeIter &lhs, const RangeIter &rhs) {
  return lhs.cur == rhs.cur && lhs.step == rhs.step;
}

inline bool operator!=(const RangeIter &lhs, const RangeIter &rhs) {
  return !(lhs == rhs);
}

class Range {
 public:
  using value_type = int;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = RangeIter;
  using const_iterator = iterator;
  using size_type = std::size_t;
  using difference_type = int;

  Range(int l, int r, int k = 1)
      : beginwith(l), length(std::max(0.0, std::ceil(1.0 * (r - l) / k))),
        passend(l + length * k), step(k) {}
  Range(int n) : Range(0, n) {}
  iterator begin() const {
    return RangeIter(beginwith, step);
  }
  iterator end() const {
    return RangeIter(passend, step);
  }
  const_iterator cbegin() const {
    return begin();
  }
  const_iterator cend() const {
    return end();
  }
  size_type size() const {
    return length;
  }
  const int first() const {
    return beginwith;
  }
  const int last() const {
    return passend - step;
  }

 private:
  const int beginwith;
  const size_type length;
  const int passend, step;
};

} // namespace gkxx

#endif // GKXX_WHEEL_RANGE_HPP