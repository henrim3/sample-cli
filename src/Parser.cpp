#include "Parser.h"
#include "Output.h"

#include <cassert>
#include <cstdlib>
#include <optional>
#include <stdexcept>

Parser::Parser( const CommandRegistry & command_registry )
    : _command_registry( command_registry ) {}

std::optional<Action> Parser::parse_action( std::string_view line ) const {
  std::optional<std::vector<std::string>> tokens = tokenize( line );

  if (!tokens) {
    Output::error( "Parsing tokens failed" );
    return std::nullopt;
  }

  const Command * command = find_matching_command( tokens.value() );

  if (command == nullptr) {
    return std::nullopt;
  }

  return Action( { .command = *command, .arg_values = {} } );
}

std::optional<Tokens> Parser::tokenize( std::string_view s ) const {
  return split_str( s, { ' ', '\t' } );
}

std::optional<Tokens>
Parser::split_str( std::string_view string,
                   const std::unordered_set<char> & delimiters ) const {
  if (delimiters.empty()) {
    throw std::invalid_argument( "At least one delimiter must be provided" );
  }

  std::vector<std::string> v = {};
  std::string s = "";
  bool quote_started = false;
  for (std::size_t i = 0; i < string.size(); i++) {
    char c = string[i];

    // don't count delimiter if inside quotes
    if (!quote_started && delimiters.contains( c )) {
      if (s.size() != 0) {
        v.push_back( s );
        s = "";
      }
      continue;
    }

    if (c == '"') {
      // if quote already started push current string
      if (quote_started) {
        quote_started = false;

        if (s.size() != 0) {
          v.push_back( s );
          s = "";
        }

        continue;
      } else {
        quote_started = true;
        continue; // don't add quote character
      }
    }

    s.push_back( c );
  }

  if (quote_started) {
    return std::nullopt;
  }

  if (s.size() != 0) {
    v.push_back( s );
  }

  return v;
}

const Command * Parser::find_matching_command( const Tokens & tokens ) const {
  const Commands * commands = &_command_registry.get_commands();
  const Command * found = nullptr; // deepest found command

  for (std::string_view token : tokens) {
    if (commands->is_empty()) {
      return found;
    }

    const Command * command = commands->get( token );

    if (command == nullptr) {
      return found;
    }

    found = command;
    commands = &command->subcommands;
  }

  return found;
}
