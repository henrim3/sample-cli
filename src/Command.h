#pragma once

#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

using CommandArgValue = std::variant<std::string, std::size_t, int>;

enum class CommandType {
  Deselect,

  Details,

  // History
  ShowHistory,
  ShowFullHistory,

  Quit,

  // Samples
  ListSamples,
  NewSample,
  PlaySample,
  SelectSample,
  StopSample,

  // Pads
  AssignSample,
  NewPad,
  PlayPad,
  SelectPad,
  StopPad,

  StopAll,
};

using MaybeCommandType = std::optional<CommandType>;

enum class CommandArgType {
  String,
  SizeT,
  Int,
};

using CommandArgTypes = std::vector<CommandArgType>;

struct InitializeCommandArgArgs {
  CommandArgType type;
  CommandArgValue value;
};

class CommandArg {
public:
  CommandArg( const InitializeCommandArgArgs & args );

  CommandArgType get_type() const;
  const CommandArgValue & get_value() const;

  std::string to_string() const;

private:
  CommandArgType _type;
  CommandArgValue _value;
};

using MaybeCommandArg = std::optional<CommandArg>;
using CommandArgs = std::vector<CommandArg>;
using MaybeCommandArgs = std::optional<CommandArgs>;

struct Command;

class Commands {
public:
  Commands( const std::vector<Command> & commands );
  Commands( std::initializer_list<Command> commands );

  const Command * get( std::string_view token ) const;

  void add( const Command & command );
  void add( const Commands & commands );

  bool is_empty() const;

private:
  std::unordered_map<std::string, Command> _commands = {};
};

struct Command {
  std::string token;
  MaybeCommandType type = std::nullopt;
  std::vector<CommandArgType> arg_types = {};
  Commands subcommands = {};
};
