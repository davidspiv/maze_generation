#define M_PI 3.14159265358979323846

#include <iomanip>
#include <iostream>

template <typename T>
T getInput(const std::string &prompt) {
  const int expectedMaxInputLength = 100;
  T input;
  std::cout << prompt;
  std::cin >> input;

  while (!std::cin || std::cin.peek() != '\n') {
    std::cin.clear();
    std::cin.ignore(expectedMaxInputLength, '\n');
    std::cout << "Error: invalid input, try again" << std::endl << prompt;
    std::cin >> input;
  }

  return input;
}

int floor(double input) { return static_cast<int>(input); }

double normalizeRadians(double x) {
  const double max = 2 * M_PI;
  return x - floor(x / max) * max;
}

void createFactorialCache(double *factorialCache, size_t iterations) {
  factorialCache[0] = 1;
  for (size_t i = 1; i <= iterations; i++) {
    factorialCache[i] = i * factorialCache[i - 1];
  }
}

double taylorSeriesSine(double radians) {
  const size_t terms = 50;
  double result = 0;
  bool isPositive = terms % 4 && terms % 4 != 3;
  double factorialCache[terms + 1];

  createFactorialCache(&factorialCache[0], terms);

  // Horner's method
  for (int i = terms; i > 0; i -= 1) {
    const double coefficient = (1 / factorialCache[i]);

    if (i % 2) {
      result += isPositive ? coefficient : -coefficient;
      isPositive = !isPositive;
    }

    result *= radians;
  }

  return result;
}

int main() {
  const double radians = normalizeRadians(getInput<double>("Enter Radians: "));
  const double answer = taylorSeriesSine(radians);
  std::cout << std::setw(15) << "Result: " << std::setprecision(15) << answer
            << std::endl;

  return 0;
}
