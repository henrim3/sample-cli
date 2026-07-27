#pragma once

#include <string>
#include <vector>

enum class CommandType {};

enum class CommandArgType {
  STR,
  SIZE_T,
  INT,
};

union CommandArg {
  std::string str;
  std::size_t size_t_;
  int int_;
};

struct Command {
  std::string token;
  CommandType type;
  std::vector<CommandArgType> arg_types;
  std::vector<Command> subcommands;
};
