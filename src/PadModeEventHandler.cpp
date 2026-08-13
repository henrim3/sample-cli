#include "PadModeEventHandler.h"
#include "IO.h"
#include "SampleManager.h"
#include <cstddef>

ModeResponse PadModeEventHandler::handle_action( const Action & action,
                                                 AppContext & context ) const {
  SampleManager & sample_manager = context.services.sample_manager;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( action.command_type ) {
    case CommandType::AssignSample: {
      std::size_t sample_id = action.get_arg<std::size_t>( 0 );
      Sample * sample = sample_manager.get_sample_by_id( sample_id );
      if ( sample == nullptr ) {
        IO::print_error( "Sample ", sample_id, " not found" );
        return ModeResponse{};
      }

      std::size_t pad_id = context.state.selected_pad_id.value();
      Pad * pad = context.services.pad_manager.get_pad_by_id( pad_id );

      if ( pad == nullptr ) {
        IO::print_error( "Pad not found" );
        return ModeResponse{};
      }

      pad->set_sample( sample );

      IO::println( "Assigned sample ", sample_id, " to pad ", pad_id );

      return ModeResponse{};
    }

    default:
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
