#pragma once

#include "Command.h"
#include <initializer_list>

class CommandRegistry {
public:
  CommandRegistry();
  CommandRegistry(std::initializer_list<Command> commands);

  void register_command( const Command & command );
  const Commands & get_commands() const;

private:
  Commands _commands;
};
