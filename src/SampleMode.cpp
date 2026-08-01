#include "SampleMode.h"
#include "AppMode.h"
#include "IO.h"
#include <stdexcept>

ModeResponse SampleMode::handle_key( SpecialKey key, AppContext & context ) {
  switch ( key ) {
    case SpecialKey::COUNT:
      std::logic_error( "Got COUNT as key" );

    case SpecialKey::BACKSPACE:
    case SpecialKey::ENTER:
    case SpecialKey::ESCAPE:
    case SpecialKey::ARROW_DOWN:
    case SpecialKey::ARROW_LEFT:
    case SpecialKey::ARROW_RIGHT:
    case SpecialKey::ARROW_UP:
    case SpecialKey::UNHANDLED:
      return ModeResponse{};
  }
}

ModeResponse SampleMode::handle_action( const Action & action,
                                        AppContext & context ) {
  switch ( action.get_command_type() ) {
    case CommandType::COUNT:
      throw std::logic_error( "Got COUNT as action" );

    case CommandType::QUIT:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::PLAY: {
      Sample * sample = context.sample_manager.get_sample_by_id(
        context.state.get_selected_sample_id() );

      if ( sample == nullptr ) {
        IO::print_error( "No sample selected" );
      } else {
        context.voice_manager.create_voice( 0, *sample );
      }
      return ModeResponse{};
    }

    case CommandType::NEW:
    case CommandType::NEW_SAMPLE:
    case CommandType::SELECT:
    case CommandType::SELECT_PAD:
    case CommandType::SELECT_SAMPLE:
      IO::println( "Command not supported in current mode" );
      return ModeResponse{};
  }
}
