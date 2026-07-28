#pragma once

#include <initializer_list>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using CommandArgValue = std::variant<std::string, std::size_t, int>;
using MaybeCommandArgValue = std::optional<CommandArgValue>;
using CommandArgValues = std::vector<CommandArgValue>;
using MaybeCommandArgValues = std::optional<CommandArgValues>;

enum class CommandType {
  NEW,
  NEW_SAMPLE,
  SELECT,
  SELECT_PAD,
};

enum class CommandArgType {
  STR,
  SIZE_T,
  INT,
};

using CommandArgTypes = std::vector<CommandArgType>;

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
  std::vector<CommandArgType> arg_types = {};
  Commands subcommands = {};
};
