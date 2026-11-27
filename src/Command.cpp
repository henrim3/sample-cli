#include "Command.h"
#include <initializer_list>

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
    add( command );
  }
}

Commands::Commands( std::initializer_list<Command> commands ) {
  for ( const Command & command : commands ) {
    add( command );
  }
}

const Command * Commands::get( std::string_view token ) const {
  std::size_t index = expected_index( token );

  // reached end or not a substring
  if ( index == _commands.size() ) {
    return nullptr;
  }

  const Command & found_command = _commands[index];

  // not a substring match
  if ( token != found_command.token.substr( 0, token.size() ) ) {
    return nullptr;
  }

  // exact match
  if ( token == found_command.token ) {
    return &found_command;
  }

  // from here on it's a substring

  // last command, no ambiguity
  if ( index == _commands.size() - 1 ) {
    return &found_command;
  }

  // check if ambiguous with next command
  if ( token == _commands[index + 1].token.substr( 0, token.size() ) ) {
    return nullptr;
  }

  return &_commands[index];
}

void Commands::add( const Command & command ) {
  std::size_t insert_at = expected_index( command.token );
  _commands.insert( _commands.begin() + static_cast<int>( insert_at ),
                    command );
}

void Commands::add( const Commands & commands ) {
  for ( const Command & command : commands._commands ) {
    add( command );
  }
}

bool Commands::is_empty() const {
  return _commands.size() == 0;
}

const std::vector<Command> & Commands::get_commands() const {
  return _commands;
}

std::size_t Commands::expected_index( std::string_view token ) const {
  // TODO: binary search
  for ( std::size_t i = 0; i < _commands.size(); i++ ) {
    if ( token <= _commands[i].token ) {
      return i;
    }
  }
  return _commands.size();
}
