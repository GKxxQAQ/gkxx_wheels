#ifndef GKXX_WHEEL_MALLOCATOR_HPP
#define GKXX_WHEEL_MALLOCATOR_HPP

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>

namespace gkxx {

template <typename Tp>
class Mallocator {
 public:
  using value_type = Tp;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = Tp *;
  using reference = Tp &;
  using const_pointer = Tp const *;
  using const_reference = Tp const &;

  constexpr Mallocator() noexcept = default;
  constexpr Mallocator(const Mallocator &) noexcept = default;
  template <typename U>
  constexpr Mallocator(const Mallocator<U> &) noexcept {}
  ~Mallocator() = default;
  pointer address(reference x) const noexcept {
    return std::addressof(x);
  }
  const_pointer address(const_reference x) const noexcept {
    return std::addressof(x);
  }
  pointer allocate(size_type n) {
    auto ptr = std::malloc(n * sizeof(Tp));
    if (!ptr)
      throw std::bad_alloc{};
    return static_cast<pointer>(ptr);
  }
  void deallocate(pointer p, size_type) {
    std::free(p);
  }
  template <typename U, typename... Args>
  void construct(U *p, Args &&...args) noexcept(
      std::is_nothrow_constructible<U, Args...>::value) {
    new (static_cast<void *>(p)) U(std::forward<Args>(args)...);
  }
  template <typename U>
  void destroy(U *p) noexcept(std::is_nothrow_destructible<U>::value) {
    p->~U();
  }
  constexpr size_type max_size() const noexcept {
#if PTRDIFF_MAX < SIZE_MAX
    return std::size_t(PTRDIFF_MAX) / sizeof(Tp);
#else
    return std::size_t(-1) / sizeof(Tp);
#endif
  }
};

} // namespace gkxx

#endif // GKXX_WHEEL_MALLOCATOR_HPP