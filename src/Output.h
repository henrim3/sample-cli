#pragma once

#include "ApplicationState.h"

class Output {
public:
  Output() = delete;

  static void prompt( const ApplicationState & state );

  template <typename T>
  static void print( const T & value );

  template <typename T>
  static void println( const T & value );

  static void newline();

  template <typename T>
  static void error( const T & value );
};
