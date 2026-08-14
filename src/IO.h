#pragma once

#include "Key.h"
#include "LineEditor.h"
#include <iostream>
#include <termios.h>

class IO {
public:
  // Lifecycle
  static void init();
  static void destroy();

  // Input
  static char get_ch();
  static Key get_key();

  // Render
  static void render( std::string_view prompt, const LineEditor & editor );

  // Output
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

private:
  static termios _oldt;

  static Key get_escape_or_arrow_key();
};
