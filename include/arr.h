#include <iostream>

template <typename T>
void expandArray(T *&arr, size_t &size) {
  const size_t STEP = 5;
  T *newArr = new T[size + STEP];

  for (size_t i = 0; i < size; i++) {
    newArr[i] = arr[i];
  }

  delete[] arr;
  arr = newArr;
  newArr = nullptr;
  size += STEP;
}
