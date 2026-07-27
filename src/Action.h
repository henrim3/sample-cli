#pragma once

#include "Command.h"
#include <vector>

class Action {
public:
  Action(const Command &command, const std::vector<CommandArgValue> &arg_values);

  const Command &get_command() const;
  const CommandArgValue &get_arg_value(std::size_t i) const;

private:
  Command _command;
  std::vector<CommandArgValue> _arg_values;
};
