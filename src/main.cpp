#include <ctime>
#include <exception>
#include <iostream>
#include <random>
#include <vector>

#include "../include/picture.h"

struct Coord2D {
  int x = 0;
  int y = 0;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

bool contains(const std::vector<int> &arr, int dir) {
  for (auto el : arr) {
    if (dir == el) return false;
  }
  return true;
};

void generate(int x, int y, Picture &pic) {
  //   pic.set(x, y, 127, 127, 127);
  if (pic.blue(y - 2, x) == 127 && pic.blue(y + 2, x) == 127 &&
      pic.blue(y, x - 2) == 127 && pic.blue(y, x + 2) == 127) {
    return;
  } else {
    std::vector<int> li(0);

    while (li.size() < 4) {
      int nx, ny, mx, my;

      int dir = (std::rand() % 4) + 1;
      while (!contains(li, dir)) {
        dir = (std::rand() % 4) + 1;
      }

      li.push_back(dir);

      switch (dir) {
        case Direction::UP:
          nx = x;
          mx = x;
          ny = y - 2;
          my = y - 1;
          break;
        case Direction::DOWN:
          nx = x;
          mx = x;
          ny = y + 2;
          my = y + 1;
          break;
        case Direction::LEFT:
          nx = x - 2;
          mx = x - 1;
          ny = y;
          my = y;
          break;
        case Direction::RIGHT:
          nx = x + 2;
          mx = x + 1;
          ny = y;
          my = y;
          break;

        default:
          nx = x;
          mx = x;
          ny = y;
          my = y;
      }

      if (pic.blue(ny, nx) != 127) {
        pic.set(my, mx, 127, 127, 127);
        generate(nx, ny, pic);
      }
    }
  }
}

void createMaze(Picture &pic) {
  const int height = pic.height();
  const int width = pic.width();
  if (height <= 0 || width <= 0)
    throw std::runtime_error("Both width and height must be greater than 0");

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (!i || !j || (i == height - 1) || (j == width - 1)) {
        pic.set(j, i, 127, 127, 127);
      } else if (!(i % 2) && !(j % 2)) {
        pic.set(j, i, 255, 255, 255);
      }
    }
  }
}

int main() {
  std::srand(std::time(0));

  int height = 11;
  int width = 11;
  Picture pic(width, height, 0, 0, 0);

  createMaze(pic);
  generate(4, 4, pic);

  pic.save("result.png");
}
