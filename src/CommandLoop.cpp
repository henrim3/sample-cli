#include "CommandLoop.h"

#include "Application.h"
#include "Command.h"
#include "IO.h"
#include <stdexcept>

CommandLoop::CommandLoop( Application & app, const Parser & parser )
    : _app( app ), _parser( parser ) {}

void CommandLoop::run() {
  IO::print_prompt( _app.get_state() );

  while ( true ) {
    SpecialKey key = IO::get_special_key();

    LoopResult result = handle_special_key_pressed( key );

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

LoopResult CommandLoop::handle_special_key_pressed( SpecialKey key ) {
  // IO::println( "special key pressed: ",
  //              SpecialKeyNames[static_cast<std::size_t>( key )] );

  std::string line = std::string( IO::get_input_buffer() );

  // handle special keys first, it's only an action on enter

  // ONLY break for enter because it will handle comannd
  // REMEMBER TO RETURN or bad things happen :)
  switch ( key ) {
    case SpecialKey::COUNT: {
      throw std::logic_error( "Got COUNT as key" );
    }

    case SpecialKey::BACKSPACE: {
      IO::handle_backspace();
      // erase character from the screen
      IO::print( '\b' );
      IO::print( ' ' );
      IO::print( '\b' );
      return LoopResult::KEEP_GOING;
    }

    case SpecialKey::ARROW_DOWN: {
      return LoopResult::KEEP_GOING;
    }

    case SpecialKey::ARROW_LEFT: {
      IO::handle_left_arrow();
      return LoopResult::KEEP_GOING;
    }

    case SpecialKey::ARROW_RIGHT: {
      IO::handle_right_arrow();
      return LoopResult::KEEP_GOING;
    }

    case SpecialKey::ARROW_UP: {
      return LoopResult::KEEP_GOING;
    }

    case SpecialKey::ENTER: {
      IO::handle_enter();
      break;
    }

    case SpecialKey::ESCAPE: {
      return LoopResult::KEEP_GOING;
    }

    case SpecialKey::UNHANDLED: {
      return LoopResult::KEEP_GOING;
    }
  }

  MaybeAction maybe_action = _parser.parse_action( line );

  if ( !maybe_action.has_value() ) {
    IO::println( "No action" );
    IO::print_prompt( _app.get_state() );
    return LoopResult::KEEP_GOING;
  }

  // action has been parsed successfully
  const Action & action = maybe_action.value();

  LoopResult res = handle_action( action );

  IO::print_prompt( _app.get_state() );

  return res;

  // IO::println(
  //   "Action type: ",
  //   CommandTypeNames[static_cast<std::size_t>( action.get_command_type() )]
  //   );
  //
  // std::size_t n_args = action.num_args();
  //
  // if ( n_args != 0 ) {
  //   IO::println( "Args:" );
  //   for ( std::size_t i = 0; i < n_args; i++ ) {
  //     IO::println( "Arg ", i, ":\n", action.get_arg( i ).to_string() );
  //   }
  // } else {
  //   IO::println( "No args" );
  // }
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
      if ( !_app.load_sample( action.get_arg<std::string>( 0 ) ) ) {
        return LoopResult::KEEP_GOING;
      }
      return LoopResult::KEEP_GOING;
    }

    case CommandType::SELECT_PAD: {
      return LoopResult::KEEP_GOING;
    }

    case CommandType::SELECT_SAMPLE: {
      if ( !_app.select_sample( action.get_arg<std::size_t>( 0 ) ) ) {
        return LoopResult::KEEP_GOING;
      }
      return LoopResult::KEEP_GOING;
    }
  }

  return LoopResult::KEEP_GOING;
}
