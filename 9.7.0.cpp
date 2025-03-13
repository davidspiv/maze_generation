#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main()
{
   string input_file_name;
   cin >> input_file_name;

   ifstream in;
   ofstream out;
   out.open("sentences.txt");
   in.open(input_file_name);
   char ch;
   while (in.get(ch))
   {
      switch (ch)
      {
      case '\n':
         continue;
      case '.':
      case '?':
      case '!':
         out << ch << endl;
		 in.get();
         continue;
      }
      out << ch;
   }

   out.close();
   in.close();
   return 0;
}
