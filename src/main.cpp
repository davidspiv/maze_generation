#include <algorithm>
#include <array>
#include <ctime>
#include <exception>
#include <iostream>
#include <random>
#include <stack>
#include <vector>

#include "../include/picture.h"

#define CELL_SIZE 3;

enum cellState { WALL, UNVISITED, VISITED };


bool contains(std::array<int, 4> &arr, int dir) {
  return std::find(arr.begin(), arr.end(), dir) != arr.end();
}


// returns random even number between 2 and maxWidth - 1;
int getStart(int max) { return 2 + (std::rand() % ((max - 2) / 2)) * 2; }


void generateNewMazeCell(int startX, int startY,
                         std::vector<std::vector<cellState>> &grid) {

  const static std::array<std::pair<int, int>, 4> directions = {
      {{0, -2}, {0, 2}, {-2, 0}, {2, 0}}};

  std::stack<std::pair<int, int>> cellStack;
  grid.at(startY).at(startX) = VISITED;
  cellStack.push({startX, startY});
  std::array<int, 4> moveOptions = {0, 1, 2, 3}; // move options are reshuffled

  while (!cellStack.empty()) {
    std::random_shuffle(moveOptions.begin(), moveOptions.end());
    bool moved = false;
    auto [currX, currY] = cellStack.top();

    for (size_t i = 0; i < directions.size(); i++) {
      const int dir = moveOptions[i];

      const int nextX = currX + directions[dir].first;
      const int nextY = currY + directions[dir].second;
      const int midX = currX + directions[dir].first / 2;
      const int midY = currY + directions[dir].second / 2;

      if (grid.at(nextY).at(nextX) != VISITED) {
        grid.at(midY).at(midX) = VISITED; // create a break in a wall
        grid.at(nextY).at(nextX) = VISITED;
        cellStack.push({nextX, nextY});
        moved = true;
        break;
      }
    }

    if (!moved) {
      cellStack.pop(); // backtrack if no moves are possible
    }
  }
}


void initializeMaze(std::vector<std::vector<cellState>> &grid) {
  const size_t width = grid.at(0).size();
  const size_t height = grid.size();

  if (height < 5 || width < 5)
    throw std::runtime_error("Both width and height must be greater than 4.");

  // Initialize grid of walls
  for (size_t i = 2; i < height - 2; i += 2) {
    for (size_t j = 2; j < width - 2; j += 2) {
      grid.at(i).at(j) = WALL;
    }
  }

  // Create a border of "visited" cells; simplifies generation algorithm
  for (size_t i = 0; i < width; ++i) {
    grid.at(0).at(i) = VISITED;
    grid.at(height - 1).at(i) = VISITED;
  }
  for (size_t j = 0; j < height; ++j) {
    grid.at(j).at(0) = VISITED;
    grid.at(j).at(width - 1) = VISITED;
  }

  // Create openings in maze perimeter for start and end
  grid.at(2).at(1) = VISITED;
  grid.at(height - 3).at(width - 2) = VISITED;
}

std::pair<int, int> getDimensions() {
  int width, height;

  std::cout << "Enter an odd number, 5 or greater." << std::endl;
  std::cout << "Width [px]: ";
  std::cin >> width;
  std::cout << "Height [px]: ";
  std::cin >> height;

  return {width, height};
};


void createPicture(std::vector<std::vector<cellState>> &grid) {

  const int n = 4; // scale
  const size_t height = grid.size();
  const size_t width = grid.at(0).size();
  Picture pic(width * n, height * n, 0, 0, 0);

  for (size_t i = 0; i < width; i++) {
    for (size_t j = 0; j < height; j++) {
      int color;
      switch (grid.at(j).at(i)) {
      case WALL:
        color = 255;
        break;
      case UNVISITED:
        color = 0;
        break;
      case VISITED:
        color = 127;
        break;
      default:
        throw std::runtime_error("Grid populated with unknown option.");
        break;
      }


      for (size_t k = 0; k < n; k++) {
        for (size_t l = 0; l < n; l++) {
          pic.set(i * n + k, j * n + l, color, color, color);
        }
      }
    }
  }

  pic.save("maze.png");
}


void removeBorder(std::vector<std::vector<cellState>> &grid) {
  const size_t width = grid.at(0).size() - 2;
  const size_t height = grid.size() - 2;

  std::vector<std::vector<cellState>> newGrid(
      height, std::vector<cellState>(width, UNVISITED));

  for (size_t i = 1; i < grid.size() - 1; ++i) {
    newGrid.at(i - 1) =
        std::vector<cellState>(grid[i].begin() + 1, grid[i].end() - 1);
  }

  grid = newGrid;
}

int main() {
  std::srand(std::time(0));

  std::pair<int, int> dimensions = getDimensions();
  auto [width, height] = dimensions;
  // Ensures odd value by rounding up
  width |= 1;
  height |= 1;

  const int startX = getStart(width);
  const int startY = getStart(height);

  std::vector<std::vector<cellState>> grid(
      height, std::vector<cellState>(width, UNVISITED));

  initializeMaze(grid);
  generateNewMazeCell(startX, startY, grid);
  //   removeBorder(grid);
  createPicture(grid);
}
