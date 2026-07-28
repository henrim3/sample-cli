#include "CommandLoop.h"

#include "Application.h"
#include "Output.h"
#include <optional>

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

    std::optional<Action> action = _parser.parse_action( line );

    if (action.has_value()) {
      Output::println( "Action type: " +
                       std::to_string( static_cast<int>(
                         action.value().get_command().type ) ) );
    } else {
      Output::println( "No action" );
    }
  }
}
