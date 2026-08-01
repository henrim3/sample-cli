#include "ProjectMode.h"
#include "IO.h"
#include "Sample.h"
#include <stdexcept>

ModeResponse ProjectMode::handle_key( SpecialKey key, AppContext & context ) {
  (void)context;
  switch ( key ) {
    case SpecialKey::Count:
      throw std::logic_error( "Got Count as key" );

    case SpecialKey::Unhandled:
    case SpecialKey::ArrowDown:
    case SpecialKey::ArrowUp:
    case SpecialKey::Backspace:
      return ModeResponse{};

    case SpecialKey::Enter:
    case SpecialKey::Escape:
    case SpecialKey::ArrowLeft:
    case SpecialKey::ArrowRight:
      return ModeResponse{};
  }
}

ModeResponse ProjectMode::handle_action( const Action & action,
                                         AppContext & context ) {
  switch ( action.get_command_type() ) {
    case CommandType::Count:
      throw std::logic_error( "Got Count as action" );

    case CommandType::Quit:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::NewSample: {
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

    case CommandType::SelectSample:
      context.state.select_sample( action.get_arg<std::size_t>( 0 ) );
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
