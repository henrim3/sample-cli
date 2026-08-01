#include "Application.h"
#include "ApplicationState.h"
#include "IO.h"
#include "Sample.h"
#include "SampleManager.h"

Application::Application( const CommandRegistry & command_registry )
    : _parser( Parser( command_registry ) ) {}

AppResponse Application::handle_special_key_pressed( SpecialKey key ) {
  // IO::println( "special key pressed: ",
  //              SpecialKeyNames[static_cast<std::size_t>( key )] );

  std::string line = std::string( IO::get_input_buffer() );

  // handle special keys first, it's only an action on enter

  // ONLY break for enter because it will handle comannd
  // REMEMBER TO RETURN or bad things happen :)
  switch ( key ) {
    case SpecialKey::Count: {
      throw std::logic_error( "Got COUNT as key" );
    }

    case SpecialKey::Backspace: {
      IO::handle_backspace();
      return AppResponse::KeepGoing;
    }

    case SpecialKey::ArrowDown: {
      return AppResponse::KeepGoing;
    }

    case SpecialKey::ArrowLeft: {
      IO::handle_left_arrow();
      return AppResponse::KeepGoing;
    }

    case SpecialKey::ArrowRight: {
      IO::handle_right_arrow();
      return AppResponse::KeepGoing;
    }

    case SpecialKey::ArrowUp: {
      return AppResponse::KeepGoing;
    }

    case SpecialKey::Enter: {
      IO::handle_enter();
      break;
    }

    case SpecialKey::Escape: {
      return AppResponse::KeepGoing;
    }

    case SpecialKey::Unhandled: {
      return AppResponse::KeepGoing;
    }
  }

  MaybeAction maybe_action = _parser.parse_action( line );

  if ( !maybe_action.has_value() ) {
    IO::println( "No action" );
    IO::print_prompt( _state );
    return AppResponse::KeepGoing;
  }

  // action has been parsed successfully
  const Action & action = maybe_action.value();

  AppResponse res = handle_action( action );

  IO::print_prompt( _state );

  return res;
}

AppResponse Application::handle_action( const Action & action ) {
  // MAKE SURE YOU RETURN!!
  switch ( action.get_command_type() ) {
    case CommandType::Count:

      // phonies go here
    case CommandType::New:
    case CommandType::Select:
      throw std::logic_error( "Got unsupported action somehow :(" );

    case CommandType::Play: {
      play();
      return AppResponse::KeepGoing;
    }

    case CommandType::Quit: {
      return AppResponse::Stop;
    }

    case CommandType::NewSample: {
      return load_sample( action.get_arg<std::string>( 0 ) );
    }

    case CommandType::SelectPad: {
      return AppResponse::KeepGoing;
    }

    case CommandType::SelectSample: {
      return select_sample( action.get_arg<std::size_t>( 0 ) );
    }
  }

  return AppResponse::KeepGoing;
}

SampleManager & Application::get_sample_manager() {
  return _sample_manager;
}

ApplicationState & Application::get_state() {
  return _state;
}

Sample * Application::get_selected_sample() {
  return _sample_manager.get_sample_by_id( _state.get_selected_sample_id() );
}

AppResponse Application::select_sample( std::size_t id ) {
  if ( !_sample_manager.has_sample( id ) ) {
    IO::print_error( "Sample ", id, " doesn't exist" );
    return AppResponse::KeepGoing;
  }

  _state.select_sample( id );
  return AppResponse::KeepGoing;
}

AppResponse Application::load_sample( std::string_view file_path ) {
  Sample * sample = _sample_manager.load_sample( file_path );
  if ( sample == nullptr ) {
    IO::print_error( "Couldn't load sample ", file_path );
    return AppResponse::KeepGoing;
  }

  IO::println( "Successfully loaded file ", file_path, " into sample ",
               sample->get_id() );

  return AppResponse::KeepGoing;
}

AppResponse Application::play_current_sample() {
  Sample * sample = get_selected_sample();
  if ( sample == nullptr ) {
    IO::print_error( "No sample selected" );
    return AppResponse::KeepGoing;
  }

  _voice_manager.create_voice( 0, *sample );

  return AppResponse::KeepGoing;
}

AppResponse Application::play() {
  switch ( _state.get_mode() ) {
    case ApplicationMode::Project:
      IO::print_error( "Command not supported yet!" );
      return AppResponse::KeepGoing;

    case ApplicationMode::Sample:
      return play_current_sample();
  }
}

std::ostream & operator<<( std::ostream & os, Application a ) {
  return os << a._state;
}
