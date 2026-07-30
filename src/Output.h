#pragma once

#include "ApplicationState.h"

class Output {
public:
  Output() = delete;

  static void prompt( const ApplicationState & state );

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

  static void newline();

  template <typename... Ts>
    static void debug( Ts &&... values ) {
      std::cout << "DEBUG: ";
      ( std::cout << ... << values );
      std::cout << std::endl;
    }

  template <typename... Ts>
  static void error( Ts &&... values ) {
    std::cout << "ERROR: ";
    ( std::cout << ... << values );
    std::cout << std::endl;
  }
};
