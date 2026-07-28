#include "StringConverter.h"
#include <cctype>
#include <optional>
#include <string>

std::optional<int> StringConverter::try_parse_int( std::string_view s ) {
  for ( char c : s ) {
    if ( !isdigit( c ) ) {
      return std::nullopt;
    }
  }
  return std::stoi( std::string( s ) );
}

std::optional<std::size_t>
StringConverter::try_parse_size_t( std::string_view s ) {
  std::optional<int> maybe_int = try_parse_int( s );
  if ( !maybe_int.has_value() ) {
    return std::nullopt;
  }

  int i = maybe_int.value();

  if ( i < 0 ) {
    return std::nullopt;
  }

  return static_cast<std::size_t>( i );
}
