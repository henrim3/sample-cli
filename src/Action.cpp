#include "Action.h"

Action::Action( const InitializeActionArgs & args )
    : _command_type( args.command_type ),
      _args( args.args ),
      _is_global( args.is_global ) {}

CommandType Action::get_command_type() const {
  return _command_type;
}

const CommandArgs & Action::get_args() const {
  return _args;
}

std::size_t Action::num_args() const {
  return _args.size();
}

bool Action::is_global() const {
  return _is_global;
}
