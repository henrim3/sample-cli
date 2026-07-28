#pragma once

#include "Action.h"
#include "CommandRegistry.h"
#include <optional>
#include <unordered_set>

using Tokens = std::vector<std::string>;

class Parser {
public:
  Parser( const CommandRegistry & command_registry );

  std::optional<Action> parse_action( std::string_view line ) const;

private:
  const CommandRegistry & _command_registry;

  std::optional<Tokens> tokenize( std::string_view s ) const;

  std::optional<Tokens>
  split_str( std::string_view s,
             const std::unordered_set<char> & delimiters ) const;

  const Command * find_matching_command( const Tokens & tokens ) const;
};
