#include "IO.h"
#include <termios.h>
#include <unistd.h>

termios IO::_oldt{};
std::string IO::_input_buffer = "";
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

    // TODO: check if readable

    print( c );
    _cursor_pos++;

    s += c;
  }
}

SpecialKey IO::get_special_key() {
  while ( true ) {
    char c = get_ch();
    // newline
    if ( c == '\n' ) {
      return SpecialKey::ENTER;
    }

    // backspace
    if ( c == 127 || c == '\b' ) {
      if ( !_input_buffer.empty() ) {
        _input_buffer.pop_back();

        // erase character from the screen
        print( '\b' );
        print( ' ' );
        print( '\b' );
      }

      return SpecialKey::BACKSPACE;
    }

    print( c );

    _input_buffer += c;
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

std::string_view IO::get_input_buffer() {
  return _input_buffer;
}

bool IO::increment_cursor_pos( int delta ) {
  if ( static_cast<int>( _cursor_pos ) + delta >= 0 ) {
    _cursor_pos =
      static_cast<std::size_t>( static_cast<int>( _cursor_pos ) + delta );
    return true;
  }
  return false;
}

void IO::clear_input_buffer() {
  _input_buffer = "";
}

bool IO::backspace() {
  if ( _input_buffer.empty() || _cursor_pos == 0 ) {
    return false;
  }

  _input_buffer.erase( _cursor_pos - 1, 1 );
  _cursor_pos--;

  return true;
}
