#include "Input.h"
#include <termios.h>
#include <unistd.h>

Input::Input() {
  // https://stackoverflow.com/questions/6698161/getting-raw-input-from-console-using-c-or-c

  termios newt;
  tcgetattr( STDIN_FILENO, &oldt ); // get old terminal IO settings

  newt = oldt;                       // copy old settings
  newt.c_lflag &= (tcflag_t)~ICANON; // disable buffered IO
  newt.c_lflag &= (tcflag_t)~ECHO;   // disable echo?

  tcsetattr( STDIN_FILENO, TCSANOW, &newt ); // use new settings
}

Input::~Input() {
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt ); // reset terminal IO settings
}

char Input::get_ch() const {
  char c;
  read(STDIN_FILENO, &c, 1);
  return c;
}
