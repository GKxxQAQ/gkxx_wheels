#include <functional>
#include <iostream>
#include <type_traits>
#include <utility>

template <typename Want>
inline constexpr const Want *get() {
    return nullptr;
}

template <typename Want, typename T1, typename... Others>
inline constexpr const Want *get(T1 &&first, Others &&...others);

template <typename Want, bool /* false */>
struct decide {
    template <typename T1, typename... Others>
    constexpr const Want *operator()(T1 &&first,
                                             Others &&...others) const {
        return get<Want>(std::forward<Others>(others)...);
    }
};

template <typename Want>
struct decide<Want, true> {
    template <typename T1, typename... Others>
    constexpr const Want *operator()(T1 &&first,
                                             Others &&...others) const {
        return &first;
    }
};

template <typename Want, typename T1, typename... Others>
inline constexpr const Want *get(T1 &&first, Others &&...others) {
    return decide<Want, std::is_same_v<Want, std::remove_reference_t<T1>>>()(
        std::forward<T1>(first), std::forward<Others>(others)...);
}

class Foo {
public:
    Foo(const Foo &) {
        std::cout << "copy ctor" << std::endl;
    }
    Foo &operator=(const Foo &) {
        std::cout << "copy assignment" << std::endl;
        return *this;
    }
    Foo(Foo &&) noexcept {
        std::cout << "move ctor" << std::endl;
    }
    Foo &operator=(Foo &&) noexcept {
        std::cout << "move assignment" << std::endl;
        return *this;
    }
    Foo() = default;
    ~Foo() = default;
    int get_value() const {
        return 100;
    }
};

int main() {
    std::cout << std::boolalpha;
    std::cout << *get<int>(3.14, 's', 42, 8.0) << std::endl;
    std::cout << (get<int>(3.14, 8.0) == nullptr) << std::endl;
    const int i = 42;
    std::cout << *get<const int>(i) << std::endl;
    Foo foo;
    [[maybe_unused]] auto x = get<Foo>(42, 's', foo);
    std::cout << x->get_value() << std::endl;
    return 0;
}