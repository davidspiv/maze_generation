#ifndef IO_H
#define IO_H

#include <fstream>
#include <iostream>

template <typename T>
void print(T output, bool carriageReturn = 1) {
  if (carriageReturn) {
    std::cout << output << std::endl;
    return;
  }

  std::cout << output << std::flush;
}

// no strings
template <typename T>
T getInput(const std::string& prompt) {
  T input;
  std::cout << prompt;
  std::cin >> input;

  while (!std::cin || std::cin.peek() != '\n') {
    std::cin.clear();
    std::cin.ignore(100, '\n');
    std::cout << "Invalid" << std::endl << prompt;
    std::cin >> input;
  }

  return input;
}

std::string getString(const std::string& prompt) {
  std::string input = "";
  std::cout << prompt;
  getline(std::cin, input);

  while (input == "") {
    std::cout << "No characters entered." << std::endl;
    std::cout << prompt;
    getline(std::cin, input);
  }

  return input;
}

// expose either input or output stream
template <typename T>
bool openFile(T& fileStream, const std::string& fileName) {
  fileStream.open(fileName);
  if (!fileStream) {
    return false;
  }

  return true;
}

// TODO: use SFINAE to restrict types to those writable to output stream
template <typename T>
bool writeToFile(const T& output, const std::string& fileName) {
  std::ofstream outputStream(fileName);

  if (!outputStream) {
    return false;
  }

  outputStream << output;
  outputStream.close();

  return true;
}

#endif
