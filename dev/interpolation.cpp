// https://chao-ji.github.io/jekyll/update/2018/07/19/BilinearResize.html

#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

vector<double> linearResize(vector<double> inputArr, size_t outSize)
{

   if (inputArr.empty() || !outSize)
      return {};

   const size_t inSize = inputArr.size();

   if (outSize == inSize)
      return inputArr;

   const double ratio = (inSize - 1) / double(outSize - 1);
   vector<double> outArr(outSize, 0);

   for (size_t i = 0; i < outSize; i++)
   {
      const int low = floor(ratio * i);
      const int high = min(int(inSize - 1), low + 1);
      const double weight = ratio * i - low;

      const double a = inputArr[low];
      const double b = inputArr[high];

      outArr[i] = (a * (1 - weight) + b * weight);
   }

   return outArr;
};

vector<vector<double>> bilinearResize(vector<vector<double>> inputArr,
                                      size_t outHeight, size_t outWidth)
{

   if (inputArr.empty() || !outHeight || !outWidth)
      return {};

   const size_t inHeight = inputArr.size();
   const size_t inWidth = inputArr[0].size();

   if (outHeight == inHeight && outWidth == inWidth)
      return inputArr;

   const double x_ratio =
       outWidth > 1 ? double(inWidth - 1) / double(outWidth - 1) : 0;
   const double y_ratio =
       outHeight > 1 ? double(inHeight - 1) / double(outHeight - 1) : 0;

   vector<vector<double>> outArr(outHeight, vector<double>(outWidth, 0));

   for (size_t i = 0; i < outHeight; i++)
   {
      for (size_t j = 0; j < outWidth; j++)
      {
         const int xLow = floor(x_ratio * j);
         const int yLow = floor(y_ratio * i);
         const int xHigh = min(xLow + 1, int(inWidth - 1));
         const int yHigh = min(yLow + 1, int(inHeight - 1));

         const double xWeight = (x_ratio * j) - xLow;
         const double yWeight = (y_ratio * i) - yLow;

         const double a = inputArr[yLow][xLow];
         const double b = inputArr[yLow][xHigh];
         const double c = inputArr[yHigh][xLow];
         const double d = inputArr[yHigh][xHigh];

         const double pixel =
             a * (1 - xWeight) * (1 - yWeight) + b * xWeight * (1 - yWeight) +
             c * yWeight * (1 - xWeight) + d * xWeight * yWeight;

         outArr[i][j] = pixel;
      }
   }

   return outArr;
};

int main()
{
   const vector<vector<double>> inputArr{{114., 195., 254., 217., 33., 160.},
                                         {110., 91., 184., 143., 190., 124.},
                                         {212., 163., 245., 39., 83., 188.},
                                         {23., 206., 62., 7., 5., 206.},
                                         {152., 177., 118., 155., 245., 41.}};
   const vector<vector<double>> outArr = bilinearResize(inputArr, 2, 10);
   for (size_t i = 0; i < outArr.size(); i++)
   {
      for (size_t j = 0; j < outArr[0].size(); j++)
      {
         cout << outArr[i][j] << ' ';
      }
      cout << '\n';
   }
   cout << flush;
}
