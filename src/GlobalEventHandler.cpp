#include "GlobalEventHandler.h"
#include "Command.h"
#include "IO.h"

GlobalEventHandler::GlobalEventHandler(
  const CommandRegistry & commandRegistry )
    : _commandRegistry( commandRegistry ) {}

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

    case CommandType::ListCommands: {
      Commands global_commands = _commandRegistry.get_global_commands();
      Commands mode_commands =
        _commandRegistry.get_mode_commands_for( context.state.mode );

      IO::println( "Global:" );
      output_all_commands( global_commands, 2 );

      IO::println( to_string( context.state.mode ), ":" );
      output_all_commands( mode_commands, 2 );

      return ModeResponse{};
    }

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

void GlobalEventHandler::output_all_commands( const Commands & commands,
                                              std::size_t indent ) const {
  if ( commands.is_empty() ) {
    return;
  }

  const std::map<std::string, Command> & repr = commands.get_commands();

  for ( const auto & [token, command] : repr ) {
    IO::print_indent( indent );
    IO::println( token );
    output_all_commands( command.subcommands, indent + 2 );
  }
}
