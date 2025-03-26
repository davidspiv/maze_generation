#include <iostream>

std::string formatDouble(double input, int decimalPlaces = 2) {
  std::string inputAsString = std::to_string(input);

  if (decimalPlaces < 0 || decimalPlaces > 6) {
    std::cout << "formatDouble() failed: decimal places argument must be 0 - 6 "
                 "inclusive"
              << std::endl;
    return inputAsString;
  }

  // remove decimal character when no decimal places
  if (!decimalPlaces) {
    decimalPlaces--;
  }

  inputAsString =
      inputAsString.substr(0, inputAsString.length() + decimalPlaces - 6);

  for (int i = inputAsString.length() - decimalPlaces - 4; i > 0; i = i - 3) {
    inputAsString.insert(i, ",");
  }

  return inputAsString;
}
