#pragma once

#include <string>
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

struct Command {
  CommandType type;
  std::string token;
  std::vector<CommandArgType> arg_types = {};
  std::vector<Command> subcommands = {};
  bool is_phony = false;
};
