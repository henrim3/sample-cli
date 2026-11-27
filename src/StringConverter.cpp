#include "StringConverter.h"
#include <cctype>
#include <optional>
#include <stdexcept>
#include <string>

MaybeInt StringConverter::try_parse_int( std::string_view s ) {
  for ( char c : s ) {
    if ( !isdigit( c ) ) {
      return std::nullopt;
    }
  }
  return std::stoi( std::string( s ) );
}

MaybeSizeT StringConverter::try_parse_size_t( std::string_view s ) {
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

MaybeDouble StringConverter::try_parse_double( std::string_view s ) {
  try {
    return std::stod( std::string( s ) );
  } catch ( const std::invalid_argument ) {
    return std::nullopt;
  } catch ( const std::out_of_range ) {
    return std::nullopt;
  }
}
