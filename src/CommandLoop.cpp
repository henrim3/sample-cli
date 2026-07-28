#include "CommandLoop.h"

#include "Application.h"
#include "Output.h"

CommandLoop::CommandLoop( Application & app, const Input & input,
                          const Parser & parser )
    : _app( app ), _input( input ), _parser( parser ) {}

void CommandLoop::run() {
  while (true) {
    char c = _input.get_ch();
    Output::print( std::to_string( c ) );
    // Output::prompt( _app.get_state() );
    // std::vector<Action> actions = _parser.parse_actions();
  }
}
