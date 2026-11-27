#include "Command.h"
#include <initializer_list>
#include <stdexcept>

std::string command_param_type_to_string( CommandParamType command_arg_type ) {
  switch ( command_arg_type ) {
    case CommandParamType::String:
      return "string";
    case CommandParamType::SizeT:
      return "size_t";
    case CommandParamType::Int:
      return "int";
    case CommandParamType::NonNegativeDouble:
      return "non-negative double";
    case CommandParamType::Bool:
      return "bool";
  }
}

CommandArg::CommandArg( const InitializeCommandArgArgs & args )
    : _type( args.type ), _value( args.value ) {}

CommandParamType CommandArg::get_type() const {
  return _type;
}

const CommandArgValue & CommandArg::get_value() const {
  return _value;
}

std::string CommandArg::to_string() const {
  std::string value_str;

  // this is so stupid
  std::visit(
    [&]( const auto & value ) {
      using T = std::decay_t<decltype( value )>;

      if constexpr ( std::is_same_v<T, int> ) {
        value_str = std::to_string( value );
      } else if constexpr ( std::is_same_v<T, std::size_t> ) {
        value_str = std::to_string( value );
      } else if constexpr ( std::is_same_v<T, std::string> ) {
        value_str = value;
      }
    },
    _value );

  return "CommandArg:\n  type: " + command_param_type_to_string( _type ) +
         "\n  value: " + value_str;
}

Commands::Commands( const std::vector<Command> & commands ) {
  for ( const Command & command : commands ) {
    _commands.insert_or_assign( command.token, command );
  }
}

Commands::Commands( std::initializer_list<Command> commands ) {
  for ( const Command & command : commands ) {
    _commands.insert_or_assign( command.token, command );
  }
}

const Command * Commands::get( std::string_view token ) const {
  try {
    return &_commands.at( std::string( token ) );
  } catch ( std::out_of_range & ) {
    return nullptr;
  }
}

void Commands::add( const Command & command ) {
  _commands.insert_or_assign( command.token, command );
}

void Commands::add( const Commands & commands ) {
  for ( const auto & command_entry : commands._commands ) {
    _commands[command_entry.second.token] = command_entry.second;
  }
}

bool Commands::is_empty() const {
  return _commands.size() == 0;
}

const std::map<std::string, Command> & Commands::get_commands() const {
  return _commands;
}
