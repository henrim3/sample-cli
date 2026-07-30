#include "Action.h"

Action::Action( const InitializeActionArgs & args )
    : _command_type( args.command_type ), _args( args.args ) {}

CommandType Action::get_command_type() const {
  return _command_type;
}

const CommandArgs & Action::get_args() const {
  return _args;
}

std::size_t Action::num_args() const {
  return _args.size();
}
