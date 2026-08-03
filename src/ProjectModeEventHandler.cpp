#include "ProjectModeEventHandler.h"
#include "IO.h"

ModeResponse ProjectModeEventHandler::handle_key( Key key,
                                                  AppContext & context ) const {
  (void)context;
  if ( !key.special_type.has_value() ) {
    return ModeResponse{};
  }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( key.special_type.value() ) {
    default:
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}

ModeResponse
ProjectModeEventHandler::handle_action( const Action & action,
                                        AppContext & context ) const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( action.command_type ) {
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
      context.state.mode = AppMode::Sample;
      return ModeResponse{};

    default:
      IO::println( "Command not supported in Project mode" );
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
