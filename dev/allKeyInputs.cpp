// https://web.archive.org/web/20170407153459/http://www.doctort.org/adam/nerd-notes/reading-single-keystroke-on-linux.html

#include <termios.h>

#include <iostream>

using namespace std;
int main() {
  // prevents Linux from buffering keystrokes.
  struct termios t;
  tcgetattr(0, &t);
  t.c_lflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &t);

  cout << "Enter a character: ";
  char c, d, e;
  cin >> c;
  cin >> d;
  cin >> e;
  cout << "\nYour character was ";
  // Using 3 char type, Cause up down right left consist with 3 character
  if ((c == 27) && (d == 91)) {
    if (e == 65) {
      cout << "UP";
    }
    if (e == 66) {
      cout << "DOWN";
    }
    if (e == 67) {
      cout << "RIGHT";
    }
    if (e == 68) {
      cout << "LEFT";
    }
  }
  return 0;
}
