#pragma once

#include "AppMode.h"
#include "Command.h"

using CommandsByMode = std::unordered_map<AppMode, Commands>;

class CommandRegistry {
public:
  CommandRegistry( std::initializer_list<CommandsByMode::value_type> commands );

  void register_global_commands( const std::vector<Command> & commands );
  void register_commands_for_mode( AppMode mode,
                                   const std::vector<Command> & commands );
  const Commands & get_global_commands() const;
  const Commands & get_mode_commands_for( AppMode mode ) const;

private:
  Commands _global_commands = {};
  CommandsByMode _mode_commands = {};
};
