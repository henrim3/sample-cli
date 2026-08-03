#include "GlobalEventHandler.h"
#include "IO.h"

ModeResponse GlobalEventHandler::handle_action( const Action & action,
                                                AppContext & context ) const {
  SampleManager & sample_manager = context.services.sample_manager;
  VoiceManager & voice_manager = context.services.voice_manager;
  HistoryManager & history_manager = context.state.history_manager;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
  switch ( action.command_type ) {
    case CommandType::Quit:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::Deselect: {
      switch ( context.state.mode ) {
        case AppMode::Main:
          return ModeResponse{ .switch_to_mode = AppMode::Main };
        default:
          return ModeResponse{ .switch_to_mode = AppMode::Project };
      }
    }

    case CommandType::ShowHistory: {
      const std::vector<std::string> & entries = history_manager.get_entries();
      IO::println( "History:" );
      std::size_t start_idx = static_cast<std::size_t>(
        std::max( 0, static_cast<int>( entries.size() ) - 21 ) );
      for ( std::size_t i = start_idx; i < entries.size(); i++ ) {
        IO::println( "  ", i, ": ", entries[i] );
      }
      return ModeResponse{};
    }

    case CommandType::ShowFullHistory: {
      const std::vector<std::string> & entries = history_manager.get_entries();
      for ( std::size_t i = 0; i < entries.size(); i++ ) {
        IO::println( "  ", i, ": ", entries[i] );
      }
      return ModeResponse{};
    }

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

    case CommandType::SelectSample: {
      std::size_t id = action.get_arg<std::size_t>( 0 );
      if ( !sample_manager.has_sample( id ) ) {
        IO::print_error( "Sample ", id, " doesn't exist" );
        return ModeResponse{};
      }
      context.state.selected_sample_id = id;
      return ModeResponse{ .switch_to_mode = AppMode::Sample };
    }

    case CommandType::StopAll:
      voice_manager.stop_all();

    default:
      return ModeResponse{};
  }
#pragma GCC diagnostic pop
}
