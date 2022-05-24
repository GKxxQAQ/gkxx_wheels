#include <wheels/directory.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
  for (auto folder : gkxx::Directory("..")) {
    std::cout << folder << std::endl;
    if (folder == "." || folder == "..")
      continue;
    for (auto name : gkxx::Directory("../" + folder)) {
      if (name.find('.') >= name.size()) {
        std::cout << "- " + name << std::endl;
        std::system(("rm ../" + folder + "/" + name).c_str());
      }
    }
  }
  return 0;
}