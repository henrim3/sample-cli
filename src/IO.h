#pragma once

#include "ApplicationState.h"
#include <array>
#include <iostream>
#include <string>
#include <termios.h>

enum class SpecialKey {
  BACKSPACE,
  ENTER,
  ESCAPE,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT,
  ARROW_UP,
  UNHANDLED,
  COUNT,
};

constexpr std::array<std::string, static_cast<size_t>( SpecialKey::COUNT )>
  SpecialKeyNames{
    "BACKSPACE",  "ENTER",       "ESCAPE",   "ARROW_DOWN",
    "ARROW_LEFT", "ARROW_RIGHT", "ARROW_UP", "UNHANDLED",
  };

class IO {
public:
  static void init();
  static void destroy();

  // Input
  static char get_ch();
  static SpecialKey get_special_key();

  // Output
  static void print_prompt( const ApplicationState & state );

  template <typename... Ts>
  static void print_no_flush( Ts &&... values ) {
    ( std::cout << ... << values );
  }

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

  static void flush_output();

  // State
  static std::string_view get_input_buffer();
  static void handle_enter();
  static bool handle_backspace();
  static bool handle_left_arrow();
  static bool handle_right_arrow();

private:
  static termios _oldt;
  static std::string _input_buffer;
  static std::size_t _cursor_pos;
  static std::string _last_prompt;

  static SpecialKey get_escape_or_arrow_key();
};
