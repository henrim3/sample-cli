#include "CommandLoop.h"

#include "Application.h"
#include "Command.h"
#include "IO.h"
#include <stdexcept>

CommandLoop::CommandLoop( Application & app, const Parser & parser )
    : _app( app ), _parser( parser ) {}

void CommandLoop::run() {
  while ( true ) {
    IO::print_prompt( _app.get_state() );
    std::string line = IO::get_line();

    MaybeAction maybe_action = _parser.parse_action( line );

    if ( !maybe_action.has_value() ) {
      IO::println( "No action" );
      continue;
    }

    // action has been parsed successfully
    const Action & action = maybe_action.value();

    IO::println(
      "Action type: ",
      CommandTypeNames[static_cast<std::size_t>( action.get_command_type() )] );

    std::size_t n_args = action.num_args();

    if ( n_args != 0 ) {
      IO::println( "Args:" );
      for ( std::size_t i = 0; i < n_args; i++ ) {
        IO::println( "Arg ", i, ":\n", action.get_arg( i ).to_string() );
      }
    } else {
      IO::println( "No args" );
    }

    LoopResult result = handle_action( action );
    switch ( result ) {
      case LoopResult::ERROR:
        IO::print_error( "Exiting on error!!" );
        return;
      case LoopResult::STOP:
        IO::println( "Exiting..." );
        return;
      case LoopResult::KEEP_GOING:; // no-op
    }
  }
}

LoopResult CommandLoop::handle_action( const Action & action ) {
  // MAKE SURE YOU RETURN!!
  switch ( action.get_command_type() ) {
    case CommandType::COUNT:

      // phonies go here
    case CommandType::NEW:
    case CommandType::SELECT:
      throw std::logic_error( "Got unsupported action somehow :(" );

    case CommandType::PLAY: {
      _app.play();
      return LoopResult::KEEP_GOING;
    }

    case CommandType::QUIT: {
      return LoopResult::STOP;
    }

    case CommandType::NEW_SAMPLE: {
      if ( !_app.load_sample(
             std::get<std::string>( action.get_arg( 0 ).get_value() ) ) ) {
        return LoopResult::KEEP_GOING;
      }
      return LoopResult::KEEP_GOING;
    }

    case CommandType::SELECT_PAD: {
      return LoopResult::KEEP_GOING;
    }

    case CommandType::SELECT_SAMPLE: {
      if ( !_app.select_sample(
             std::get<std::size_t>( action.get_arg( 0 ).get_value() ) ) ) {
        return LoopResult::KEEP_GOING;
      }
      return LoopResult::KEEP_GOING;
    }
  }

  return LoopResult::KEEP_GOING;
}
