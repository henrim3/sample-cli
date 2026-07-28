#include "CommandRegistry.h"

CommandRegistry::CommandRegistry() : _commands( {} ) {}

CommandRegistry::CommandRegistry( std::initializer_list<Command> commands )
    : _commands( commands ) {}

void CommandRegistry::register_command( const Command & command ) {
  _commands.add( command );
}

const Commands & CommandRegistry::get_commands() const {
  return _commands;
}
