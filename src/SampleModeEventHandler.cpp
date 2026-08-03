#include "SampleModeEventHandler.h"
#include "IO.h"
#include "ModeResponse.h"
#include "TypeDefs.h"

ModeResponse SampleModeEventHandler::handle_key( Key key,
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
SampleModeEventHandler::handle_action( const Action & action,
                                       AppContext & context ) const {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( action.command_type ) {
    case CommandType::PlaySample: {
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

    default:
      IO::println( "Command not supported in Sample mode" );
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
