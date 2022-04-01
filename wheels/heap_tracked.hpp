#ifndef GKXX_WHEEL_HEAP_TRACKED_HPP
#define GKXX_WHEEL_HEAP_TRACKED_HPP

#include <cstddef>
#include <stdexcept>
#include <unordered_set>

namespace gkxx {

/**
 * This is the HeapTracked class that is defined in *More Effective C++, 2/e*,
 * by Scott Meyers. By inheriting it publicly (as mix-in), it can track whether
 * the object is heap-based.
 */

class HeapTracked {
  using _Raw_addr = const void *;

 public:
  class MissingAddress;
  virtual ~HeapTracked() = 0;

  static void *operator new(std::size_t size);
  static void operator delete(void *ptr);
  bool isOnHeap() const;

 private:
  static std::unordered_set<_Raw_addr> addresses;
};

class HeapTracked::MissingAddress : public std::invalid_argument {
 public:
  MissingAddress()
      : std::invalid_argument("deleting an object that is not heap-based.") {}
};

std::unordered_set<HeapTracked::_Raw_addr> HeapTracked::addresses;

HeapTracked::~HeapTracked() {}

void *HeapTracked::operator new(std::size_t size) {
  void *memPtr = ::operator new(size);
  addresses.insert(memPtr);
  return memPtr;
}

void HeapTracked::operator delete(void *ptr) {
  auto found = addresses.find(ptr);
  if (found != addresses.end()) {
    addresses.erase(found);
    ::operator delete(ptr);
  } else
    throw MissingAddress();
}

bool HeapTracked::isOnHeap() const {
  const void *ptr = dynamic_cast<const void *>(this);
  return addresses.find(ptr) != addresses.end();
}

} // namespace gkxx

#endif // GKXX_WHEEL_HEAP_TRACKED_HPP