#include <climits>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include "io.h"
using namespace std;

unsigned long getPrime(unsigned long min, unsigned long max)
{
   return min + (rand() % (max - min));
}

bool isPerfectSquare(unsigned long long num)
{
   const unsigned long long sq = sqrt(num);
   return sq * sq == num;
}

int main()
{
   srand(time(0));
   const unsigned long p = getPrime(UCHAR_MAX, USHRT_MAX);
   const unsigned long q = getPrime(UCHAR_MAX, USHRT_MAX);
   const unsigned long n = p * q;

   unsigned long a = ceil((unsigned long)sqrt(n));
   unsigned long b;
   size_t iterations = 0;
   while (true)
   {
      iterations++;
      unsigned long long b2 = (unsigned long long)a * a - n;
      if (isPerfectSquare(b2))
      {
         b = sqrt(b2);
         break;
      }
      a += 1;
   }

   const unsigned long long x = a + b;
   const unsigned long long y = a - b;

   print("ITER: " + to_string(iterations));
   print(x * y == n);
   print("DELTA: " + to_string(x - y));
}
