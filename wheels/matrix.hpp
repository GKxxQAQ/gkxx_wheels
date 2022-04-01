#ifndef GKXX_WHEEL_MATRIX_HPP
#define GKXX_WHEEL_MATRIX_HPP

#include <initializer_list>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <type_traits>
#include <wheels/dynarray.hpp>

namespace gkxx {

template <typename T>
class Matrix;

template <typename T, typename Ref, typename Ptr>
class __Mat_ele_iter {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = typename Matrix<T>::elements_type;
  using reference = Ref;
  using pointer = Ptr;

 private:
  using _Self = __Mat_ele_iter<T, Ref, Ptr>;
  using size_type = typename Matrix<T>::size_type;
  Matrix<T> *m_pmat;
  size_type m_cur;

  void check_dereferencible(const std::string &msg) const {
    check_dereferencible(m_cur, msg);
  }
  void check_dereferencible(size_type pos, const std::string &msg) const {
    if (pos >= m_pmat->element_count())
      throw std::out_of_range(msg);
  }
  void check_range(const std::string &msg) const {
    check_range(m_cur, msg);
  }
  void check_range(size_type pos, const std::string &msg) const {
    if (pos > m_pmat->element_count())
      throw std::out_of_range(msg);
  }
  void convert(size_type pos, size_type &r, size_type &c) const {
    r = pos / m_pmat->column_count();
    c = pos % m_pmat->column_count();
  }
  void convert(size_type &r, size_type &c) const {
    convert(m_cur, r, c);
  }

 public:
  __Mat_ele_iter(Matrix<T> *pm, size_type cur) : m_pmat(pm), m_cur(cur) {}
  __Mat_ele_iter() : m_pmat(nullptr), m_cur(0) {}
  template <
      typename Iter,
      typename = typename std::enable_if<
          std::is_same<_Self, __Mat_ele_iter<T, const value_type &,
                                             const value_type *>>::value &&
              std::is_same<
                  Iter, __Mat_ele_iter<T, value_type &, value_type *>>::value,
          void>::type>
  __Mat_ele_iter(const Iter &t)
      : m_pmat(t.matrix_pointer()), m_cur(t.index()) {}
  Matrix<T> *matrix_pointer() const {
    return m_pmat;
  }
  size_type index() const {
    return m_cur;
  }
  reference operator*() const {
    check_dereferencible("Dereferencing a Matrix elements' iterator that "
                         "is not dereferencible!");
    size_type r, c;
    convert(r, c);
    return (*m_pmat)[r][c];
  }
  pointer operator->() const {
    return &this->operator*();
  }
  _Self &operator++() {
    check_range(m_cur + 1,
                "Matrix elements' iterator out of range after increment.");
    ++m_cur;
    return *this;
  }
  _Self operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }
  _Self &operator--() {
    check_range(m_cur - 1,
                "Matrix elements' iterator out of range after decrement.");
    --m_cur;
    return *this;
  }
  _Self operator--(int) {
    auto tmp = *this;
    --*this;
    return tmp;
  }
  reference operator[](difference_type n) const {
    check_dereferencible(m_cur + n,
                         "operator[] on Matrix elements' iterator with "
                         "subscript out of range.");
    size_type r, c;
    convert(m_cur + n, r, c);
    return (*m_pmat)[r][c];
  }
  _Self &operator+=(difference_type n) {
    check_range(m_cur + n, "Matrix elements' iterator out of range after "
                           "moving by operator+=.");
    m_cur += n;
    return *this;
  }
  _Self operator+(difference_type n) const {
    auto tmp = *this;
    tmp += n;
    return tmp;
  }
  _Self &operator-=(difference_type n) {
    check_range(m_cur - n, "Matrix elements' iterator out of "
                           "range after moving by operator-=.");
    m_cur -= n;
    return *this;
  }
  _Self operator-(difference_type n) const {
    auto tmp = *this;
    tmp -= n;
    return tmp;
  }

  template <typename R, typename P>
  void __check_diff(const __Mat_ele_iter<T, R, P> &t,
                    const std::string &msg) const {
    if (t.matrix_pointer() != matrix_pointer())
      throw std::invalid_argument(msg);
  }
};

template <typename T, typename RefL, typename PtrL, typename RefR,
          typename PtrR>
inline bool operator==(const __Mat_ele_iter<T, RefL, PtrL> &lhs,
                       const __Mat_ele_iter<T, RefR, PtrR> &rhs) {
  lhs.__check_diff(rhs, "operator== on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() == rhs.index();
}

template <typename T, typename Ref, typename Ptr>
inline bool operator==(const __Mat_ele_iter<T, Ref, Ptr> &lhs,
                       const __Mat_ele_iter<T, Ref, Ptr> &rhs) {
  lhs.__check_diff(rhs, "operator== on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() == rhs.index();
}

template <typename T, typename RefL, typename PtrL, typename RefR,
          typename PtrR>
inline bool operator!=(const __Mat_ele_iter<T, RefL, PtrL> &lhs,
                       const __Mat_ele_iter<T, RefR, PtrR> &rhs) {
  lhs.__check_diff(rhs, "operator1= on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() != rhs.index();
}

template <typename T, typename Ref, typename Ptr>
inline bool operator!=(const __Mat_ele_iter<T, Ref, Ptr> &lhs,
                       const __Mat_ele_iter<T, Ref, Ptr> &rhs) {
  lhs.__check_diff(rhs, "operator!= on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() != rhs.index();
}

template <typename T, typename RefL, typename PtrL, typename RefR,
          typename PtrR>
inline bool operator<(const __Mat_ele_iter<T, RefL, PtrL> &lhs,
                      const __Mat_ele_iter<T, RefR, PtrR> &rhs) {
  lhs.__check_diff(rhs, "operator< on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() < rhs.index();
}

template <typename T, typename Ref, typename Ptr>
inline bool operator<(const __Mat_ele_iter<T, Ref, Ptr> &lhs,
                      const __Mat_ele_iter<T, Ref, Ptr> &rhs) {
  lhs.__check_diff(rhs, "operator< on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() < rhs.index();
}

template <typename T, typename RefL, typename PtrL, typename RefR,
          typename PtrR>
inline bool operator<=(const __Mat_ele_iter<T, RefL, PtrL> &lhs,
                       const __Mat_ele_iter<T, RefR, PtrR> &rhs) {
  lhs.__check_diff(rhs, "operator<= on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() <= rhs.index();
}

template <typename T, typename Ref, typename Ptr>
inline bool operator<=(const __Mat_ele_iter<T, Ref, Ptr> &lhs,
                       const __Mat_ele_iter<T, Ref, Ptr> &rhs) {
  lhs.__check_diff(rhs, "operator<= on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() <= rhs.index();
}

template <typename T, typename RefL, typename PtrL, typename RefR,
          typename PtrR>
inline bool operator>(const __Mat_ele_iter<T, RefL, PtrL> &lhs,
                      const __Mat_ele_iter<T, RefR, PtrR> &rhs) {
  lhs.__check_diff(rhs, "operator> on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() > rhs.index();
}

template <typename T, typename Ref, typename Ptr>
inline bool operator>(const __Mat_ele_iter<T, Ref, Ptr> &lhs,
                      const __Mat_ele_iter<T, Ref, Ptr> &rhs) {
  lhs.__check_diff(rhs, "operator> on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() > rhs.index();
}

template <typename T, typename RefL, typename PtrL, typename RefR,
          typename PtrR>
inline bool operator>=(const __Mat_ele_iter<T, RefL, PtrL> &lhs,
                       const __Mat_ele_iter<T, RefR, PtrR> &rhs) {
  lhs.__check_diff(rhs, "operator>= on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() >= rhs.index();
}

template <typename T, typename Ref, typename Ptr>
inline bool operator>=(const __Mat_ele_iter<T, Ref, Ptr> &lhs,
                       const __Mat_ele_iter<T, Ref, Ptr> &rhs) {
  lhs.__check_diff(rhs, "operator>= on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() >= rhs.index();
}

template <typename T, typename RefL, typename PtrL, typename RefR,
          typename PtrR>
inline auto operator-(const __Mat_ele_iter<T, RefL, PtrL> &lhs,
                      const __Mat_ele_iter<T, RefR, PtrR> &rhs) {
  lhs.__check_diff(rhs, "operator- on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() - rhs.index();
}

template <typename T, typename Ref, typename Ptr>
inline auto operator-(const __Mat_ele_iter<T, Ref, Ptr> &lhs,
                      const __Mat_ele_iter<T, Ref, Ptr> &rhs) {
  lhs.__check_diff(rhs, "operator- on Matrix elements' iterators pointing to "
                        "different matrices!");
  return lhs.index() - rhs.index();
}

template <typename T, typename Ref, typename Ptr>
inline auto operator+(typename __Mat_ele_iter<T, Ref, Ptr>::difference_type n,
                      const __Mat_ele_iter<T, Ref, Ptr> &t) {
  return t + n;
}

template <typename T, typename Ref, typename Ptr>
class __Mat_elements {
 public:
  using iterator = __Mat_ele_iter<T, Ref, Ptr>;
  using const_iterator = __Mat_ele_iter<T, const T &, const T *>;
  using reference = Ref;
  using pointer = Ptr;
  using const_reference = const T &;
  using const_pointer = const T *;
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 private:
  Matrix<T> *m_pmat;
  using _Self = __Mat_elements<T, Ref, Ptr>;

 public:
  __Mat_elements() = delete;
  __Mat_elements(Matrix<T> *pm) : m_pmat(pm) {}
  // copy operations are constructed by default
  // iterators
  iterator begin() {
    return iterator(m_pmat, 0);
  }
  const_iterator begin() const {
    return const_iterator(m_pmat, 0);
  }
  const_iterator cbegin() const {
    return begin();
  }
  iterator end() {
    return iterator(m_pmat, m_pmat->element_count());
  }
  const_iterator end() const {
    return const_iterator(m_pmat, m_pmat->element_count());
  }
  const_iterator cend() const {
    return end();
  }
  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crbegin() const {
    return rbegin();
  }
  reverse_iterator rend() {
    return reverse_iterator(begin());
  }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator crend() const {
    return rend();
  }
  Dynarray<T> to_dynarray() const {
    return Dynarray<T>(begin(), end());
  }
  const_reference operator[](size_type n) const {
    return at(n);
  }
  reference operator[](size_type n) {
    return at(n);
  }
  const_reference at(size_type n) const {
    return *(cbegin() + n);
  }
  reference at(size_type n) {
    return *(begin() + n);
  }
  size_type size() const {
    return m_pmat->element_count();
  }
  bool empty() const {
    return m_pmat->empty();
  }
  const_reference front() const {
    return m_pmat->front().front();
  }
  reference front() {
    return m_pmat->front().front();
  }
  const_reference back() const {
    return m_pmat->back().back();
  }
  reference back() {
    return m_pmat->back().back();
  }
};

template <typename T>
class Matrix {
 private:
  using _Self = Matrix<T>;
  using _Storage_t = Dynarray<Dynarray<T>>;

 public:
  using value_type = Dynarray<T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using pointer = value_type *;
  using const_reference = const value_type &;
  using const_pointer = const value_type *;
  using iterator = typename _Storage_t::iterator;
  using const_iterator = typename _Storage_t::const_iterator;
  using reverse_iterator = typename _Storage_t::reverse_iterator;
  using const_reverse_iterator = typename _Storage_t::const_reverse_iterator;

 private:
  using _Ele_t = __Mat_elements<T, T &, T *>;
  using _Cele_t = __Mat_elements<T, const T &, const T *>;

 public:
  // elements-based
  using elements_type = T;
  using elements_iterator = typename _Ele_t::iterator;
  using const_elements_iterator = typename _Cele_t::iterator;
  using reverse_elements_iterator = typename _Ele_t::reverse_iterator;
  using const_reverse_elements_iterator = typename _Cele_t::reverse_iterator;

 private:
  size_type row, col;
  _Storage_t m_data;

 public:
  Matrix() : row(0), col(0), m_data() {}
  explicit Matrix(size_type r, size_type c)
      : row(c > 0 ? r : 0), col(r > 0 ? c : 0), m_data(row) {
    for (auto &v : m_data)
      v.resize(col);
  }
  explicit Matrix(size_type r, size_type c, const T &x) : Matrix(r, c) {
    for (auto &v : m_data)
      v.resize(col, x);
  }
  Matrix(std::initializer_list<std::initializer_list<T>> il)
      : Matrix(il.size(), std::max_element(
                              il.begin(), il.end(),
                              [](const std::initializer_list<T> &lhs,
                                 const std::initializer_list<T> &rhs) -> bool {
                                return lhs.size() < rhs.size();
                              })
                              ->size()) {
    if (row) {
      auto ilit = il.begin();
      for (auto &line : m_data) {
        std::copy(ilit->begin(), ilit->end(), line.begin());
        ++ilit;
      }
    }
  }

  // Iterators
  iterator begin() {
    return m_data.begin();
  }
  iterator end() {
    return m_data.end();
  }
  const_iterator begin() const {
    return m_data.cbegin();
  }
  const_iterator end() const {
    return m_data.cend();
  }
  const_iterator cbegin() const {
    return m_data.cbegin();
  }
  const_iterator cend() const {
    return m_data.cend();
  }
  reverse_iterator rbegin() {
    return m_data.rbegin();
  }
  reverse_iterator rend() {
    return m_data.rend();
  }
  const_reverse_iterator rbegin() const {
    return m_data.crbegin();
  }
  const_reverse_iterator rend() const {
    return m_data.crend();
  }
  const_reverse_iterator crbegin() const {
    return m_data.crbegin();
  }
  const_reverse_iterator crend() const {
    return m_data.crend();
  }

  size_type size() const {
    return row;
  }
  size_type row_count() const {
    return row;
  }
  size_type column_count() const {
    return col;
  }
  size_type element_count() const {
    return row * col;
  }
  bool empty() const {
    return !row;
  }

  // data access
  reference operator[](size_type n) {
    return const_cast<reference>(
        static_cast<const _Self *>(this)->operator[](n));
  }
  const_reference operator[](size_type n) const {
    return m_data[n];
  }
  reference at(size_type n) {
    return this->operator[](n);
  }
  const_reference at(size_type n) const {
    return this->operator[](n);
  }
  reference front() {
    return m_data.front();
  }
  const_reference front() const {
    return m_data.front();
  }
  reference back() {
    return m_data.back();
  }
  const_reference back() const {
    return m_data.back();
  }

  _Ele_t elements() {
    return _Ele_t(this);
  }
  _Cele_t elements() const {
    return _Cele_t(this);
  }

  std::string to_latex(const std::string & = "matrix") const;
};

template <typename T>
inline void print(const Matrix<T> &mt, std::ostream &os = std::cout,
                  const std::string &sep = " ",
                  const std::string &rowDelim = "\n",
                  const std::string &end = "\n") {
  for (auto it = mt.begin(); it != mt.end(); ++it)
    print(*it, os, sep, it + 1 == mt.end() ? end : rowDelim);
}

template <typename T>
std::string Matrix<T>::to_latex(const std::string &tp) const {
  if (tp != "matrix" && tp != "vmatrix" && tp != "Vmatrix" && tp != "bmatrix" &&
      tp != "Bmatrix" && tp != "pmatrix")
    throw std::invalid_argument(
        "Invalid LaTeX matrix type!\n\'" + tp +
        "\' is not one of \'matrix\', \'vmatrix\', \'Vmatrix\', "
        "\'bmatrix\', \'Bmatrix\' or \'pmatrix\'.");
  std::stringstream ss;
  ss << "\\begin{" << tp << "}\n";
  print(*this, ss, " & ", "\\\\\n", "\n");
  ss << "\\end{" << tp << "}";
  return ss.str();
}

template <typename T>
Matrix<T> transpose(const Matrix<T> &mt) {
  Matrix<T> res(mt.column_count(), mt.row_count());
  using size_type = typename Matrix<T>::size_type;
  for (size_type i = 0; i < res.row_count(); ++i)
    for (size_type j = 0; j < res.column_count(); ++j)
      res[i][j] = mt[j][i];
  return res;
}

template <typename T>
inline bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  if (lhs.size() != rhs.size())
    return false;
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T>
inline bool operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs) {
  return !(lhs == rhs);
}

} // namespace gkxx

#endif // GKXX_WHEEL_MATRIX_HPP