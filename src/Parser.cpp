#include "Parser.h"

#include <cassert>
#include <optional>
#include <stdexcept>

Parser::Parser( const CommandRegistry & command_registry )
    : _command_registry( command_registry ) {}

std::optional<Action> Parser::parse_action( std::string_view line ) const {
  std::vector<std::string> tokens = tokenize( line );

  return {};
}

std::vector<std::string> Parser::tokenize( std::string_view s ) const {
  return split_str( s, { ' ', '\t' } );
}

std::vector<std::string>
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

  if (s.size() != 0) {
    v.push_back( s );
  }

  return v;
}

const Command * Parser::find_matching_command() const {
  // const std::vector<Command> & commands = _command_registry.get_commands();
  //
  // for (std::string_view token : tokens) {
  // }
  return nullptr;
}
