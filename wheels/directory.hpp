#ifndef GKXX_WHEEL_DIRECTORY_HPP
#define GKXX_WHEEL_DIRECTORY_HPP

#include <cstddef>
#include <dirent.h>
#include <iterator>
#include <string>

namespace gkxx {

class Directory {
 public:
  class iterator;
  using const_iterator = iterator;

 private:
  std::string m_path;

 public:
  Directory(const std::string &path) : m_path(path) {}
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;
};

class Directory::iterator {
 public:
  using value_type = std::string;
  using pointer = const value_type *;
  using reference = const value_type &;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

 private:
  DIR *m_dp;
  bool m_isend;
  std::string m_cur;

  void m_read() {
    dirent *d = readdir(m_dp);
    if (d)
      m_cur = d->d_name;
    else
      m_isend = true;
  }

 public:
  iterator() : m_dp(nullptr), m_isend(true) {}
  iterator(const std::string &path)
      : m_dp(opendir(path.c_str())), m_isend(false) {
    m_read();
  }
  ~iterator() {
    closedir(m_dp);
  }

  reference operator*() const {
    return m_cur;
  }

  pointer operator->() const {
    return std::addressof(operator*());
  }

  iterator &operator++() {
    m_read();
    return *this;
  }

  iterator operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  friend bool operator==(const iterator &lhs, const iterator &rhs) {
    return (lhs.m_isend == rhs.m_isend) &&
           (lhs.m_isend || lhs.m_cur == rhs.m_cur);
  }

  friend bool operator!=(const iterator &lhs, const iterator &rhs) {
    return !(lhs == rhs);
  }
};

inline Directory::const_iterator Directory::begin() const {
  return const_iterator{m_path};
}

inline Directory::const_iterator Directory::end() const {
  return const_iterator{};
}

inline Directory::const_iterator Directory::cbegin() const {
  return begin();
}

inline Directory::const_iterator Directory::cend() const {
  return end();
}

} // namespace gkxx

#endif // GKXX_WHEEL_DIRECTORY_HPP