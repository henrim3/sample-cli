#include "IO.h"
#include <termios.h>
#include <unistd.h>

termios IO::oldt{};

void IO::init() {
  // https://stackoverflow.com/questions/6698161/getting-raw-input-from-console-using-c-or-c

  termios newt;
  tcgetattr( STDIN_FILENO, &oldt ); // get old terminal IO settings

  newt = oldt;                       // copy old settings
  newt.c_lflag &= (tcflag_t)~ICANON; // disable buffered IO
  newt.c_lflag &= (tcflag_t)~ECHO;   // disable echo?

  tcsetattr( STDIN_FILENO, TCSANOW, &newt ); // use new settings
}

void IO::destroy() {
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt ); // reset terminal IO settings
}

char IO::get_ch() {
  char c;
  read( STDIN_FILENO, &c, 1 );
  return c;
}

std::string IO::get_line() {
  std::string s = "";
  while ( true ) {
    char c = get_ch();
    if ( c == '\n' || c == '\r' ) {
      print_newline();
      return s;
    }

    // handle backspace
    if ( c == 127 || c == '\b' ) {
      if ( !s.empty() ) {
        s.pop_back();

        // erase character from the screen
        print( '\b' );
        print( ' ' );
        print( '\b' );
      }
      continue;
    }

    print( c );

    s += c;
  }
}

void IO::print_prompt( const ApplicationState & state ) {
  std::cout << "sample-cli ";

  // output selected sample id
  if ( state.get_mode() == ApplicationMode::Sample ) {
    std::cout << "[sample " << state.get_selected_sample_id() << "] ";
  }

  std::cout << "> " << std::flush;
}

void IO::print_newline() {
  std::cout << std::endl;
}


