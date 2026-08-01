#include "IO.h"
#include "AppMode.h"
#include <poll.h>
#include <termios.h>
#include <unistd.h>

termios IO::_oldt{};
std::string IO::_input_buffer = "";
std::size_t IO::_cursor_pos = 0;
std::string IO::_last_prompt = "";

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

SpecialKey IO::get_special_key() {
  while ( true ) {
    char c = get_ch();
    // enter
    if ( c == '\n' ) {
      return SpecialKey::Enter;
    }

    // escape key and arrows
    if ( c == '\x1b' ) {
      return get_escape_or_arrow_key();
    }

    // backspace
    if ( c == 127 || c == '\b' ) {
      return SpecialKey::Backspace;
    }

    handle_input( c );
  }
}

void IO::print_prompt( const AppContext & context ) {
  _last_prompt = "sample-cli ";

  // output selected sample id
  switch ( context.state.get_mode_type() ) {
    case AppModeType::Project:
      break;
    case AppModeType::Sample:
      _last_prompt += "[sample " +
                      std::to_string( context.state.get_selected_sample_id() ) +
                      "] ";
      break;
  }

  _last_prompt += "> ";

  print( _last_prompt );
}

void IO::print_newline() {
  std::cout << std::endl;
}

void IO::flush_output() {
  std::cout << std::flush;
}

void IO::handle_input( char c ) {
  // handle end-of-line input
  if ( _cursor_pos == _input_buffer.size() ) {
    _input_buffer.push_back( c );
    _cursor_pos++;
    print( c );
    return;
  }

  // mid-line input
  _input_buffer.insert( _cursor_pos, std::string( 1, c ) );
  _cursor_pos++;

  print_no_flush( c );
  print_no_flush( _input_buffer.substr( _cursor_pos ) );

  for ( std::size_t i = 0; i < _input_buffer.size() - _cursor_pos; i++ ) {
    print_no_flush( "\x1b[D" );
  }

  flush_output();
}

std::string_view IO::get_input_buffer() {
  return _input_buffer;
}

void IO::handle_enter() {
  _input_buffer = "";
  _cursor_pos = 0;
  print_newline();
}

bool IO::handle_backspace() {
  if ( _input_buffer.empty() || _cursor_pos == 0 ) {
    return false;
  }

  _input_buffer.erase( _cursor_pos - 1, 1 );
  _cursor_pos--;

  print_no_flush( "\r" );          // to beginning
  print_no_flush( _last_prompt );  // prompt again
  print_no_flush( "\x1b[K" );      // clear to end of line
  print_no_flush( _input_buffer ); // print buffer

  // move left back to cursor
  for ( size_t i = 0; i < _input_buffer.size() - _cursor_pos; i++ ) {
    print_no_flush( "\x1b[D" );
  }

  flush_output();

  return true;
}

bool IO::handle_left_arrow() {
  if ( _cursor_pos == 0 ) {
    return false;
  }
  _cursor_pos--;
  print( "\x1b[D" );
  return true;
}
bool IO::handle_right_arrow() {
  if ( _cursor_pos == _input_buffer.size() ) {
    return false;
  }
  _cursor_pos++;
  print( "\x1b[C" );
  return true;
}

SpecialKey IO::get_escape_or_arrow_key() {
  pollfd pfd{ STDIN_FILENO, POLLIN, 0 };

  if ( poll( &pfd, 1, 20 ) == 0 ) {
    // 20 ms elapsed, treat as escape
    return SpecialKey::Escape;
  } else {
    char seq[2];
    read( STDIN_FILENO, &seq[0], 1 );
    read( STDIN_FILENO, &seq[1], 1 );

    if ( seq[0] == '[' ) {
      switch ( seq[1] ) {
        case 'A': // up
          return SpecialKey::ArrowUp;
        case 'B': // down
          return SpecialKey::ArrowDown;
        case 'C': // right
          return SpecialKey::ArrowRight;
        case 'D': // left
          return SpecialKey::ArrowLeft;
      }
    }

    return SpecialKey::Unhandled;
  }
}
