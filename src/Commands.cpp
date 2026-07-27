#include "Commands.h"

void Commands::register_command(const Command &command) {
  _commands.push_back(command);
}

const std::vector<Command> &Commands::get_commands() const {
  return _commands;
}

