#include "SequenceModeEventHandler.h"
#include "ModeResponse.h"
#include <cstddef>

ModeResponse
SequenceModeEventHandler::handle_action( const Action & action,
                                         AppContext & context ) const {
  PlaybackEngine & playback_engine = context.services.playback_engine;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( action.command_type ) {
    case CommandType::NewNote: {
      std::size_t pad_id = action.get_arg<std::size_t>( 0 );
      double start_beat = action.get_arg<double>( 1 );
      double duration = action.get_arg<double>( 2 );
      bool one_shot = action.get_arg<bool>( 3 );

      playback_engine.add_note( start_beat, duration, one_shot, pad_id );

      return ModeResponse{};
    }

    case CommandType::PlaySequence:
      playback_engine.play();
      return ModeResponse{};

    case CommandType::SetSequenceLength:
      playback_engine.set_sequence_length( action.get_arg<double>( 0 ) );
      return ModeResponse{};

    case CommandType::StopSequence:
      playback_engine.stop();
      return ModeResponse{};

    default:
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
