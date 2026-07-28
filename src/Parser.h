#pragma once

#include "Action.h"
#include "Command.h"
#include "CommandRegistry.h"
#include <optional>
#include <unordered_set>

using Tokens = std::vector<std::string>;
using MaybeTokens = std::optional<Tokens>;

class Parser {
public:
  Parser( const CommandRegistry & command_registry );

  MaybeAction parse_action( std::string_view line ) const;

private:
  const CommandRegistry & _command_registry;

  MaybeTokens tokenize( std::string_view s ) const;

  MaybeTokens split_str( std::string_view s,
                         const std::unordered_set<char> & delimiters ) const;

  std::pair<const Command *, std::size_t>
  find_matching_command( const Tokens & tokens ) const;

  MaybeCommandArgValue validate_arg( std::string_view token,
                                     CommandArgType expected_type ) const;

  MaybeCommandArgValues
  validate_args( const std::vector<CommandArgType> & arg_types,
                 Tokens::const_iterator begin,
                 Tokens::const_iterator end ) const;
};
