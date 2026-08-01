#include "SampleMode.h"
#include "IO.h"
#include <stdexcept>

ModeResponse SampleMode::handle_key( SpecialKey key, AppContext & context ) {
  switch ( key ) {
    case SpecialKey::Count:
      std::logic_error( "Got Count as key" );

    case SpecialKey::Backspace:
    case SpecialKey::Enter:
    case SpecialKey::Escape:
    case SpecialKey::ArrowDown:
    case SpecialKey::ArrowLeft:
    case SpecialKey::ArrowRight:
    case SpecialKey::ArrowUp:
    case SpecialKey::Unhandled:
      return ModeResponse{};
  }
}

ModeResponse SampleMode::handle_action( const Action & action,
                                        AppContext & context ) {
  switch ( action.get_command_type() ) {
    case CommandType::Count:
      throw std::logic_error( "Got Count as action" );

    case CommandType::Quit:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::Play: {
      Sample * sample = context.sample_manager.get_sample_by_id(
        context.state.get_selected_sample_id() );

      if ( sample == nullptr ) {
        IO::print_error( "No sample selected" );
      } else {
        context.voice_manager.create_voice( 0, *sample );
      }
      return ModeResponse{};
    }

    case CommandType::List:
    case CommandType::ListSamples:
    case CommandType::New:
    case CommandType::NewSample:
    case CommandType::Select:
    case CommandType::SelectPad:
    case CommandType::SelectSample:
      IO::println( "Command not supported in current mode" );
      return ModeResponse{};
      break;
  }
}
