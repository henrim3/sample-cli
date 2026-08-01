#pragma once

#include "Command.h"

struct InitializeActionArgs {
  CommandType command_type;
  const CommandArgs & args;
  bool is_global;
};

class Action {
public:
  Action( const InitializeActionArgs & args );

  CommandType get_command_type() const;
  const CommandArgs & get_args() const;
  bool is_global() const;

  template <typename T>
  T get_arg( std::size_t i ) const {
    return std::get<T>( _args[i].get_value() );
  }

  std::size_t num_args() const;

private:
  const CommandType _command_type;
  CommandArgs _args;
  bool _is_global;
};

using MaybeAction = std::optional<Action>;
