#pragma once

#include "CommandType.h"
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using CommandArgValue =
  std::variant<std::string, std::size_t, int, double, bool>;

enum class CommandParamType {
  String,
  SizeT,
  Int,
  NonNegativeDouble,
  Bool,
};

std::string command_param_type_to_string( CommandParamType command_arg_type );

using CommandParamTypes = std::vector<CommandParamType>;

struct CommandParam {
  std::string name;
  CommandParamType type;
};

using CommandParams = std::vector<CommandParam>;

struct InitializeCommandArgArgs {
  CommandParamType type;
  CommandArgValue value;
};

class CommandArg {
public:
  CommandArg( const InitializeCommandArgArgs & args );

  CommandParamType get_type() const;
  const CommandArgValue & get_value() const;

  std::string to_string() const;

private:
  CommandParamType _type;
  CommandArgValue _value;
};

using MaybeCommandArg = std::optional<CommandArg>;
using CommandArgs = std::vector<CommandArg>;
using MaybeCommandArgs = std::optional<CommandArgs>;

struct Command;

class Commands {
public:
  Commands();
  Commands( const std::vector<Command> & commands );
  Commands( std::initializer_list<Command> commands );

  const Command * get( std::string_view token ) const;

  void add( const Command & command );
  void add( const Commands & commands );

  bool is_empty() const;

  const std::vector<Command> & get_commands() const;

private:
  std::vector<Command> _commands;

  std::size_t expected_index( std::string_view token ) const;
};

struct Command {
  std::string token;
  MaybeCommandType type = std::nullopt;
  std::vector<CommandParam> params = {};
  Commands subcommands = {};
};

inline Commands::Commands() = default;
