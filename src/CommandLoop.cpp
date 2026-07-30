#include "CommandLoop.h"

#include "Application.h"
#include "Command.h"
#include "Output.h"
#include <stdexcept>

CommandLoop::CommandLoop( Application & app, const Input & input,
                          const Parser & parser )
    : _app( app ), _input( input ), _parser( parser ) {}

void CommandLoop::run() {
  while ( true ) {
    Output::prompt( _app.get_state() );
    std::string line = _input.get_line();

    MaybeAction maybe_action = _parser.parse_action( line );

    if ( !maybe_action.has_value() ) {
      Output::println( "No action" );
      continue;
    }

    // action has been parsed successfully
    const Action & action = maybe_action.value();

    Output::println(
      "Action type: ",
      CommandTypeNames[static_cast<std::size_t>( action.get_command_type() )] );

    std::size_t n_args = action.num_args();

    if ( n_args != 0 ) {
      Output::println( "Args:" );
      for ( std::size_t i = 0; i < n_args; i++ ) {
        Output::println( "Arg ", i, ":\n", action.get_arg( i ).to_string() );
      }
    } else {
      Output::println( "No args" );
    }

    LoopResult result = handle_action( action );
    switch ( result ) {
      case LoopResult::ERROR:
        Output::error( "Exiting on error!!" );
        return;
      case LoopResult::STOP:
        Output::println( "Exiting..." );
        return;
      case LoopResult::KEEP_GOING:; // no-op
    }
  }
}

LoopResult CommandLoop::handle_action( const Action & action ) {
  switch ( action.get_command_type() ) {
    case CommandType::QUIT:
      return LoopResult::STOP;

    case CommandType::COUNT:
      throw std::logic_error( "Got COUNT action somehow :(" );

    // phonies
    case CommandType::NEW:
    case CommandType::SELECT:
      return LoopResult::KEEP_GOING;

    case CommandType::NEW_SAMPLE:
      if ( _app.get_sample_manager().load_sample( std::get<std::string>(
             action.get_arg( 0 ).get_value() ) ) == nullptr ) {
        return LoopResult::ERROR;
      }
      return LoopResult::KEEP_GOING;

    case CommandType::SELECT_PAD:
      return LoopResult::KEEP_GOING;

    case CommandType::SELECT_SAMPLE:
      return LoopResult::KEEP_GOING;
  }

  return LoopResult::KEEP_GOING;
}
