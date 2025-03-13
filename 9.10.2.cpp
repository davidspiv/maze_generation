#include <iostream>
#include <string>
using namespace std;

/**
   A simulated lock with digit buttons.
*/
class Lock
{
 public:
   void push(int button);
   bool open();

 private:
   string combination = "0042";
   string input;
};

void Lock::push(int button)
{
   string button_as_string = to_string(button);
   input += button_as_string;
}

bool Lock::open()
{
   bool isOpened = input == combination;
   input = "";
   return isOpened;
}

int main()
{
   Lock my_lock;
   my_lock.push(4);
   my_lock.push(2);
   cout << boolalpha;
   cout << my_lock.open() << endl;
   cout << "Expected: false\n" << endl;
   my_lock.push(0);
   my_lock.push(0);
   my_lock.push(4);
   my_lock.push(2);
   cout << my_lock.open() << endl;
   cout << "Expected: true\n" << endl;
   my_lock.push(0);
   my_lock.push(4);
   my_lock.push(2);
   cout << my_lock.open() << endl;
   cout << "Expected: false\n" << endl;
   my_lock.push(0);
   my_lock.push(0);
   my_lock.push(0);
   my_lock.push(4);
   my_lock.push(2);
   cout << my_lock.open() << endl;
   cout << "Expected: false\n" << endl;
   my_lock.push(0);
   my_lock.push(0);
   my_lock.push(4);
   my_lock.push(2);
   cout << my_lock.open() << endl;
   cout << "Expected: true" << endl;
   return 0;
}
