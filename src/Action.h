#pragma once

#include "Command.h"
#include <vector>

class Action {
  Command command;
  std::vector<CommandArg> args;
};
