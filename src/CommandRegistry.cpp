#include "CommandRegistry.h"

#include <vector>

void CommandRegistry::register_command( const Command & command ) {
  _commands.push_back( command );
}

const std::vector<Command> & CommandRegistry::get_commands() const {
  return _commands;
}
