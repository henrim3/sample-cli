#include "Output.h"
#include "ApplicationState.h"
#include <iostream>


void Output::prompt( const ApplicationState & state ) {
  std::cout << "sample-cli ";

  // output selected sample id
  if (state.get_mode() == ApplicationMode::Sample) {
    std::cout << "[sample " << state.get_selected_sample_id() << "] ";
  }

  std::cout << "> ";
}

void Output::print( std::string_view s ) {
  std::cout << s << std::endl;
}

void Output::error( std::string_view s ) {
  std::cout << s << std::endl;
}
