#include "Parser.h"
#include "AppMode.h"
#include "Command.h"
#include "CommandRegistry.h"
#include "IO.h"
#include "StringConverter.h"

#include <cassert>
#include <cstdlib>
#include <stdexcept>

Parser::Parser( const CommandRegistry & command_registry )
    : _command_registry( command_registry ) {}

MaybeAction Parser::parse_global_action( std::string_view line ) const {
  MaybeTokens maybe_tokens = tokenize( line );

  if ( !maybe_tokens ) {
    IO::print_error( "Parsing tokens failed" );
    return std::nullopt;
  }

  const Tokens & tokens = maybe_tokens.value();

  auto [command, depth] =
    find_matching_command( tokens, _command_registry.get_global_commands() );

  if ( command == nullptr || !command->type.has_value() ) {
    return std::nullopt;
  }

  if ( tokens.size() == depth && command->params.size() == 0 ) {
    return Action{
      .command_type = command->type.value(),
      .args = {},
      .is_global = true,
    };
  }

  MaybeCommandArgs maybe_args = parse_args(
    command->params, tokens.cbegin() + (int)depth, tokens.cend() );

  if ( !maybe_args.has_value() ) {
    return std::nullopt;
  }

  return Action{
    .command_type = command->type.value(),
    .args = maybe_args.value(),
    .is_global = true,
  };
}

MaybeAction Parser::parse_mode_action( std::string_view line,
                                       AppMode mode ) const {
  MaybeTokens maybe_tokens = tokenize( line );

  if ( !maybe_tokens ) {
    IO::print_error( "Parsing tokens failed" );
    return std::nullopt;
  }

  const Tokens & tokens = maybe_tokens.value();

  auto [command, depth] = find_matching_command(
    tokens, _command_registry.get_mode_commands_for( mode ) );

  if ( command == nullptr || !command->type.has_value() ) {
    return std::nullopt;
  }

  if ( tokens.size() == depth && command->params.size() == 0 ) {
    return Action{
      .command_type = command->type.value(),
      .args = {},
    };
  }

  MaybeCommandArgs maybe_args = parse_args(
    command->params, tokens.cbegin() + (int)depth, tokens.cend() );

  if ( !maybe_args.has_value() ) {
    return std::nullopt;
  }

  return Action{
    .command_type = command->type.value(),
    .args = maybe_args.value(),
  };
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
Parser::find_matching_command( const Tokens & tokens,
                               const Commands & commands ) const {
  const Commands * commands_ptr = &commands;
  const Command * found = nullptr; // deepest found command
  std::size_t depth = 0;

  if ( commands_ptr->is_empty() ) {
    return { found, depth };
  }

  for ( std::string_view token : tokens ) {
    const Command * command = commands_ptr->get( token );

    if ( command == nullptr ) {
      return { found, depth };
    }

    found = command;
    commands_ptr = &command->subcommands;
    depth++;
  }

  return { found, depth };
}

MaybeCommandArg Parser::validate_arg( std::string_view token,
                                      CommandParamType expected_type ) const {
  switch ( expected_type ) {
    case CommandParamType::Int: {
      MaybeInt i = StringConverter::try_parse_int( token );

      if ( !i.has_value() ) {
        return std::nullopt;
      }

      return CommandArg( { .type = CommandParamType::Int, .value = i.value() } );
    }

    case CommandParamType::SizeT: {
      MaybeSizeT n = StringConverter::try_parse_size_t( token );

      if ( !n.has_value() ) {
        return std::nullopt;
      }

      return CommandArg(
        { .type = CommandParamType::SizeT, .value = n.value() } );
    }

    case CommandParamType::String: {
      return CommandArg(
        { .type = CommandParamType::String, .value = std::string( token ) } );
    }

    case CommandParamType::NonNegativeDouble: {
      MaybeDouble md = StringConverter::try_parse_double( token );

      if ( !md.has_value() ) {
        return std::nullopt;
      }

      double d = md.value();

      if ( d < 0 ) {
        return std::nullopt;
      }

      return CommandArg(
        { .type = CommandParamType::NonNegativeDouble, .value = d } );
    }

    case CommandParamType::Bool: {
      if ( token == "false" || token == "f" || token == "0" ) {
        return CommandArg( { .type = CommandParamType::Bool, .value = false } );
      }

      if ( token == "true" || token == "t" || token == "1" ) {
        return CommandArg( { .type = CommandParamType::Bool, .value = true } );
      }

      return std::nullopt;
    }
  }
}

MaybeCommandArgs Parser::parse_args( const CommandParams & params,
                                     Tokens::const_iterator begin,
                                     Tokens::const_iterator end ) const {
  std::size_t expected_n_args = params.size();
  int n_args = end - begin;

  if ( expected_n_args != static_cast<std::size_t>( n_args ) ) {
    IO::print_error( "Invalid number of arguments (" +
                     std::to_string( n_args ) + ") expected " +
                     std::to_string( expected_n_args ) );
    return std::nullopt;
  }

  Tokens::const_iterator it;
  std::size_t i = 0;

  CommandArgs args;

  for ( it = begin; it != end; it++ ) {
    MaybeCommandArg maybe_arg = validate_arg( *it, params[i].type );

    if ( !maybe_arg.has_value() ) {
      IO::print_error( "Invalid argument ", i );
      return std::nullopt;
    }

    args.push_back( maybe_arg.value() );

    i++;
  }

  return args;
}
