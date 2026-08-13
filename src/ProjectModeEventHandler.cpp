#include "ProjectModeEventHandler.h"
#include "IO.h"
#include "ModeResponse.h"
#include "PadManager.h"

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
  PadManager & pad_manager = context.services.pad_manager;
  SampleManager & sample_manager = context.services.sample_manager;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( action.command_type ) {
    case CommandType::ListSamples: {
      IO::println( "Samples:" );
      auto samples = sample_manager.get_samples();
      if ( samples.empty() ) {
        IO::println( "  No samples loaded" );
        return ModeResponse{};
      }
      for ( const auto & [key, value] : samples ) {
        IO::println( "  ", key, ": ", value.get_file_path() );
      }
      return ModeResponse{};
    }

    case CommandType::NewPad: {
      const Pad & pad = context.services.pad_manager.create_pad();
      IO::println( "Pad ", pad.get_id(), " created" );
      return ModeResponse{};
    }

    case CommandType::NewSample: {
      Sample * sample =
        sample_manager.load_sample( action.get_arg<std::string>( 0 ) );

      if ( sample == nullptr ) {
        IO::print_error( "Couldn't load sample" );
      } else {
        IO::println( "Sample loaded successfully into sample ",
                     sample->get_id() );
      }

      return ModeResponse{};
    }

    case CommandType::SelectPad: {
      std::size_t pad_id = action.get_arg<std::size_t>( 0 );
      const Pad * pad = pad_manager.get_pad_by_id( pad_id );
      if ( pad == nullptr ) {
        IO::print_error( "Pad ", pad_id, " doesn't exist" );
        return ModeResponse{};
      }
      context.state.selected_pad_id = pad_id;
      context.state.mode = AppMode::Pad;
      return ModeResponse{};
    }

    case CommandType::SelectSample: {
      std::size_t sample_id = action.get_arg<std::size_t>( 0 );
      const Sample * sample = sample_manager.get_sample_by_id( sample_id );
      if ( sample == nullptr ) {
        IO::print_error( "Sample ", sample_id, " doesn't exist" );
        return ModeResponse{};
      }
      context.state.selected_sample_id = sample_id;
      context.state.mode = AppMode::Sample;
      return ModeResponse{};
    }

    default:
      IO::println( "Command not supported in Project mode" );
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
