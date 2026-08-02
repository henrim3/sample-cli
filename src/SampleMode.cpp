#include "SampleMode.h"
#include "IO.h"
#include "ModeResponse.h"
#include "TypeDefs.h"

ModeResponse SampleMode::handle_key( Key key, AppContext & context ) {
  (void)context;
  if ( !key.special_type.has_value() ) {
    return ModeResponse{};
  }

  switch ( key.special_type.value() ) {
    case SpecialKeyType::Backspace:
    case SpecialKeyType::Enter:
    case SpecialKeyType::Escape:
    case SpecialKeyType::ArrowDown:
    case SpecialKeyType::ArrowLeft:
    case SpecialKeyType::ArrowRight:
    case SpecialKeyType::ArrowUp:
    case SpecialKeyType::Unhandled:
      return ModeResponse{};
  }
}

ModeResponse SampleMode::handle_action( const Action & action,
                                        AppContext & context ) {
  switch ( action.get_command_type() ) {
    case CommandType::Quit:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::Play: {
      MaybeSizeT maybe_sample_id = context.state.selected_sample_id;

      if ( !maybe_sample_id.has_value() ) {
        IO::print_error(
          "No sample selected, use 'list samples' to list available samples" );
        return ModeResponse{};
      }

      std::size_t sample_id = maybe_sample_id.value();

      Sample * sample =
        context.services.sample_manager.get_sample_by_id( sample_id );

      if ( sample == nullptr ) {
        IO::print_error( "Sample ", sample_id, " doesn't exist" );
      } else {
        context.services.voice_manager.create_voice( 0, *sample );
      }
      return ModeResponse{};
    }

    case CommandType::Deselect:
    case CommandType::List:
    case CommandType::ListSamples:
    case CommandType::New:
    case CommandType::NewSample:
    case CommandType::Select:
    case CommandType::SelectPad:
    case CommandType::SelectSample:
    case CommandType::Stop:
    case CommandType::StopAll:
      IO::println( "Command not supported in current mode" );
      return ModeResponse{};
      break;
  }
}
