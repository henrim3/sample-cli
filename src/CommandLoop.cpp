#include "CommandLoop.h"

#include "Application.h"
#include "Output.h"

CommandLoop::CommandLoop( Application & app, const Input & input,
                          const Parser & parser )
    : _app( app ), _input( input ), _parser( parser ) {}

void CommandLoop::run() {
  while (true) {
    Output::prompt( _app.get_state() );
    std::string line = _input.get_line();
    if (line == "quit") {
      return;
    }
  }
}
