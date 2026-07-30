#include "Command.h"
#include <initializer_list>
#include <stdexcept>

CommandArg::CommandArg( const InitializeCommandArgArgs & args )
    : _type( args.type ), _value( args.value ) {}

CommandArgType CommandArg::get_type() const {
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

  return "CommandArg:\n  type: " +
         CommandArgTypeNames[static_cast<std::size_t>( _type )] +
         "\n  value: " + value_str;
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

bool Commands::is_empty() const {
  return _commands.size() == 0;
}
