#pragma once

#include "Command.h"

struct InitializeActionArgs {
  CommandType command_type;
  const CommandArgs & args;
};

class Action {
public:
  Action( const InitializeActionArgs & args );

  CommandType get_command_type() const;
  const CommandArgs & get_args() const;
  const CommandArg & get_arg( std::size_t i ) const;
  std::size_t num_args() const;

private:
  const CommandType _command_type;
  CommandArgs _args;
};

using MaybeAction = std::optional<Action>;
