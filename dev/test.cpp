#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

bool contains(std::vector<int> arr, int dir) {
  for (auto el : arr) {
    if (dir == el) return false;
  }
  return true;
};

int main() {
  std::srand(std::time(0));

  std::vector<int> li(0);
  while (li.size() < 4) {
    int dir = (std::rand() % 4) + 1;
    while (!contains(li, dir)) {
      dir = (std::rand() % 4) + 1;
    }
    li.push_back(dir);
  }

  for (auto el : li) {
    std::cout << el << std::endl;
  }
}
