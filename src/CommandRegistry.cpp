#include "CommandRegistry.h"
#include "AppMode.h"

CommandRegistry::CommandRegistry(
  std::initializer_list<CommandsByMode::value_type> commands )
    : _mode_commands( commands ) {}

void CommandRegistry::register_global_commands(
  const std::vector<Command> & commands ) {
  _global_commands = Commands{ commands };
}

void CommandRegistry::register_commands_for_mode(
  AppMode mode, const std::vector<Command> & commands ) {
  if ( _mode_commands.contains( mode ) ) {
    _mode_commands.at( mode ).add( commands );
    return;
  }
  _mode_commands.insert_or_assign( mode, Commands{ commands } );
}

const Commands & CommandRegistry::get_global_commands() const {
  return _global_commands;
}

const Commands & CommandRegistry::get_mode_commands_for( AppMode mode ) const {
  return _mode_commands.at( mode );
}
