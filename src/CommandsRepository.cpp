#include "CommandsRepository.h"

void CommandsRepository::register_command(const Command &command) {
  _commands.push_back(command);
}

const std::vector<Command> &CommandsRepository::get_commands() const {
  return _commands;
}

