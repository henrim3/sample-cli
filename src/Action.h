#pragma once

#include "Command.h"
#include <vector>

struct ActionArgs {
  const Command & command;
  const std::vector<CommandArgValue> & arg_values;
};

class Action {
public:
  Action( const ActionArgs & args );

  const Command & get_command() const;
  const CommandArgValue & get_arg_value( std::size_t i ) const;

private:
  const Command & _command;
  std::vector<CommandArgValue> _arg_values;
};
