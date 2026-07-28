#include "CommandRegistry.h"


void CommandRegistry::register_command( const Command & command ) {
  _commands.add( command );
}

const Commands & CommandRegistry::get_commands() const {
  return _commands;
}
