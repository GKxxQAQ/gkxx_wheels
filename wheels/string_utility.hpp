#ifndef GKXX_WHEEL_STRING_UTILITY_HPP
#define GKXX_WHEEL_STRING_UTILITY_HPP

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <string>
#include <vector>

namespace gkxx {

template <typename Iterable>
std::string strip(const std::string &str, const Iterable &chars) {
  auto not_in_chars = [&chars](char x) -> bool {
    return std::find(std::begin(chars), std::end(chars), x) == std::end(chars);
  };
  auto last = std::find_if(str.rbegin(), str.rend(), not_in_chars).base();
  auto first = std::find_if(str.begin(), last, not_in_chars);
  return std::string(first, last);
}

inline std::string strip(const std::string &str,
                         std::initializer_list<char> chars) {
  return strip<std::initializer_list<char>>(str, chars);
}

inline std::string strip(const std::string &str, char the_char = ' ') {
  return strip(str, std::string(1, the_char));
}

template <typename Iterable>
std::string lstrip(const std::string &str, const Iterable &chars) {
  auto not_in_chars = [&chars](char x) -> bool {
    return std::find(std::begin(chars), std::end(chars), x) == std::end(chars);
  };
  auto pos = std::find_if(str.begin(), str.end(), not_in_chars);
  return std::string(pos, str.end());
}

inline std::string lstrip(const std::string &str,
                          std::initializer_list<char> chars) {
  return lstrip<std::initializer_list<char>>(str, chars);
}

inline std::string lstrip(const std::string &str, char the_char = ' ') {
  return lstrip(str, std::string(1, the_char));
}

template <typename Iterable>
std::string rstrip(const std::string &str, const Iterable &chars) {
  auto not_in_chars = [&chars](char x) -> bool {
    return std::find(std::begin(chars), std::end(chars), x) == std::end(chars);
  };
  auto last = std::find_if(str.rbegin(), str.rend(), not_in_chars).base();
  return std::string(str.begin(), last);
}

inline std::string rstrip(const std::string &str,
                          std::initializer_list<char> chars) {
  return rstrip<std::initializer_list<char>>(str, chars);
}

inline std::string rstrip(const std::string &str, char the_char = ' ') {
  return rstrip(str, std::string(1, the_char));
}

template <typename Iterable>
std::string join(const std::string &sep, const Iterable &ele) {
  if (std::empty(ele))
    return "";
  auto it = std::begin(ele);
  std::string result = *it++;
  while (it != std::end(ele))
    result += sep + *it++;
  return result;
}

inline std::string join(const std::string &sep,
                        std::initializer_list<std::string> chars) {
  return join<std::initializer_list<std::string>>(sep, chars);
}

template <typename Iterable>
inline std::string join(char sep, const Iterable &ele) {
  return join(std::string(1, sep), ele);
}

inline std::string join(char sep, std::initializer_list<std::string> chars) {
  return join<std::initializer_list<std::string>>(sep, chars);
}

namespace __kmp {

  std::vector<int> get_kmp_next(const std::string &pattern) {
    std::vector<int> next(pattern.size(), 0);
    next[0] = -1;
    for (std::string::size_type i = 1; i < pattern.size(); ++i) {
      next[i] = next[i - 1];
      while (next[i] >= 0 && pattern[next[i] + 1] != pattern[i])
        next[i] = next[next[i]];
      if (pattern[next[i] + 1] == pattern[i])
        ++next[i];
    }
    return next;
  }

  std::vector<std::string::size_type>
  kmp_match_no_overlap(const std::string &str, const std::string &pattern) {
    using size_type = std::string::size_type;
    auto next = get_kmp_next(pattern);
    std::vector<size_type> result;
    size_type last_pos = 0, str_size = str.size(),
              pattern_size = pattern.size();
    int j = -1;
    for (size_type i = 0; i < str_size; ++i) {
      while (str[i] != pattern[j + 1] && j >= 0)
        j = next[j];
      if (str[i] == pattern[j + 1])
        ++j;
      if (j + 1 == (int)pattern_size) {
        size_type start = i - pattern_size + 1;
        if (start >= last_pos) {
          result.emplace_back(start);
          last_pos = i + 1;
        }
        j = next[j];
      }
    }
    return result;
  }

  std::vector<std::string::size_type> kmp_match(const std::string &str,
                                                const std::string &pattern) {
    using size_type = std::string::size_type;
    auto next = get_kmp_next(pattern);
    std::vector<size_type> result;
    size_type str_size = str.size(), pattern_size = pattern.size();
    int j = -1;
    for (size_type i = 0; i < str_size; ++i) {
      while (str[i] != pattern[j + 1] && j >= 0)
        j = next[j];
      if (str[i] == pattern[j + 1])
        ++j;
      if (j + 1 == (int)pattern_size) {
        result.emplace_back(i - pattern_size + 1);
        j = next[j];
      }
    }
    return result;
  }

} // namespace __kmp

std::vector<std::string> split_by_kmp(const std::string &str,
                                      const std::string &delim) {
  using size_type = std::string::size_type;
  auto match = __kmp::kmp_match_no_overlap(str, delim);
  if (match.empty())
    return std::vector<std::string>(1, str);
  std::vector<std::string> result;
  size_type begin_pos = 0;
  for (auto end_pos : match) {
    result.emplace_back(str.substr(begin_pos, end_pos - begin_pos));
    begin_pos = end_pos + delim.size();
  }
  result.emplace_back(str.substr(begin_pos));
  return result;
}

// Should use the normal version of 'split', since the pattern is too short.
std::vector<std::string> split_by_kmp(const std::string &str,
                                      char delim) = delete;

std::vector<std::string> split(const std::string &str,
                               const std::string &delim) {
  std::vector<std::string> result;
  auto curr = str.begin();
  while (true) {
    auto next = std::search(curr, str.end(), delim.begin(), delim.end());
    result.emplace_back(curr, next);
    if (next == str.end())
      break;
    curr = next + delim.size();
  }
  return result;
}

inline auto split(const std::string &str, char delim) {
  return split(str, std::string(1, delim));
}

std::string replace_by_kmp(const std::string &str, const std::string &old,
                           const std::string &new_,
                           std::string::size_type max_replace = -1) {
  using size_type = std::string::size_type;
  auto match = __kmp::kmp_match_no_overlap(str, old);
  std::string result;
  size_type cnt = 0, begin_pos = 0;
  for (auto end_pos : match) {
    result += str.substr(begin_pos, end_pos - begin_pos) + new_;
    begin_pos = end_pos + old.size();
    if (++cnt == max_replace)
      break;
  }
  result += str.substr(begin_pos);
  return result;
}

std::string replace_by_kmp(const std::string &str, const std::string &old,
                           char new_, std::string::size_type max_replace = -1) {
  return replace_by_kmp(str, old, std::string(1, new_), max_replace);
}

// Should use the normal version of 'replace', since the pattern is too short.
std::string replace_by_kmp(const std::string &str, char old,
                           const std::string &new_,
                           std::string::size_type max_replace = -1) = delete;

std::string replace_by_kmp(const std::string &str, char old, char new_,
                           std::string::size_type max_replace = -1) = delete;

std::string replace(const std::string &str, const std::string &old,
                    const std::string &new_,
                    std::string::size_type max_replace = -1) {
  using size_type = std::string::size_type;
  std::string result;
  auto curr = str.begin();
  size_type cnt = 0;
  while (true) {
    auto next = std::search(curr, str.end(), old.begin(), old.end());
    result.append(curr, next);
    if (next == str.end())
      break;
    result += new_;
    ++cnt;
    curr = next + old.size();
    if (cnt == max_replace) {
      result.append(curr, str.end());
      break;
    }
  }
  return result;
}

inline std::string replace(const std::string &str, const std::string &old,
                           char new_, std::string::size_type max_replace = -1) {
  return replace(str, old, std::string(1, new_), max_replace);
}

inline std::string replace(const std::string &str, char old,
                           const std::string &new_,
                           std::string::size_type max_replace = -1) {
  return replace(str, std::string(1, old), new_, max_replace);
}

inline std::string replace(const std::string &str, char old, char new_,
                           std::string::size_type max_replace = -1) {
  return replace(str, std::string(1, old), std::string(1, new_), max_replace);
}

} // namespace gkxx

#endif // GKXX_WHEEL_STRING_UTILITY_HPP