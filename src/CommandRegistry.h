#pragma once

#include "Command.h"

class CommandRegistry {
public:
  void register_command( const Command & command );
  const Commands & get_commands() const;

private:
  Commands _commands = {};
};
