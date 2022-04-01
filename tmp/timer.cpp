#include <chrono>
#include <iostream>
#include <random>

int main() {
  namespace chrono = std::chrono;
  auto start = chrono::high_resolution_clock::now();
  std::random_device rd;
  std::default_random_engine e(rd());
  std::uniform_int_distribution<int> u(1, 10000);
  int x = 0;
  for (int i = 0; i < 100000000; ++i)
    x ^= u(e);
  auto end = chrono::high_resolution_clock::now();
  std::cout
      << chrono::duration_cast<chrono::microseconds>(end - start).count() /
             1000000.0
      << std::endl;
  return 0;
}