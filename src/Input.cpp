#include "Input.h"
#include "Output.h"
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
  read( STDIN_FILENO, &c, 1 );
  return c;
}

std::string Input::get_line() const {
  std::string s = "";
  while (true) {
    char c = get_ch();
    if (c == '\n' || c == '\r') {
      Output::newline();
      return s;
    }

    // handle backspace
    if (c == 127 || c == '\b') {
      if (!s.empty()) {
        s.pop_back();

        // Erase character from the screen
        Output::print( '\b' );
        Output::print( ' ' );
        Output::print( '\b' );
      }
      continue;
    }

    Output::print( c );

    s += c;
  }
}
