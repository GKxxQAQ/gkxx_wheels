#include <wheels/directory.hpp>
#include <iostream>

int main() {
  auto dir = gkxx::Directory("..");
  for (auto name : dir)
    std::cout << name << " ";
  std::cout << std::endl;
  for (auto name : dir)
    std::cout << name << " ";
  std::cout << std::endl;
  for (auto it = dir.begin(); it != dir.end(); ++it)
    std::cout << it->size() << " ";
  std::cout << std::endl;
  return 0;
}