#pragma once

#include "Action.h"
#include "CommandRegistry.h"
#include <unordered_set>

class Parser {
public:
  Parser( const CommandRegistry & command_registry );

  Action parse_action( std::string_view line ) const;

private:
  const CommandRegistry & _command_registry;

  std::vector<std::string> tokenize( std::string_view s ) const;

  std::vector<std::string>
  split_str( std::string_view s,
             const std::unordered_set<char> & delimiters ) const;

  const Command * find_matching_command() const;
};
