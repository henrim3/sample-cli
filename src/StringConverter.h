#pragma once

#include <optional>
#include <string_view>

class StringConverter {
public:
  static std::optional<int> try_parse_int( std::string_view s );
  static std::optional<std::size_t> try_parse_size_t( std::string_view s );
};
