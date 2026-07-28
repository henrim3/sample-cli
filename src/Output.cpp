#include "Output.h"
#include "ApplicationState.h"
#include <cstdio>
#include <iostream>

void Output::init() {
  std::setvbuf(stdout, NULL, _IONBF, 0); // disable output buffering
}

void Output::prompt( const ApplicationState & state ) {
  std::cout << "sample-cli ";

  // output selected sample id
  if (state.get_mode() == ApplicationMode::Sample) {
    std::cout << "[sample " << state.get_selected_sample_id() << "] ";
  }

  std::cout << "> ";
}

void Output::print( std::string_view s ) {
  std::cout << s;
}

void Output::println( std::string_view s ) {
  std::cout << s << std::endl;
}

void Output::error( std::string_view s ) {
  std::cout << s << std::endl;
}
