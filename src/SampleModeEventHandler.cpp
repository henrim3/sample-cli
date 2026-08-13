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
  VoiceManager & voice_manager = context.services.voice_manager;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( action.command_type ) {
    case CommandType::PlaySample: {
      const Sample * sample = context.get_selected_sample();

      if ( sample == nullptr ) {
        IO::print_error( "Current sample doesn't exist" );
      } else {
        context.services.voice_manager.create_voice_for_sample( *sample );
      }
      return ModeResponse{};
    }

    case CommandType::StopSample: {
      const Sample * sample = context.get_selected_sample();
      if ( sample == nullptr ) {
        IO::print_error( "Current sample doesn't exist" );
        return ModeResponse{};
      }

      voice_manager.stop_voices_for_sample( sample->get_id() );
      return ModeResponse{};
    }

    default:
      IO::println( "Command not supported in Sample mode" );
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
