#ifndef GKXX_WHEEL_STRING_CTYPE_HPP
#define GKXX_WHEEL_STRING_CTYPE_HPP

#include <cctype>
#include <string>

namespace gkxx {

inline bool isalnum(const std::string &s) {
  for (auto c : s)
    if (!std::isalnum(c))
      return false;
  return true;
}

inline bool isalpha(const std::string &s) {
  for (auto c : s)
    if (!std::isalpha(c))
      return false;
  return true;
}

inline bool islower(const std::string &s) {
  for (auto c : s)
    if (!std::islower(c))
      return false;
  return true;
}

inline bool isupper(const std::string &s) {
  for (auto c : s)
    if (!std::isupper(c))
      return false;
  return true;
}

inline bool isdigit(const std::string &s) {
  for (auto c : s)
    if (!std::isdigit(c))
      return false;
  return true;
}

inline bool isxdigit(const std::string &s) {
  for (auto c : s)
    if (!std::isxdigit(c))
      return false;
  return true;
}

inline bool iscntrl(const std::string &s) {
  for (auto c : s)
    if (!std::iscntrl(c))
      return false;
  return true;
}

inline bool isgraph(const std::string &s) {
  for (auto c : s)
    if (!std::isgraph(c))
      return false;
  return true;
}

inline bool isspace(const std::string &s) {
  for (auto c : s)
    if (!std::isspace(c))
      return false;
  return true;
}

inline bool isblank(const std::string &s) {
  for (auto c : s)
    if (!std::isblank(c))
      return false;
  return true;
}

inline bool isprint(const std::string &s) {
  for (auto c : s)
    if (!std::isprint(c))
      return false;
  return true;
}

inline std::string tolower(const std::string &s) {
  std::string result(s);
  for (auto &c : result)
    c = std::tolower(c);
  return result;
}

inline std::string toupper(const std::string &s) {
  std::string result(s);
  for (auto &c : result)
    c = std::toupper(c);
  return result;
}

} // namespace gkxx

#endif // GKXX_WHEEL_STRING_CTYPE_HPP