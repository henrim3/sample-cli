#include "Parser.h"
#include "Command.h"
#include "Output.h"
#include "StringConverter.h"

#include <cassert>
#include <cstdlib>
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

  if ( command == nullptr || command->is_phony ) {
    return std::nullopt;
  }

  if ( tokens.size() == depth && command->arg_types.size() == 0 ) {
    return Action( { .command_type = command->type, .args = {} } );
  }

  MaybeCommandArgs maybe_args = parse_args(
    command->arg_types, tokens.cbegin() + (int)depth, tokens.cend() );

  if ( !maybe_args.has_value() ) {
    return std::nullopt;
  }

  return Action(
    { .command_type = command->type, .args = maybe_args.value() } );
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

MaybeCommandArg Parser::validate_arg( std::string_view token,
                                      CommandArgType expected_type ) const {
  switch ( expected_type ) {
    case CommandArgType::INT: {
      MaybeInt i = StringConverter::try_parse_int( token );

      if ( !i.has_value() ) {
        return std::nullopt;
      }

      return CommandArg( { .type = CommandArgType::INT, .value = i.value() } );
    }

    case CommandArgType::SIZE_T: {
      MaybeSizeT n = StringConverter::try_parse_size_t( token );

      if ( !n.has_value() ) {
        return std::nullopt;
      }

      return CommandArg(
        { .type = CommandArgType::SIZE_T, .value = n.value() } );
    }

    case CommandArgType::STR: {
      return CommandArg(
        { .type = CommandArgType::STR, .value = std::string( token ) } );
    }

    case CommandArgType::COUNT: {
      throw std::logic_error( "Invalid CommandArgType" );
    }
  }

  throw std::logic_error( "how" );
}

MaybeCommandArgs Parser::parse_args( const CommandArgTypes & arg_types,
                                     Tokens::const_iterator begin,
                                     Tokens::const_iterator end ) const {
  std::size_t expected_n_args = arg_types.size();
  int n_args = end - begin;

  if ( expected_n_args != static_cast<std::size_t>( n_args ) ) {
    Output::error( "Invalid number of aruguments (" + std::to_string( n_args ) +
                   ") expected " + std::to_string( expected_n_args ) );
    return std::nullopt;
  }

  Tokens::const_iterator it;
  std::size_t i = 0;

  CommandArgs args;

  for ( it = begin; it != end; it++ ) {
    MaybeCommandArg maybe_arg = validate_arg( *it, arg_types[i] );

    if ( !maybe_arg.has_value() ) {
      Output::error( "Invalid argument " + std::to_string( i ) );
      return std::nullopt;
    }

    args.push_back( maybe_arg.value() );

    i++;
  }

  return args;
}
