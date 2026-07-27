#pragma once

#include "CommandsRepository.h"
class CommandParser {
public:
  CommandParser(const CommandsRepository &commands_repository);
private:
  const CommandsRepository &_commands_repository;
};
