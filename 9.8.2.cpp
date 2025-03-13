#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main()
{
   //    cout << "Input file: " << endl;
   //    string input_file_name;
   //    cin >> input_file_name;

   ifstream in;
   in.open("scores1.txt");

   char ch;
   while (in.get(ch))
   {
      // Add the characters before the first digit
      // to the student's name.
      string student = "";
      while (in && !isdigit(ch))
      {
         student = student + ch;
         in.get(ch);
      }
      student = student.substr(0, student.length() - 1); // remove space at end

      int total = 0;

      // Add all of the numbers in the rest of the line.
      // Use the one-character look-ahead from the section "Reading Text Input"
      // Stop when the last character read is a '\n'
	  
      in.putback(ch);
      while (in.get(ch))
      {
         if (ch == '\n')
         {
            break;
         }
         else if (ch == ' ')
         {
            continue;
         }
         in.putback(ch);
         int val;
         in >> val;
         total += val;
      }

      cout << student << ": " << total << endl;
   }

   return 0;
}
