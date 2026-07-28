#include "Action.h"

Action::Action( const InitializeActionArgs & args )
    : _command( args.command ), _arg_values( args.arg_values ) {}

const Command & Action::get_command() const {
  return _command;
}
const CommandArgValue & Action::get_arg_value( std::size_t i ) const {
  return _arg_values[i];
}
