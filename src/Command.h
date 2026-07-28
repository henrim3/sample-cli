#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

enum class CommandType {
  SELECT,
  SELECT_PAD,
};

enum class CommandArgType {
  STR,
  SIZE_T,
  INT,
};

using CommandArgValue = std::variant<std::string, std::size_t, int>;

struct Command;

class Commands {
public:
  Commands( std::initializer_list<Command> commands );

  const Command * get( std::string_view token ) const;
  void add( const Command & command );

private:
  std::unordered_map<std::string, Command> _commands;
};

struct Command {
  CommandType type;
  std::string token;
  std::vector<CommandArgType> arg_types = {};
  Commands subcommands = {};
  bool is_phony = false;
};
