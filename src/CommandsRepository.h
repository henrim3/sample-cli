#pragma once

#include "Command.h"
#include <vector>

class CommandsRepository {
public:
  void register_command(const Command &command);
  const std::vector<Command> &get_commands() const;

private:
  std::vector<Command> _commands;
};
