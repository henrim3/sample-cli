#pragma once

#include "ApplicationState.h"
#include <iostream>
#include <string>
#include <termios.h>

class IO {
public:
  static void init();
  static void destroy();

  // Input
  static char get_ch();

  static std::string get_line();

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

private:
  static termios oldt;
};
