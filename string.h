#include <iostream>

int countChar(std::string toSearch, char toCount) {
  int newLineTotal = 0;

  // just use accumulate
  for (char &character : toSearch) {
    if (character == toCount) {
      newLineTotal += 1;
    }
  }

  return newLineTotal;
}
