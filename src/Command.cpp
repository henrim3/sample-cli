#include "Command.h"
#include <initializer_list>
#include <stdexcept>

Commands::Commands( std::initializer_list<Command> commands ) {
  for (const Command & command : commands) {
    _commands.insert_or_assign( command.token, command );
  }
}

const Command * Commands::get( std::string_view token ) const {
  try {
    return &_commands.at( std::string( token ) );
  } catch (std::out_of_range &) {
    return nullptr;
  }
}

void Commands::add( const Command & command ) {
  _commands.insert_or_assign( command.token, command );
}

bool Commands::is_empty() const {
  return _commands.size() == 0;
}
