#include "Output.h"
#include <iostream>

void Output::prompt(const ApplicationState &state) {
  (void)state;
  std::cout << "sample-cli > ";
}

void Output::error(std::string_view s) { std::cout << s << std::endl; }
