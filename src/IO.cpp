#include "IO.h"
#include "Key.h"
#include <poll.h>
#include <string>
#include <termios.h>
#include <unistd.h>

termios IO::_oldt{};
std::size_t IO::_cursor_pos = 0;

void IO::init() {
  // https://stackoverflow.com/questions/6698161/getting-raw-input-from-console-using-c-or-c

  termios newt;
  tcgetattr( STDIN_FILENO, &_oldt ); // get old terminal IO settings

  newt = _oldt;                      // copy old settings
  newt.c_lflag &= (tcflag_t)~ICANON; // disable buffered IO
  newt.c_lflag &= (tcflag_t)~ECHO;   // disable echo?

  tcsetattr( STDIN_FILENO, TCSANOW, &newt ); // use new settings
}

void IO::destroy() {
  tcsetattr( STDIN_FILENO, TCSANOW, &_oldt ); // reset terminal IO settings
}

char IO::get_ch() {
  char c;
  read( STDIN_FILENO, &c, 1 );
  return c;
}

Key IO::get_key() {
  while ( true ) {
    char c = get_ch();
    // enter
    if ( c == '\n' ) {
      return Key{ std::string( 1, c ), SpecialKeyType::Enter };
    }

    // escape key and arrows
    if ( c == '\x1b' ) {
      return get_escape_or_arrow_key();
    }

    // backspace
    if ( c == 127 || c == '\b' ) {
      return Key{ std::string( 1, c ), SpecialKeyType::Backspace };
    }

    return Key{ std::string( 1, c ) }; // return regular key
  }
}

void IO::render( std::string_view prompt, const LineEditor & editor ) {
  print_no_flush( "\r" );     // to beginning
  print_no_flush( prompt );   // print prompt
  print( editor.get_text() ); // print buffer
  print_no_flush( "\x1b[K" ); // clear to end of line

  print_no_flush( "\r" ); // to beginning

  // to cursor
  for ( std::size_t i = 0; i < prompt.size() + editor.get_cursor_pos(); i++ ) {
    print_no_flush( "\x1b[C" );
  }

  flush_output();
}

void IO::print_newline() {
  std::cout << std::endl;
}

void IO::flush_output() {
  std::cout << std::flush;
}

Key IO::get_escape_or_arrow_key() {
  pollfd pfd{ STDIN_FILENO, POLLIN, 0 };

  if ( poll( &pfd, 1, 20 ) == 0 ) {
    // 20 ms elapsed, treat as escape
    return Key{ "\x1b", SpecialKeyType::Escape };
  } else {
    char seq[2];
    read( STDIN_FILENO, &seq[0], 1 );
    read( STDIN_FILENO, &seq[1], 1 );

    if ( seq[0] == '[' ) {
      switch ( seq[1] ) {
        case 'A': // up
          return Key{ "\x1b[A", SpecialKeyType::ArrowUp };
        case 'B': // down
          return Key{ "\x1b[B", SpecialKeyType::ArrowDown };
        case 'C': // right
          return Key{ "\x1b[C", SpecialKeyType::ArrowRight };
        case 'D': // left
          return Key{ "\x1b[D", SpecialKeyType::ArrowLeft };
      }
    }

    return Key{ "\x1b" + std::string( seq ), SpecialKeyType::Unhandled };
  }
}
