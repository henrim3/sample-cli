#include "DefaultActionHandler.h"
#include "AppContext.h"
#include "AppModeType.h"
#include "IO.h"
#include "ModeResponse.h"
#include "SampleManager.h"

ModeResponse DefaultActionHandler::handle_action( const Action & action,
                                                  AppContext & context ) {
  SampleManager & sample_manager = context.services.sample_manager;
  VoiceManager & voice_manager = context.services.voice_manager;

  switch ( action.get_command_type() ) {
    case CommandType::Quit:
      return ModeResponse{ .loop_should = LoopBehavior::Stop };

    case CommandType::Deselect:
      return ModeResponse{ .switch_to_mode = AppModeType::Project };

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
      return ModeResponse{ .switch_to_mode = AppModeType::Sample };
    }

    case CommandType::StopAll:
      voice_manager.stop_all();

    case CommandType::List:
    case CommandType::New:
    case CommandType::NewSample:
    case CommandType::Play:
    case CommandType::Select:
    case CommandType::SelectPad:
    case CommandType::Stop:
      return ModeResponse{};
  }
}
