#include "ProjectMode.h"
#include "IO.h"
#include "ModeResponse.h"
#include "Sample.h"

ModeResponse ProjectMode::handle_key( Key key, AppContext & context ) {
  (void)context;
  if ( !key.special_type.has_value() ) {
    return ModeResponse{};
  }

  switch ( key.special_type.value() ) {
    case SpecialKeyType::Unhandled:
    case SpecialKeyType::ArrowDown:
    case SpecialKeyType::ArrowUp:
    case SpecialKeyType::Backspace:
      return ModeResponse{};

    case SpecialKeyType::Enter:
    case SpecialKeyType::Escape:
    case SpecialKeyType::ArrowLeft:
    case SpecialKeyType::ArrowRight:
      return ModeResponse{};
  }
}

ModeResponse ProjectMode::handle_action( const Action & action,
                                         AppContext & context ) {
  switch ( action.get_command_type() ) {
    case CommandType::Quit:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::NewSample: {
      Sample * sample = context.services.sample_manager.load_sample(
        action.get_arg<std::string>( 0 ) );

      if ( sample == nullptr ) {
        IO::print_error( "Couldn't load sample" );
      } else {
        IO::println( "Sample loaded successfully into sample ",
                     sample->get_id() );
      }

      return ModeResponse{};
    }

    case CommandType::SelectSample:
      context.state.selected_sample_id = action.get_arg<std::size_t>( 0 );
      return ModeResponse{};

    case CommandType::Deselect:
    case CommandType::List:
    case CommandType::ListSamples:
    case CommandType::Play:
    case CommandType::SelectPad:
    case CommandType::New:
    case CommandType::Select:
    case CommandType::Stop:
    case CommandType::StopAll:
      IO::println( "Command not supported in current mode" );
      return ModeResponse{};
      break;
  }
}
