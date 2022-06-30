#ifndef GKXX_WHEEL_HEAP_TRACKED_HPP
#define GKXX_WHEEL_HEAP_TRACKED_HPP

#include <cstddef>
#include <stdexcept>
#include <unordered_set>

namespace gkxx {

/**
 * This is the Heap_tracked class that is defined in *More Effective C++, 2/e*,
 * by Scott Meyers. By inheriting it publicly (as mix-in), it can track whether
 * the object is heap-based.
 */

class Heap_tracked {
  using _Raw_addr = const void *;

 public:
  class Missing_address;
  virtual ~Heap_tracked() = 0;

  static void *operator new(std::size_t size);
  static void operator delete(void *ptr);
  bool is_on_heap() const;

 private:
  static std::unordered_set<_Raw_addr> addresses;
};

class Heap_tracked::Missing_address : public std::invalid_argument {
 public:
  Missing_address()
      : std::invalid_argument("deleting an object that is not heap-based.") {}
};

std::unordered_set<Heap_tracked::_Raw_addr> Heap_tracked::addresses;

Heap_tracked::~Heap_tracked() {}

void *Heap_tracked::operator new(std::size_t size) {
  void *memPtr = ::operator new(size);
  addresses.insert(memPtr);
  return memPtr;
}

void Heap_tracked::operator delete(void *ptr) {
  auto found = addresses.find(ptr);
  if (found != addresses.end()) {
    addresses.erase(found);
    ::operator delete(ptr);
  } else
    throw Missing_address();
}

bool Heap_tracked::is_on_heap() const {
  const void *ptr = dynamic_cast<const void *>(this);
  return addresses.find(ptr) != addresses.end();
}

} // namespace gkxx

#endif // GKXX_WHEEL_HEAP_TRACKED_HPP