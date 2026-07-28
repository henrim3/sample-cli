#include "CommandLoop.h"

#include "Application.h"
#include "Command.h"
#include "Output.h"
#include <optional>

CommandLoop::CommandLoop( Application & app, const Input & input,
                          const Parser & parser )
    : _app( app ), _input( input ), _parser( parser ) {}

void CommandLoop::run() {
  while ( true ) {
    Output::prompt( _app.get_state() );
    std::string line = _input.get_line();

    if ( line == "quit" ) {
      return;
    }

    std::optional<Action> maybe_action = _parser.parse_action( line );

    if ( !maybe_action.has_value() ) {
      Output::println( "No action" );
      continue;
    }

    const Action & action = maybe_action.value();

    Output::println(
      "Action type: " +
      CommandTypeNames[static_cast<std::size_t>( action.get_command_type() )] );

    std::size_t n_args = action.num_args();

    if ( n_args != 0 ) {
      Output::println( "Args:" );
      for ( std::size_t i = 0; i < n_args; i++ ) {
        Output::print( "Arg " + std::to_string( i ) + ":\n" +
                       action.get_arg( i ).to_string() );
      }
    } else {
      Output::println( "No args" );
    }
  }
}
