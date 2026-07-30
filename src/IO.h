#pragma once

#include "ApplicationState.h"
#include <iostream>
#include <array>
#include <string>
#include <termios.h>

enum class SpecialKey {
  ENTER,
  BACKSPACE,
  COUNT,
};

constexpr std::array<std::string, static_cast<size_t>( SpecialKey::COUNT )>
  SpecialKeyNames{ "ENTER", "BACKSPACE" };

class IO {
public:
  static void init();
  static void destroy();

  // Input
  static char get_ch();
  static std::string get_line();
  static SpecialKey get_special_key();

  // Output
  static void print_prompt( const ApplicationState & state );

  template <typename... Ts>
  static void print( Ts &&... values ) {
    ( std::cout << ... << values );
    std::cout << std::flush;
  }

  template <typename... Ts>
  static void println( Ts &&... values ) {
    ( std::cout << ... << values );
    std::cout << std::endl;
  }

  static void print_newline();

  template <typename... Ts>
  static void print_debug( Ts &&... values ) {
    std::cout << "DEBUG: ";
    ( std::cout << ... << values );
    std::cout << std::endl;
  }

  template <typename... Ts>
  static void print_error( Ts &&... values ) {
    std::cout << "ERROR: ";
    ( std::cout << ... << values );
    std::cout << std::endl;
  }

  // State
  static std::string_view get_input_buffer();
  static bool increment_cursor_pos( int delta );
  static void clear_input_buffer();
  static bool backspace();

private:
  static termios _oldt;
  static std::string _input_buffer;
  static std::size_t _cursor_pos;
};
