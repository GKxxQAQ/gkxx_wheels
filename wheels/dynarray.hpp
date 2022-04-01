#ifndef GKXX_WHEEL_DYNARRAY_HPP
#define GKXX_WHEEL_DYNARRAY_HPP

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <wheels/normal_iter.hpp>
#include <wheels/range.hpp>

namespace gkxx {

template <typename T>
class Dynarray {
 public:
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T &;
  using const_reference = const T &;
  using pointer = T *;
  using const_pointer = const T *;
  using value_type = T;
  using iterator = Checked_normal_iter<pointer, Dynarray>;
  using const_iterator = Checked_normal_iter<const_pointer, Dynarray>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // copy control
  Dynarray() : nSize(0), data(nullptr) {}
  Dynarray(const Dynarray &t) : nSize(t.nSize), data(new T[nSize]) {
    std::copy(t.data, t.data + nSize, data);
  }
  Dynarray &operator=(const Dynarray &t) {
    Dynarray temp = t;
    swap(temp);
    return *this;
  }
  Dynarray(Dynarray &&t) noexcept : nSize(t.nSize), data(t.data) {
    t.nSize = 0;
    t.data = nullptr;
  }
  Dynarray &operator=(Dynarray &&t) noexcept {
    if (this != &t) {
      nSize = t.nSize;
      delete[] data;
      data = t.data;
      t.data = nullptr;
      t.nSize = 0;
    }
    return *this;
  }
  ~Dynarray() {
    delete[] data;
  }
  void swap(Dynarray<T> &t) {
    using std::swap;
    swap(nSize, t.nSize);
    swap(data, t.data);
  }

  // other constructors
  template <typename ForwardIter>
  explicit Dynarray(ForwardIter first, ForwardIter last)
      : nSize(std::distance(first, last)), data(new T[nSize]) {
    std::copy(first, last, data);
  }
  explicit Dynarray(size_type n) : nSize(n), data(new T[n]()) {}
  explicit Dynarray(size_type n, const T &x) : Dynarray(n) {
    std::fill(data, data + nSize, x);
  }
  Dynarray(std::initializer_list<T> il) : nSize(il.size()), data(new T[nSize]) {
    std::copy(il.begin(), il.end(), data);
  }
  Dynarray(Range r) : nSize(r.size()), data(new T[nSize]) {
    std::copy(r.begin(), r.end(), data);
  }

  // elements accessing
  // Negative subscript is not allowed.
  // All accessing operations check boundary.
  reference operator[](size_type n) {
    return const_cast<reference>(
        static_cast<const Dynarray &>(*this).operator[](n));
  }
  const_reference operator[](size_type n) const {
    check_if_out(n, "Subscript of Dynarray out of range.");
    return data[n];
  }
  reference at(size_type n) {
    return const_cast<reference>(static_cast<const Dynarray &>(*this).at(n));
  }
  const_reference at(size_type n) const {
    check_if_out(n, "Call to Dynarray::at with subscript out of range.");
    return data[n];
  }
  reference front() {
    return const_cast<reference>(static_cast<const Dynarray &>(*this).front());
  }
  const_reference front() const {
    check_if_out(0, "front() on empty Dynarray.");
    return data[0];
  }
  reference back() {
    return const_cast<reference>(static_cast<const Dynarray &>(*this).back());
  }
  const_reference back() const {
    check_if_out(0, "back() on empty Dynarray.");
    return data[nSize - 1];
  }
  size_type size() const {
    return nSize;
  }
  bool empty() const {
    return !nSize;
  }

  // iterators
  iterator begin() {
    return iterator(data, data, data + nSize);
  }
  iterator end() {
    return iterator(data + nSize, data, data + nSize);
  }
  const_iterator begin() const {
    return const_iterator(data, data, data + nSize);
  }
  const_iterator end() const {
    return const_iterator(data + nSize, data, data + nSize);
  }
  const_iterator cbegin() const {
    return begin();
  }
  const_iterator cend() const {
    return end();
  }
  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }
  reverse_iterator rend() {
    return reverse_iterator(begin());
  }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator crbegin() const {
    return rbegin();
  }
  const_reverse_iterator crend() const {
    return rend();
  }

  // broadcast operations
  template <typename Op>
  void binary_broadcast(const Dynarray<T> &t, Op f);
  template <typename Op>
  void binary_broadcast(const T &t, Op f) {
    std::for_each(data, data + nSize, [t, f](T &a) { f(a, t); });
  }

  Dynarray &operator+=(const Dynarray &);
  Dynarray &operator+=(const T &);

  Dynarray &operator-=(const Dynarray &);
  Dynarray &operator-=(const T &);

  Dynarray &operator*=(const Dynarray &);
  Dynarray &operator*=(const T &);

  Dynarray &operator/=(const Dynarray &);
  Dynarray &operator/=(const T &);

  Dynarray &operator%=(const Dynarray &);
  Dynarray &operator%=(const T &);

  Dynarray &operator<<=(const Dynarray &);
  Dynarray &operator<<=(const T &);

  Dynarray &operator>>=(const Dynarray &);
  Dynarray &operator>>=(const T &);

  Dynarray &operator&=(const Dynarray &);
  Dynarray &operator&=(const T &);

  Dynarray &operator^=(const Dynarray &);
  Dynarray &operator^=(const T &);

  Dynarray &operator|=(const Dynarray &);
  Dynarray &operator|=(const T &);

  // slice
  // Negative subscript is not allowed
  Dynarray operator()(Range r) const {
    return do_slice(r, "Invalid slice range (aka \'operator()\') on Dynarray.");
  }
  Dynarray operator()(int l, int r, int k = 1) const {
    return operator()(Range(l, r, k));
  }
  Dynarray slice(Range r) const {
    return do_slice(r, "Invalid slice range on Dynarray.");
  }
  Dynarray slice(int l, int r, int k = 1) const {
    return slice(Range(l, r, k));
  }

  // size operations
  void assign(const Dynarray &d) {
    this->operator=(d);
  }
  void resize(size_type n, const T &x = T()) {
    if (n < nSize)
      assign(Dynarray<T>(n));
    else if (n > nSize) {
      Dynarray<T> tmp(n);
      std::copy(data, data + nSize, tmp.data);
      std::fill(tmp.data + nSize, tmp.data + n, x);
      assign(tmp);
    }
  }

 private:
  size_type nSize;
  pointer data;
  void check_if_out(size_type n, const std::string &msg) const {
    if (n >= nSize)
      throw std::out_of_range(msg);
  }
  void check_if_out(Range r, const std::string &msg) const {
    if (r.first() < 0 || r.first() >= nSize)
      throw std::out_of_range(msg);
    if (r.last() < 0 || r.last() >= nSize)
      throw std::out_of_range(msg);
  }
  Dynarray do_slice(Range, const std::string &) const;
};

template <typename T>
template <typename Op>
void Dynarray<T>::binary_broadcast(const Dynarray<T> &t, Op f) {
  if (nSize >= t.nSize) {
    for (size_type i = 0; i < t.nSize; ++i)
      f(data[i], t.data[i]);
  } else {
    T *p = new T[t.nSize]();
    std::copy(data, data + nSize, p);
    for (size_type i = 0; i < t.nSize; ++i)
      f(p[i], t.data[i]);
    delete[] data;
    data = p;
    nSize = t.nSize;
  }
}

template <typename T>
Dynarray<T> Dynarray<T>::do_slice(Range r, const std::string &msg) const {
  Dynarray d(r.size());
  check_if_out(r, msg);
  int i = 0;
  for (int x : r)
    d[i++] = data[x];
  return d;
}

template <typename T>
inline void swap(Dynarray<T> &lhs, Dynarray<T> &rhs) {
  lhs.swap(rhs);
}

template <typename T>
inline bool operator==(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  return lhs.size() == rhs.size() &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T>
inline bool operator!=(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  return !(lhs == rhs);
}

template <typename T>
inline bool operator<(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end());
}

template <typename T>
inline bool operator>(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(),
                                      lhs.end());
}

template <typename T>
inline bool operator<=(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  return !(lhs > rhs);
}

template <typename T>
inline bool operator>=(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  return !(lhs < rhs);
}

template <typename T>
inline Dynarray<T> operator-(const Dynarray<T> &dt) {
  Dynarray<T> ret = dt;
  std::for_each(ret.begin(), ret.end(), [](T &t) { t = -t; });
  return ret;
}

template <typename T>
inline Dynarray<T> operator~(const Dynarray<T> &dt) {
  Dynarray<T> ret = dt;
  std::for_each(ret.begin(), ret.end(), [](T &t) { t = ~t; });
  return ret;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator+=(const Dynarray<T> &t) {
  binary_broadcast(t, [](T &a, const T &b) { a += b; });
  return *this;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator+=(const T &t) {
  binary_broadcast(t, [](T &a, const T &b) { a += b; });
  return *this;
}

template <typename T>
inline Dynarray<T> operator+(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret += rhs;
}

template <typename T>
inline Dynarray<T> operator+(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret += rhs;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator-=(const Dynarray<T> &t) {
  return this->operator+=(-t);
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator-=(const T &t) {
  return this->operator+=(-t);
}

template <typename T>
inline Dynarray<T> operator-(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret -= rhs;
}

template <typename T>
inline Dynarray<T> operator-(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret -= rhs;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator*=(const Dynarray<T> &t) {
  binary_broadcast(t, [](T &a, const T &b) { a *= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator*=(const T &t) {
  binary_broadcast(t, [](T &a, const T &b) { a *= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> operator*(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret *= rhs;
}

template <typename T>
inline Dynarray<T> operator*(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret *= rhs;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator/=(const Dynarray<T> &t) {
  binary_broadcast(t, [](T &a, const T &b) { a /= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator/=(const T &t) {
  binary_broadcast(t, [](T &a, const T &b) { a /= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> operator/(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret /= rhs;
}

template <typename T>
inline Dynarray<T> operator/(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret /= rhs;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator%=(const Dynarray<T> &t) {
  binary_broadcast(t, [](T &a, const T &b) { a %= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator%=(const T &t) {
  binary_broadcast(t, [](T &a, const T &b) { a %= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> operator%(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret %= rhs;
}

template <typename T>
inline Dynarray<T> operator%(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret %= rhs;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator&=(const Dynarray<T> &t) {
  binary_broadcast(t, [](T &a, const T &b) { a &= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator&=(const T &t) {
  binary_broadcast(t, [](T &a, const T &b) { a &= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> operator&(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret &= rhs;
}

template <typename T>
inline Dynarray<T> operator&(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret &= rhs;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator^=(const Dynarray<T> &t) {
  binary_broadcast(t, [](T &a, const T &b) { a ^= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator^=(const T &t) {
  binary_broadcast(t, [](T &a, const T &b) { a ^= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> operator^(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret ^= rhs;
}

template <typename T>
inline Dynarray<T> operator^(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret ^= rhs;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator|=(const Dynarray<T> &t) {
  binary_broadcast(t, [](T &a, const T &b) { a |= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator|=(const T &t) {
  binary_broadcast(t, [](T &a, const T &b) { a |= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> operator|(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret |= rhs;
}

template <typename T>
inline Dynarray<T> operator|(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret |= rhs;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator<<=(const Dynarray<T> &t) {
  binary_broadcast(t, [](T &a, const T &b) { a <<= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator<<=(const T &t) {
  binary_broadcast(t, [](T &a, const T &b) { a <<= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> operator<<(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret <<= rhs;
}

template <typename T>
inline Dynarray<T> operator<<(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret <<= rhs;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator>>=(const Dynarray<T> &t) {
  binary_broadcast(t, [](T &a, const T &b) { a >>= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> &Dynarray<T>::operator>>=(const T &t) {
  binary_broadcast(t, [](T &a, const T &b) { a >>= b; });
  return *this;
}

template <typename T>
inline Dynarray<T> operator>>(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  Dynarray<T> ret = lhs;
  return ret >>= rhs;
}

template <typename T>
inline Dynarray<T> operator>>(const Dynarray<T> &lhs, const T &rhs) {
  Dynarray<T> ret = lhs;
  return ret >>= rhs;
}

template <typename T>
inline void print(const Dynarray<T> &t, std::ostream &os = std::cout,
                  const std::string &sep = " ", const std::string &end = "\n") {
  for (auto it = t.begin(); it != t.end(); ++it) {
    auto nxt = it;
    if (++nxt != t.end())
      os << *it << sep;
    else
      os << *it;
  }
  os << end << std::flush;
}

template <typename T>
inline Dynarray<T> cross(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  if (lhs.size() != rhs.size())
    throw std::invalid_argument(
        "Cross product of vectors with different dimensions.");
  if (lhs.size() == 3)
    return Dynarray<T>({lhs.data[1] * rhs.data[2] - lhs.data[2] - rhs.data[1],
                        lhs.data[2] * rhs.data[0] - lhs.data[0] * rhs.data[2],
                        lhs.data[0] * rhs.data[1] - lhs.data[1] * rhs.data[0]});
  throw std::invalid_argument("Could not compute the cross product of "
                              "vectors whose dimension is not 3.");
}

template <typename T>
inline T dot(const Dynarray<T> &lhs, const Dynarray<T> &rhs) {
  if (lhs.size() != rhs.size())
    throw std::invalid_argument(
        "Dot product of vectors with different dimensions.");
  T res = T();
  for (auto i = lhs.begin(), j = rhs.begin(); i != lhs.end(); ++i, ++j)
    res += *i * *j;
  return res;
}

} // namespace gkxx

#endif // GKXX_WHEEL_DYNARRAY_HPP