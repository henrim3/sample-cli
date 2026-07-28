#pragma once

#include "Action.h"
#include "CommandRegistry.h"

class Parser {
public:
  Parser( const CommandRegistry & command_registry );

  std::vector<Action> parse_actions( std::string_view s ) const;

private:
  const CommandRegistry & _command_registry;

  std::vector<std::string> tokenize( std::string_view s ) const;

  std::vector<std::string>
  split_str( std::string_view s, const std::vector<char> & delimiters ) const;
};
