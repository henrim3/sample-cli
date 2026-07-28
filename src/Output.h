#pragma once

#include "ApplicationState.h"
#include <string_view>

class Output {
public:
  Output() = delete;

  static void init();

  static void prompt( const ApplicationState & state );
  static void print( std::string_view s );
  static void println( std::string_view s );
  static void error( std::string_view s );
};
