#include "Parser.h"
#include "Command.h"
#include "Output.h"
#include "StringConverter.h"
#include "StringValidation.h"

#include <cassert>
#include <cstdlib>
#include <optional>
#include <stdexcept>

Parser::Parser( const CommandRegistry & command_registry )
    : _command_registry( command_registry ) {}

MaybeAction Parser::parse_action( std::string_view line ) const {
  MaybeTokens maybe_tokens = tokenize( line );

  if ( !maybe_tokens ) {
    Output::error( "Parsing tokens failed" );
    return std::nullopt;
  }

  const Tokens & tokens = maybe_tokens.value();

  auto [command, depth] = find_matching_command( tokens );

  if ( command == nullptr ) {
    return std::nullopt;
  }

  if ( tokens.size() == depth ) {
    return Action( { .command = *command, .arg_values = {} } );
  }

  MaybeCommandArgValues maybe_arg_values = validate_args(
    command->arg_types, tokens.cbegin() + (int)depth, tokens.cend() );

  return Action(
    { .command = *command, .arg_values = maybe_arg_values.value_or( {} ) } );
}

MaybeTokens Parser::tokenize( std::string_view s ) const {
  return split_str( s, { ' ', '\t' } );
}

MaybeTokens
Parser::split_str( std::string_view string,
                   const std::unordered_set<char> & delimiters ) const {
  if ( delimiters.empty() ) {
    throw std::invalid_argument( "At least one delimiter must be provided" );
  }

  std::vector<std::string> v = {};
  std::string s = "";
  bool quote_started = false;
  for ( std::size_t i = 0; i < string.size(); i++ ) {
    char c = string[i];

    // don't count delimiter if inside quotes
    if ( !quote_started && delimiters.contains( c ) ) {
      if ( s.size() != 0 ) {
        v.push_back( s );
        s = "";
      }
      continue;
    }

    if ( c == '"' ) {
      // if quote already started push current string
      if ( quote_started ) {
        quote_started = false;

        if ( s.size() != 0 ) {
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

  if ( quote_started ) {
    return std::nullopt;
  }

  if ( s.size() != 0 ) {
    v.push_back( s );
  }

  return v;
}

std::pair<const Command *, std::size_t>
Parser::find_matching_command( const Tokens & tokens ) const {
  const Commands * commands = &_command_registry.get_commands();
  const Command * found = nullptr; // deepest found command
  std::size_t depth = 0;

  for ( std::string_view token : tokens ) {
    if ( commands->is_empty() ) {
      return { found, depth };
    }

    const Command * command = commands->get( token );

    if ( command == nullptr ) {
      return { found, depth };
    }

    found = command;
    commands = &command->subcommands;
    depth++;
  }

  return { found, depth };
}

MaybeCommandArgValue
Parser::validate_arg( std::string_view token,
                      CommandArgType expected_type ) const {
  switch ( expected_type ) {
    case CommandArgType::INT:
      return StringConverter::try_parse_int( token );
    case CommandArgType::SIZE_T:
      return StringConverter::try_parse_size_t( token );
    case CommandArgType::STR:
      return std::string( token );
  }
}

MaybeCommandArgValues
Parser::validate_args( const CommandArgTypes & arg_types,
                       Tokens::const_iterator begin,
                       Tokens::const_iterator end ) const {
  std::size_t expected_n_args = arg_types.size();
  int n_args = end - begin;

  if ( expected_n_args != static_cast<std::size_t>( n_args ) ) {
    return std::nullopt;
  }

  Tokens::const_iterator it;
  std::size_t i = 0;

  for ( it = begin; it != end; it++ ) {
    MaybeCommandArgValue maybe_val = validate_arg( *it, arg_types[i] );
    i++;
  }

  return std::nullopt;
}
