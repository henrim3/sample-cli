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
  // MAKE SURE YOU ADD TO THE THING BELOW
  Deselect,
  List,
  ListSamples,
  Quit,
  New,
  NewSample,
  Play,
  Select,
  SelectPad,
  SelectSample,
  Stop,
  StopAll,
  Count
};

enum class CommandArgType {
  String,
  SizeT,
  Int,
  Count,
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
  Commands( std::initializer_list<Command> commands );

  const Command * get( std::string_view token ) const;
  void add( const Command & command );

  bool is_empty() const;

private:
  std::unordered_map<std::string, Command> _commands;
};

struct Command {
  std::string token;
  CommandType type;
  bool is_phony = false;
  bool is_global = false;
  std::vector<CommandArgType> arg_types = {};
  Commands subcommands = {};
};
