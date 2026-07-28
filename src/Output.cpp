#include "Output.h"
#include "ApplicationState.h"
#include <cstdio>
#include <iostream>

void Output::prompt( const ApplicationState & state ) {
  std::cout << "sample-cli ";

  // output selected sample id
  if (state.get_mode() == ApplicationMode::Sample) {
    std::cout << "[sample " << state.get_selected_sample_id() << "] ";
  }

  std::cout << "> " << std::flush;
}

void Output::print( std::string_view s ) {
  std::cout << s << std::flush;
}

void Output::print( char c ) {
  std::cout << c << std::flush;
}

void Output::print( int i ) {
  std::cout << i << std::flush;
}

void Output::print( std::size_t n ) {
  std::cout << n << std::flush;
}

void Output::println( std::string_view s ) {
  std::cout << s << std::endl;
}

void Output::newline() {
  std::cout << std::endl;
}

void Output::error( std::string_view s ) {
  std::cout << s << std::endl;
}
