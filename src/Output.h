#pragma once

#include "ApplicationState.h"
#include <string_view>

class Output {
public:
  Output() = delete;

  static void prompt( const ApplicationState & state );

  static void print( std::string_view s );
  static void print( char c );
  static void print( int i );
  static void print( std::size_t n );

  static void println( std::string_view s );

  static void newline();

  static void error( std::string_view s );
};
