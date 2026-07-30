#include "Output.h"
#include "ApplicationState.h"
#include <iostream>

void Output::prompt( const ApplicationState & state ) {
  std::cout << "sample-cli ";

  // output selected sample id
  if ( state.get_mode() == ApplicationMode::Sample ) {
    std::cout << "[sample " << state.get_selected_sample_id() << "] ";
  }

  std::cout << "> " << std::flush;
}

template <typename T>
void Output::print( const T & value ) {
  std::cout << value << std::flush;
}

template <typename T>
void Output::println( const T & value ) {
  std::cout << value << std::endl;
}

void Output::newline() {
  std::cout << std::endl;
}

template <typename T>
void Output::error( const T & value ) {
  std::cout << value << std::endl;
}
