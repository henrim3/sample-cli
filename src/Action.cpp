#include "Action.h"

Action::Action(const Command &command,
               const std::vector<CommandArgValue> &arg_values)
    : _command(command), _arg_values(arg_values) {}

const Command &Action::get_command() const {
  return _command;
}
const CommandArgValue &Action::get_arg_value(std::size_t i) const {
  return _arg_values[i];
}
