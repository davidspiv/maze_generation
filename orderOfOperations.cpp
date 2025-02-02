#include <cmath>
#include <iostream>
#include <string>

#include "io.h"

using namespace std;

bool isValueChar(char ch) { return isdigit(ch) || ch == '.'; }

int main() {
  const double answer = 2.9 / 3.0 * 34 - 6 + 3 / 4.0;
  const string inputString = "2.9 / 3 * 34 - 6 + 3 / 4 ";
  double subResult = 0;
  double addend = 0;
  char operation = '+';
  size_t splitindex = 0;
  bool isOperation = false;

  for (size_t i = 0; i < inputString.length(); i++) {
    if (isOperation) {
      if (isValueChar(inputString[i])) {
        splitindex = i;
        isOperation = false;
      } else if (inputString[i] != ' ') {
        operation = inputString[i];
      }
    } else {
      if (!isValueChar(inputString[i])) {
        const double value =
            stod(inputString.substr(splitindex, i - splitindex));

        switch (operation) {
          case '+':
            if (addend) {
              subResult += addend;
              addend = 0;
            }
            addend = value;
            break;
          case '-':
            if (addend) {
              subResult += addend;
              addend = 0;
            }
            addend = -value;

            break;
          case '*':
            if (addend) {
              subResult += addend * value;
              addend = 0;
            } else {
              subResult *= value;
            }
            break;
          case '/':
            if (addend) {
              subResult += addend / value;
              addend = 0;
            } else {
              subResult /= value;
            }
            break;

          default:
            print("ERROR: switch statement");
            break;
        }
        isOperation = true;
      }
    }
  }

  subResult += addend;

  print("CALC: " + to_string(subResult) + '\n');

  print("ANSWER: " + to_string(answer) + '\n');
}
