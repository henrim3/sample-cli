#pragma once

#include "Command.h"

class Action {
public:
  CommandType command_type;
  CommandArgs args;
  bool is_global = false;

  template <typename T>
  T get_arg( std::size_t i ) const {
    return std::get<T>( args[i].get_value() );
  }
};

using MaybeAction = std::optional<Action>;
