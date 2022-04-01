#ifndef GKXX_WHEEL_NORMAL_ITER_HPP
#define GKXX_WHEEL_NORMAL_ITER_HPP

#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace gkxx {

template <typename Iter, typename Ctn>
class Checked_normal_iter {
 private:
  using __traits_type = std::iterator_traits<Iter>;
  Iter cur, begin, end;
  void check_dereferencible(const std::string &msg) const {
    check_dereferencible(cur, msg);
  }
  void check_dereferencible(Iter pos, const std::string &msg) const {
    if (pos >= end || pos < begin)
      throw std::out_of_range(msg);
  }
  void check_range(const std::string &msg) const {
    check_range(cur, msg);
  }
  void check_range(Iter pos, const std::string &msg) const {
    if (pos > end || pos < begin)
      throw std::out_of_range(msg);
  }

 public:
  using iterator_type = Iter;
  using iterator_category = typename __traits_type::iterator_category;
  using pointer = typename __traits_type::pointer;
  using reference = typename __traits_type::reference;
  using difference_type = typename __traits_type::difference_type;
  using value_type = typename __traits_type::value_type;

 public:
  Checked_normal_iter() = default;
  explicit Checked_normal_iter(const Iter &it, const Iter &be, const Iter &ed)
      : cur(it), begin(be), end(ed) {}
  // Conversion from iterator to const_iterator
  template <typename Other>
  Checked_normal_iter(
      const Checked_normal_iter<
          Other,
          typename std::enable_if<
              std::is_same<Other, typename Ctn::pointer>::value, Ctn>::type> &t)
      : cur(t.base()), begin(t.first()), end(t.pass_end()) {}
  const Iter &base() const {
    return cur;
  }
  const Iter &first() const {
    return begin;
  }
  const Iter &pass_end() const {
    return end;
  }
  reference operator*() const {
    check_dereferencible(
        "Dereferencing an iterator that is not dereferencible!");
    return *cur;
  }
  pointer operator->() const {
    return &this->operator*();
  }
  Checked_normal_iter &operator++() {
    check_range(cur + 1, "Iterator out of range after increment.");
    ++cur;
    return *this;
  }
  Checked_normal_iter operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }
  Checked_normal_iter &operator--() {
    check_range(cur - 1, "Iterator out of range after decrement.");
    --cur;
    return *this;
  }
  Checked_normal_iter operator--(int) {
    auto tmp = *this;
    --*this;
    return tmp;
  }
  reference operator[](difference_type n) const {
    check_dereferencible(cur + n,
                         "operator[] on iterator with subscript out of range.");
    return cur[n];
  }
  Checked_normal_iter &operator+=(difference_type n) {
    check_range(cur + n, "Iterator out of range after moving by operator+=.");
    cur += n;
    return *this;
  }
  Checked_normal_iter operator+(difference_type n) const {
    auto tmp = *this;
    tmp.operator+=(n);
    return tmp;
  }
  Checked_normal_iter &operator-=(difference_type n) {
    check_range(cur - n, "Iterator out of range after moving by operator-=.");
    cur -= n;
    return *this;
  }
  Checked_normal_iter operator-(difference_type n) const {
    auto tmp = *this;
    tmp.operator-=(n);
    return tmp;
  }
};

template <typename IterL, typename IterR, typename Ctn>
inline bool operator==(const Checked_normal_iter<IterL, Ctn> &lhs,
                       const Checked_normal_iter<IterR, Ctn> &rhs) {
  return lhs.base() == rhs.base();
}
template <typename Iter, typename Ctn>
inline bool operator==(const Checked_normal_iter<Iter, Ctn> &lhs,
                       const Checked_normal_iter<Iter, Ctn> &rhs) {
  return lhs.base() == rhs.base();
}

template <typename IterL, typename IterR, typename Ctn>
inline bool operator!=(const Checked_normal_iter<IterL, Ctn> &lhs,
                       const Checked_normal_iter<IterR, Ctn> &rhs) {
  return lhs.base() != rhs.base();
}
template <typename Iter, typename Ctn>
inline bool operator!=(const Checked_normal_iter<Iter, Ctn> &lhs,
                       const Checked_normal_iter<Iter, Ctn> &rhs) {
  return lhs.base() != rhs.base();
}

template <typename IterL, typename IterR, typename Ctn>
inline bool operator<(const Checked_normal_iter<IterL, Ctn> &lhs,
                      const Checked_normal_iter<IterR, Ctn> &rhs) {
  return lhs.base() < rhs.base();
}
template <typename Iter, typename Ctn>
inline bool operator<(const Checked_normal_iter<Iter, Ctn> &lhs,
                      const Checked_normal_iter<Iter, Ctn> &rhs) {
  return lhs.base() < rhs.base();
}

template <typename IterL, typename IterR, typename Ctn>
inline bool operator<=(const Checked_normal_iter<IterL, Ctn> &lhs,
                       const Checked_normal_iter<IterR, Ctn> &rhs) {
  return lhs.base() <= rhs.base();
}
template <typename Iter, typename Ctn>
inline bool operator<=(const Checked_normal_iter<Iter, Ctn> &lhs,
                       const Checked_normal_iter<Iter, Ctn> &rhs) {
  return lhs.base() <= rhs.base();
}

template <typename IterL, typename IterR, typename Ctn>
inline bool operator>(const Checked_normal_iter<IterL, Ctn> &lhs,
                      const Checked_normal_iter<IterR, Ctn> &rhs) {
  return lhs.base() > rhs.base();
}
template <typename Iter, typename Ctn>
inline bool operator>(const Checked_normal_iter<Iter, Ctn> &lhs,
                      const Checked_normal_iter<Iter, Ctn> &rhs) {
  return lhs.base() > rhs.base();
}

template <typename IterL, typename IterR, typename Ctn>
inline bool operator>=(const Checked_normal_iter<IterL, Ctn> &lhs,
                       const Checked_normal_iter<IterR, Ctn> &rhs) {
  return lhs.base() >= rhs.base();
}
template <typename Iter, typename Ctn>
inline bool operator>=(const Checked_normal_iter<Iter, Ctn> &lhs,
                       const Checked_normal_iter<Iter, Ctn> &rhs) {
  return lhs.base() >= rhs.base();
}

template <typename IterL, typename IterR, typename Ctn>
inline auto operator-(const Checked_normal_iter<IterL, Ctn> &lhs,
                      const Checked_normal_iter<IterR, Ctn> &rhs) {
  return lhs.base() - rhs.base();
}
template <typename Iter, typename Ctn>
inline auto operator-(const Checked_normal_iter<Iter, Ctn> &lhs,
                      const Checked_normal_iter<Iter, Ctn> &rhs) {
  return lhs.base() - rhs.base();
}

template <typename Iter, typename Ctn>
inline auto
operator+(typename Checked_normal_iter<Iter, Ctn>::difference_type n,
          const Checked_normal_iter<Iter, Ctn> &t) {
  auto tmp = t;
  tmp += n;
  return tmp;
}

} // namespace gkxx

#endif // GKXX_WHEEL_NORMAL_ITER_HPP