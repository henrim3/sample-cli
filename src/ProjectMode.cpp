#include "ProjectMode.h"
#include "AppMode.h"
#include "IO.h"
#include "Sample.h"
#include <stdexcept>

ModeResponse ProjectMode::handle_key( SpecialKey key, AppContext & context ) {
  switch ( key ) {
    case SpecialKey::COUNT:
      throw std::logic_error( "Got COUNT as key" );

    case SpecialKey::UNHANDLED:
    case SpecialKey::ARROW_DOWN:
    case SpecialKey::ARROW_UP:
    case SpecialKey::BACKSPACE:
      return ModeResponse{};

    case SpecialKey::ENTER:
    case SpecialKey::ESCAPE:
    case SpecialKey::ARROW_LEFT:
    case SpecialKey::ARROW_RIGHT:
      return ModeResponse{};
  }
}

ModeResponse ProjectMode::handle_action( const Action & action,
                                         AppContext & context ) {
  switch ( action.get_command_type() ) {
    case CommandType::COUNT:
      throw std::logic_error( "Got COUNT as action" );

    case CommandType::QUIT:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::NEW_SAMPLE: {
      Sample * sample =
        context.sample_manager.load_sample( action.get_arg<std::string>( 0 ) );

      if ( sample == nullptr ) {
        IO::print_error( "Couldn't load sample" );
      } else {
        IO::println( "Sample loaded successfully into sample ",
                     sample->get_id() );
      }

      return ModeResponse{};
    }

    case CommandType::SELECT_SAMPLE:
      context.state.select_sample( action.get_arg<std::size_t>( 0 ) );
      return ModeResponse{};

    case CommandType::PLAY: 
    case CommandType::SELECT_PAD:
    case CommandType::NEW:
    case CommandType::SELECT:
      IO::println( "Command not supported in current mode" );
      return ModeResponse{};
  }
}
