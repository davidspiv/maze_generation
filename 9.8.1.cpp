#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main()
{
   cout << "Input file: " << endl;
   string input_file_name;
   cin >> input_file_name;

   ifstream in;
   in.open(input_file_name);

   size_t uppercase = 0;
   size_t lowercase = 0;
   size_t digits = 0;
   size_t whitespace = 0;
   size_t other = 0;
   char ch;
   while (in.get(ch))
   {
      if (isupper(ch))
      {
         uppercase++;
      }
      else if (islower(ch))
      {
         lowercase++;
      }
      else if (isdigit(ch))
      {
         digits++;
      }
      else if (isspace(ch))
      {
         whitespace++;
      }
      else
      {
         other++;
      }
   }

   cout << "Uppercase: " << uppercase << endl;
   cout << "Lowercase: " << lowercase << endl;
   cout << "Digits: " << digits << endl;
   cout << "Whitespace: " << whitespace << endl;
   cout << "Other: " << other << endl;

   return 0;
}
